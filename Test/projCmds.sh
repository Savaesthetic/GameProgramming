#!/bin/bash

# Cd into project path so script runs as if it was called from project directory
cd "$PROJECT_PATH" # PROJECT_PATH defined in envSetup

# Path for the build directory
BUILD_PATH="./build/"
# Path for the src directory
SRC_PATH="./src/"

function config()
{
   # Create the build directory
   if [ ! -d "$BUILD_PATH" ]; then
      mkdir "$BUILD_PATH"
   fi

   # Configure cmake
   if [ -d "$BUILD_PATH" ]; then
      cmake -S "$SRC_PATH" -B "$BUILD_PATH"
   else
      echo "Error: ${BUILD_PATH} not found. Can not continue with cmake."
      exit 1
   fi
}

while getopts "bcdr" option; do
   case $option in
      b) # Build the executable by executing the make file
         make --directory "$BUILD_PATH"
         exit;;
      c) # Configure the environment
         config
         exit;;
      d) # Clean the testDir by deleting ./build
         if [ -d "$BUILD_PATH" ]; then
            rm -r ./build
         fi
         exit;;
      r) # Run the build executable
         "./${BUILD_PATH}/${PROJECT_NAME}.exe"
         exit;;
   esac
done