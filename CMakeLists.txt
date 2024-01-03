cmake_minimum_required(VERSION 3.7.0)
project(TShell VERSION 1.0.0 LANGUAGES CXX)

# Add main.cpp file of the project root directory as a source file
set(SOURCE_DIR src)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(TShell "src/main.cpp")
set(CMAKE_CXX_STANDARD 17)
include_directories("src" "src/include" )
