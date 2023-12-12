import readline from 'node:readline'
import process from 'node:process'
import url from 'node:url'
import path from 'node:path'
import fsPromises from 'node:fs/promises'
import chokidar from 'chokidar'
import * as rollup from 'rollup'
import typescript from '@rollup/plugin-typescript'

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

const rollupWatchers = new Map()

async function main() {
  await fsPromises.rm(buildDir, {
    recursive: true,
    force: true,
  })
  await fsPromises.mkdir(buildDir)

  const watcher = chokidar.watch(srcDir)

  watcher.on('add', async (filePath) => {
    console.log('[Add watcher]', filePath)

    if (!rollupWatchers.has(filePath)) {
      const inputOptions = {
        input: filePath,
        plugins: [
          typescript({
            filterRoot: false,
            module: 'es6'
          }),
        ],
      }

      const outputOptions = {
        file: getBuildPath(filePath),
        format: 'umd',
        name: 'globalThis.__exports'
      }

      const rw = rollup.watch({
        ...inputOptions,
        output: [outputOptions],
        watch: {
          clearScreen: false,
        },
      })

      rw.on('event', ({ result }) => {
        if (result) {
          console.log('[Successfully compiled]', filePath)
          result.close()
        }
      })

      rollupWatchers.set(filePath, rw)
    }
  })

  watcher.on('unlink', async (filePath) => {
    if (rollupWatchers.has(filePath)) {
      console.log('[Close watcher]', filePath)
      await rollupWatchers.get(filePath).close()
      rollupWatchers.delete(filePath)
    }

    await fsPromises.rm(getBuildPath(filePath), {
      recursive: true,
      force: true,
    })
  })

}

const rl = readline.createInterface({ input: process.stdin, output: process.stdout })
rl.question(`Build directory "${buildDir}" will be deleted before continuing - ok?\n(Y/n) `, (answer) => {
  if (answer === 'y' || answer === 'Y') {
    main()
  } else {
    process.exit(-1)
  }
})
