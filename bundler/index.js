import readline from 'node:readline'
import process from 'node:process'
import path from 'node:path'
import fsPromises from 'node:fs/promises'
import chokidar from 'chokidar'
import ts from 'typescript'
import * as esbuild from 'esbuild'

if (process.argv.length !== 4) {
  console.error(`Usage: node index.js C:\\absolute\\path\\to\\source\\directory C:\\absolute\\path\\to\\build\\directory`)
  process.exit(-1)
}

const srcDir = process.argv[2]
console.log('Source directory:', srcDir)
const buildDir = process.argv[3]
console.log('Build (output) directory:', buildDir)

function getBuildPath(filePath) {
  const relativePath = path.relative(srcDir, filePath)
  const parsedPath = path.parse(relativePath)
  return path.join(buildDir, parsedPath.dir, parsedPath.name + '.js')
}

const esbuildWatchers = new Map()

async function main() {
  const tsConfig = JSON.parse(await fsPromises.readFile(path.join(srcDir, 'tsconfig.json')))
  console.warn('[Warning] tsconfig.json will not be watched - you must restart the bundler if you change this file')

  await fsPromises.rm(buildDir, {
    recursive: true,
    force: true,
  })
  await fsPromises.mkdir(buildDir)

  const watcher = chokidar.watch(srcDir)

  watcher.on('add', async (filePath) => {
    const filePathRegexFilter = /\.tsx?/

    if (!filePath.match(filePathRegexFilter)) {
      console.log('[Skipping]', filePath)
      return
    }

    console.log('[Add watcher]', filePath)

    if (!esbuildWatchers.has(filePath)) {
      const ctx = await esbuild.context({
        entryPoints: [filePath],
        outfile: getBuildPath(filePath),
        write: false,
        bundle: true,
        format: 'iife',
        globalName: 'globalThis.__djinnExports',
        plugins: [
          {
            name: 'custom-plugin',
            setup(build) {
              build.onLoad({ filter: filePathRegexFilter }, async (args) => {
                const fileContents = await fsPromises.readFile(args.path, 'utf8')
                const compilerOptions = {
                  ...tsConfig.compilerOptions,
                  noEmitOnError: true,
                }
                const host = ts.createCompilerHost(compilerOptions);
                host.writeFile = (fileName, contents) => {}
                const program = ts.createProgram([args.path], compilerOptions, host)
                const emitResult = program.emit()
                const allDiagnostics = ts.getPreEmitDiagnostics(program).concat(emitResult.diagnostics)
                allDiagnostics.forEach(diagnostic => {
                  if (diagnostic.file) {
                    let { line, character } = ts.getLineAndCharacterOfPosition(diagnostic.file, diagnostic.start)
                    let message = ts.flattenDiagnosticMessageText(diagnostic.messageText, "\n")
                    console.log(`${diagnostic.file.fileName} (${line + 1},${character + 1}): ${message}`)
                  } else {
                    console.log(ts.flattenDiagnosticMessageText(diagnostic.messageText, "\n"))
                  }
                })
                if (emitResult.emitSkipped) {
                  throw new Error(`[Error] ${args.path}: TypeScript compilation failed - see above errors`)
                } else {
                  return {
                    contents: fileContents,
                    loader: 'ts',
                  }
                }
              })
              build.onEnd(async (result) => {
                if (result.errors.length === 0) {
                  for (const outFile of result.outputFiles) {
                    await fsPromises.writeFile(outFile.path, outFile.text.replace(/\s*var globalThis;\s*/g, ''), 'utf8')
                  }
                  console.log(`[Success] ${filePath}`)
                } else {
                  console.error(`[Error] ${filePath} - see above errors`)
                }
                if (result.warnings.length > 0) {
                  console.warn(`[Warn] ${filePath}`, result.warnings)
                }
              })
            }
          }
        ]
      })

      await ctx.watch()

      esbuildWatchers.set(filePath, ctx)
    }
  })

  watcher.on('unlink', async (filePath) => {
    if (esbuildWatchers.has(filePath)) {
      console.log('[Close watcher]', filePath)
      await esbuildWatchers.get(filePath).dispose()
      esbuildWatchers.delete(filePath)
    }

    await fsPromises.rm(getBuildPath(filePath), {
      recursive: true,
      force: true,
    })
  })
}

const rl = readline.createInterface({ input: process.stdin, output: process.stdout })
rl.question(`Build directory "${buildDir}" will be deleted before continuing - ok?\n(Y/n) `, (answer) => {
  if (answer.toLowerCase() === 'y') {
    main()
  } else {
    process.exit(-1)
  }
})
