# OpenGL Template

This is an OpengGL project template written c++.

It features:

- Model loading
  - Mesh handling
  - Texture handling
- Skybox loading
- Shader loading
  - Basic shader program
  - +Geometry shader
  - Clunky uniform updaters
- Skybox loading
- Logger
- Clunky opengl error handler
- Controllable Widgets
- PBR
  - Metallic texture
  - Roughness texture
  - Normal Map
  - Albedo
  - HDR tone mapping
  - Gamma correction
- Controllable Lights

TODO:

1. Add **hdri** support
2. Add **IBL** support
3. Add **specular map** to the PBR shader
4. **Multiply TBN** with light variables in the geometry instead of fragment
5. Include higher resolution textures

## Build

At the time of writing this, the conan version used is `Conan version 1.44.0` with experimental features enabled.

The following aliases are used in this document:

```bash
alias run="./run.sh"
alias log="./log_run.sh"
```

Both **run** and **log** accept the same arguments, that of the command to **run**. Both commands will log runtime information, however `log.sh` will also log the whole building process.

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
  "install": "./run.sh dependencies",
  "build": "./run.sh conan",
  "run": "./run.sh conan execute",
  "clean": "./run.sh clean",
  "build run": "./run.sh dependencies conan execute",
  "log build": "./log_run.sh dependencies conan",
  "log run": "./log_run.sh conan execute",
  "log build run": "./log_run.sh dependencies conan execute"
}
```

Now useful commands are accesible via the `CTRL + SHIFT + R` shortcut.
