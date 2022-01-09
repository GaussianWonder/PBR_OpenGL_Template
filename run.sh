#!/bin/bash

print_help () {
  echo "Example:"
  echo "  run clean build opengl_template"
  echo "  run opengl_template"
  echo "  run build"
  echo "  run clean build"
  echo "  run clean"
  echo ""
  echo "  :help   displays this message"
  echo "  :clean  clean build folder"
  echo "  :build  build project"
  echo "  :exec   execute the executable"
  echo "  :cb     clean build shorthand"
  echo ""
}

pwd=$(pwd)

if [ $# -eq 0 ]; then
  echo "No arguments provided"
  print_help
  exit 1
fi

is_help=false

cmds=()

for arg in "$@"
do
  case $arg in

    "help")
      is_help=true
      ;;

    "clean")
      cmds+=("./scripts/clean.sh")
      ;;

    "build")
      cmds+=("./scripts/build.sh")
      ;;

    "execute")
      # TODO dynamic project executable select
      cmds+=("./build/opengl_template")
      ;;

    "cb")
      cmds+=("./scripts/clean_build.sh")
      ;;

    *)
      # Support for custom commands via script path
      cmds+=("$arg")
      ;;
  esac
done

if [ "$is_help" = true ]; then
  print_help
  exit 0
fi

echo "Executing ${#cmds[@]} commands"
echo ""

for cmd in "${cmds[@]}"
do
  echo "Doing $cmd"
  time $cmd
  echo ""
done

cd $pwd