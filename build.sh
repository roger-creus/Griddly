#!/bin/bash

cd "$(dirname "$0")"

PLATFORM="x86_64" 
BUILD="Debug"
CONAN_BUILD="missing"

for i in "$@"; do
  case $i in
    -p=*|--platform=*)
      PLATFORM="${i#*=}"
      shift # past argument=value
      ;;
    -b=*|--build-type=*)
      BUILD="${i#*=}"
      shift # past argument=value
      ;;
    -f|--force-rebuild)
      CONAN_BUILD="all"
      shift # past argument=value
      ;;
  esac
done

echo $BUILD
echo $CONAN_BUILD
echo $PLATFORM

if [[ $PLATFORM == "WASM" ]]
then
  conan install deps/wasm/conanfile_wasm.txt --profile:host deps/wasm/emscripten.profile --profile:build default -s build_type=$BUILD --build missing -if build_wasm
  cmake . -B build_wasm -GNinja -DWASM=ON -DCMAKE_BUILD_TYPE=$BUILD -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
  cmake --build build_wasm --config $BUILD
else
  conan install deps/conanfile.txt --profile default --profile deps/build.profile -s build_type=$BUILD --build $CONAN_BUILD -if build
  cmake . -B build -GNinja -DCMAKE_BUILD_TYPE=$BUILD -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
  cmake --build build --config $BUILD
fi

