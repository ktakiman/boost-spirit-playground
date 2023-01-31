cmake_minimum_required(VERSION 3.14)

project(SpiritTest
  LANGUAGES CXX  
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_executable(test)

target_sources(test
  PRIVATE
  main.cpp
  helper.cpp
)

target_include_directories(test
  PRIVATE
  /home/kei/dev/cpp/boost_1_81_0/
)


