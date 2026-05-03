cmake_minimum_required(VERSION 3.16)
project(xmc4800_ecat_host LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(ecat_host STATIC
  src/ethercat.cpp
  src/raw_socket.cpp
)
target_include_directories(ecat_host PUBLIC include)
target_compile_options(ecat_host PRIVATE -Wall -Wextra -Wpedantic)

add_executable(ecat_scan_xmc4800 src/main.cpp)
target_link_libraries(ecat_scan_xmc4800 PRIVATE ecat_host)

target_compile_options(ecat_scan_xmc4800 PRIVATE -Wall -Wextra -Wpedantic)
