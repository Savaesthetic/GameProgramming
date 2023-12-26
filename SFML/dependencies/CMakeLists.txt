include(FetchContent)

# SFML
FetchContent_Declare(
  sfml
  URL https://github.com/SFML/SFML/archive/refs/tags/2.5.1.zip
  URL_MD5 2c4438b3e5b2d81a6e626ecf72bf75be
)
add_subdirectory(sfml)

# Dear ImGui
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG b81bd7ed984ce095c20a059dd0f4d527e006998f # 1.90.0
)

FetchContent_MakeAvailable(imgui)

# ImGui-SFML
FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY https://github.com/SFML/imgui-sfml
  GIT_TAG 2a4b9dd9b34df61d91e33667dbaf5902c4923867 # 2.6.x
)
add_subdirectory(imgui-sfml)
