cmake_minimum_required(VERSION 3.15)
project( $ENV{PROJECT_NAME}
  LANGUAGES CXX
  VERSION 1.0
)

add_subdirectory(dependencies)
add_subdirectory(src)
