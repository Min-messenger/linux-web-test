cmake_minimum_required(VERSION 3.15)
project(VirtualLinux)

set(CMAKE_CXX_STANDARD 11)

# Настройки для Emscripten
set(CMAKE_EXECUTABLE_SUFFIX ".html")
set(CMAKE_CXX_FLAGS "-s WASM=1 -s USE_WEBGL2=1 -s FULL_ES3=1 -s ALLOW_MEMORY_GROWTH=1")

# Флаги для экспорта функций
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s EXPORTED_FUNCTIONS=['_main','_launchApp','_closeWindow']")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap']")

add_executable(virtual-linux main.cpp) 