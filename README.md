# OpenGL Template

This is an OpengGL project start template in c++.

## Build

The following commands will be displayed using an alias:

```bash
alias run="./run.sh"
alias log="./log_run.sh"
```

Both **run** and **log** accept the same arguments, that of the command to **run**. Both commands will log runtime information, however log will also log the whole console output to a file.

## Help menu

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

  :help   displays this message
  :clean  clean build folder
  :build  build project
  :exec   execute the executable
  :cb     clean build shorthand
```

The run script will take its arguments and execute them sequentially.

There are only 5 **presetted commands**, but custom commands can be provided by path.

Since this is a simple script, i recommend to read it for further customisation and understanding of the build process.

## Recommendations

If using VSCode, I recommend the following settings for **command-runner**.

```json
"command-runner.terminal.name": "runCommand",
"command-runner.terminal.autoClear": true,
"command-runner.terminal.autoFocus": true,
"command-runner.commands": {
    "run": "./run.sh build execute",
    "build": "./run.sh clean build",
    "log": "./log_run.sh clean build",
    "log run": "./log_run.sh clean build execute"
}
```
