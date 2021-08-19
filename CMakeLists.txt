# Basic parameters; check that these match your project / environment
cmake_minimum_required(VERSION 3.8)

# Temporary check to stop it breaking if building normally with master branch of sdk
if(EXISTS ${32BLIT_DIR}/32blit-pico/sdk_import.cmake)
	include(${32BLIT_DIR}/32blit-pico/sdk_import.cmake)
endif()

project(Super-Square-Bros)

set(PROJECT_SOURCE SuperSquareBros.cpp)
set(PROJECT_DISTRIBS LICENSE README.md)

if(PICO_SDK_PATH)
	set(PROJECT_SOURCE ${PROJECT_SOURCE} PicoAudio.cpp)
else()
	set(PROJECT_SOURCE ${PROJECT_SOURCE} Audio.cpp)
endif()

# Build configuration; approach this with caution!
if(MSVC)
  add_compile_options("/W4" "/wd4244" "/wd4324")
else()
  add_compile_options("-Wall" "-Wextra" "-Wdouble-promotion")
endif()
find_package (32BLIT CONFIG REQUIRED PATHS ../32blit-sdk)

blit_executable (${PROJECT_NAME} ${PROJECT_SOURCE})
blit_assets_yaml (${PROJECT_NAME} assets.yml)
blit_metadata (${PROJECT_NAME} metadata.yml)
target_compile_definitions(${PROJECT_NAME} PRIVATE ALLOW_HIRES=0)
add_custom_target (flash DEPENDS ${PROJECT_NAME}.flash)

# setup release packages
install (FILES ${PROJECT_DISTRIBS} DESTINATION .)
set (CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set (CPACK_GENERATOR "ZIP" "TGZ")
include (CPack)
