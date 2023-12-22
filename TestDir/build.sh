#!/bin/bash

# Get the path to the directory the shell script lives in
PARENT_PATH=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
# Path for the build directory
BUILD_PATH="./build"

# Cd into shell script location
# This allows the script to be called from whatever directory and run as if it was called form TestDir
cd "$PARENT_PATH"

while getopts "d" option; do
   case $option in
      d) # Clean the testDir by deleting ./build
         rm -r ./build
         exit;;
   esac
done

# Build the test project
mkdir "$BUILD_PATH"

# Configure cmake
if [ -d "$BUILD_PATH" ]; then
  cmake -S "." -B "$BUILD_PATH"
else
  echo "Error: ${BUILD_PATH} not found. Can not continue with cmake."
  exit 1
fi

#Run the cmake thing