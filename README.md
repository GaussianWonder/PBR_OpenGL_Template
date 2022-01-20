# OpenGL Template

This is an OpengGL project start template in c++.

## Build

The following commands will be displayed using an alias:

```bash
alias run="./run.sh"
alias log="./log_run.sh"
```

Both **run** and **log** accept the same arguments, that of the command to **run**. Both commands will log runtime information, however log will also log the whole console output to a file.

### Help menu

```bash
run help
```

```text
Example:
  run clean build opengl_template
  run opengl_template
  run build
  run clean build
  run clean
  run conan

  :help   displays this message
  :clean  clean build folder
  :build  build project
  :exec   execute the executable
  :cb     clean build shorthand
  :conan  same as build
  :dependencies  conan install dependencies
```

The run script will take its arguments and execute them sequentially.

Custom commands can be provided as stringified paths to the run command.

Since this is a simple script, i recommend to read it for further customisation and understanding of the build process.

### Get up and running

```bash
run clean dependencies build execute
```

### Fast rebuild and run

```bash
run build execute
```

## Recommendations

If using VSCode, I recommend the following settings for **command-runner**.

```json
"command-runner.terminal.autoClear": true,
"command-runner.terminal.autoFocus": true,
"command-runner.commands": {
    "install": "./run.sh clean dependencies",
    "run": "./run.sh conan execute",
    "build": "./run.sh conan",
    "log": "./log_run.sh clean dependencies conan",
    "log run": "./log_run.sh conan execute"
}
```

Now useful commands are accesible via the `CTRL + SHIFT + R` shortcut.
