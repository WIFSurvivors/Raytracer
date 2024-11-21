#! /bin/bash
SCRIPT_DIR=$(dirname "$(realpath "$0")")
BUILD_DIR="$SCRIPT_DIR/build"


compile(){
  echo "Compiling binaries"
  mkdir -p $BUILD_DIR
  cd $BUILD_DIR || exit
  cmake ..
  make
  echo "Compilation finished"
  ./RayTracer
}


if [[ "$1" == "--recompile" ]]; then
  compile
  exit
elif [[ "$1" == "--cleancompile" ]]; then
  echo "Clean compiling..."
  if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR" || exit
    echo "Removed existing build directory"
  fi
  compile
  exit
elif [[ "$1" == "--clean" ]]; then
  echo "Cleaning generated directories..."
  if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR" || exit
    echo "Removed existing build directory"
  fi
  exit
else
  echo "--recompile or --cleancompile"
fi
