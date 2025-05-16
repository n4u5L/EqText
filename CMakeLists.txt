cmake_minimum_required(VERSION 3.12)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(Qt5_DIR "C:\\Users\\dwcre\\MyCode\\bin\\Qt_5.12.12\\5.12.12\\msvc2017_64\\lib\\cmake\\Qt5")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_compile_definitions(NOMINMAX _USE_MATH_DEFINES)
add_compile_options(-Zc:preprocessor -utf-8 -EHsc -bigobj)

project(main)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets REQUIRED)

file(GLOB sources *.cpp)
add_executable(main ${sources})
target_link_libraries(main PRIVATE Qt5::Core Qt5::Widgets)