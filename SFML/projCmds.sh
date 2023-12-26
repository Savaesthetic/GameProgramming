#!/bin/bash

# Cd into project path so script runs as if it was called from project directory
cd "$PROJECT_PATH" # PROJECT_PATH defined in envSetup

# Path for the build directory
BUILD_PATH="./build/"

function config()
{
   # Create the build directory
   if [ ! -d "$BUILD_PATH" ]; then
      mkdir "$BUILD_PATH"
   fi

   # Configure cmake
   if [ -d "$BUILD_PATH" ]; then
      cmake -S "$PROJECT_PATH" -B "$BUILD_PATH"

      # Move the necessary font and config file into the build src dir
      cp "${PROJECT_PATH}/resources/config/config.txt" "${BUILD_PATH}/src/"
      cp "${PROJECT_PATH}/resources/fonts/FreeMono.ttf" "${BUILD_PATH}/src/"
   else
      echo "Error: ${BUILD_PATH} not found. Can not continue with cmake."
      exit 1
   fi
}

while getopts "bcdr" option; do
   case $option in
      b) # Build the executable by executing the make file
         cmake --build "$BUILD_PATH"
         exit;;
      c) # Configure the environment
         config
         exit;;
      d) # Clean the testDir by deleting ./build
         if [ -d "$BUILD_PATH" ]; then
            rm -rf "$BUILD_PATH"
         fi
         exit;;
      r) # Run the build executable
         cd "${BUILD_PATH}/src/"
         "./${PROJECT_NAME}"
         exit;;
   esac
done