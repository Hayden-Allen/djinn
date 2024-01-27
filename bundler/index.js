import readline from "node:readline";
import process from "node:process";
import path from "node:path";
import chokidar from "chokidar";
import ts from "typescript";
import * as esbuild from "esbuild";
import fs from "node:fs";

if (process.argv.length !== 4) {
  console.error(
    `Usage: node index.js C:\\absolute\\path\\to\\source\\directory C:\\absolute\\path\\to\\build\\directory`
  );
  process.exit(-1);
}

const srcDir = process.argv[2];
console.log("Source directory:", srcDir);
const buildDir = process.argv[3];
console.log("Build (output) directory:", buildDir);

function getBuildPath(filePath) {
  const relativePath = path.relative(srcDir, filePath);
  const parsedPath = path.parse(relativePath);
  return path.join(buildDir, parsedPath.dir, parsedPath.name + ".js");
}

const esbuildWatchers = new Map();

function main() {
  const tsConfig = JSON.parse(
    fs.readFileSync(path.join(srcDir, "tsconfig.json"))
  );
  console.warn(
    "[Warning] tsconfig.json will not be watched - you must restart the bundler if you change this file"
  );

  fs.rmSync(buildDir, {
    recursive: true,
    force: true,
  });
  fs.mkdirSync(buildDir);

  const watcher = chokidar.watch(srcDir);

  watcher.on("add", async (filePath) => {
    const filePathRegexFilter = /[^\.d]\.tsx?/;

    if (!filePath.match(filePathRegexFilter)) {
      console.log("[Skipping]", filePath);
      return;
    }

    console.log("[Add watcher]", filePath);

    if (!esbuildWatchers.has(filePath)) {
      const ctx = await esbuild.context({
        entryPoints: [filePath],
        outfile: getBuildPath(filePath),
        write: false,
        bundle: true,
        format: "iife",
        globalName: "globalThis.__djinnExports",
        plugins: [
          {
            name: "custom-plugin",
            setup(build) {
              build.onLoad({ filter: filePathRegexFilter }, (args) => {
                const fileContents = fs.readFileSync(args.path, "utf8");
                const compilerOptions = {
                  ...tsConfig.compilerOptions,
                  target: ts.ScriptTarget.ES2015,
                  noEmitOnError: true,
                };
                const host = ts.createCompilerHost(compilerOptions);
                host.writeFile = (fileName, contents) => {};
                const program = ts.createProgram(
                  [args.path],
                  compilerOptions,
                  host
                );
                const emitResult = program.emit();
                const allDiagnostics = ts
                  .getPreEmitDiagnostics(program)
                  .concat(emitResult.diagnostics);
                allDiagnostics.forEach((diagnostic) => {
                  if (diagnostic.file) {
                    let { line, character } = ts.getLineAndCharacterOfPosition(
                      diagnostic.file,
                      diagnostic.start
                    );
                    let message = ts.flattenDiagnosticMessageText(
                      diagnostic.messageText,
                      "\n"
                    );
                    console.log(
                      `${diagnostic.file.fileName} (${line + 1},${
                        character + 1
                      }): ${message}`
                    );
                  } else {
                    console.log(
                      ts.flattenDiagnosticMessageText(
                        diagnostic.messageText,
                        "\n"
                      )
                    );
                  }
                });
                if (emitResult.emitSkipped) {
                  throw new Error(
                    `[Error] ${args.path}: TypeScript compilation failed - see above errors`
                  );
                } else {
                  return {
                    contents: fileContents,
                    loader: "ts",
                  };
                }
              });
              build.onEnd((result) => {
                if (result.errors.length === 0) {
                  for (const outFile of result.outputFiles) {
                    fs.mkdirSync(path.dirname(outFile.path), {
                      recursive: true,
                    });
                    fs.writeFileSync(
                      outFile.path,
                      outFile.text.replace(/\s*var globalThis;\s*/g, ""),
                      "utf8"
                    );
                  }
                  console.log(`[Success] ${filePath}`);
                } else {
                  console.error(`[Error] ${filePath} - see above errors`);
                }
                if (result.warnings.length > 0) {
                  console.warn(`[Warn] ${filePath}`, result.warnings);
                }
              });
            },
          },
        ],
      });

      await ctx.watch();

      esbuildWatchers.set(filePath, ctx);
    }
  });

  watcher.on("unlink", async (filePath) => {
    if (esbuildWatchers.has(filePath)) {
      console.log("[Close watcher]", filePath);
      await esbuildWatchers.get(filePath).dispose();
      esbuildWatchers.delete(filePath);
    }

    fs.rmSync(getBuildPath(filePath), {
      recursive: true,
      force: true,
    });
  });
}

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
});
rl.question(
  `Build directory "${buildDir}" will be deleted before continuing - ok?\n(Y/n) `,
  (answer) => {
    if (answer.toLowerCase() === "y") {
      main();
    } else {
      process.exit(-1);
    }
  }
);
