cmake_minimum_required(VERSION 3.14)

project(SpiritTest
  LANGUAGES CXX  
)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# not sure if this is the best way to override the output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(test)

target_sources(test
  PRIVATE
  main.cpp
  helper.cpp
  basic.cpp
  phoenix.cpp
)

target_include_directories(test
  PRIVATE
  /home/kei/dev/cpp/boost_1_81_0/
)
target_compile_features(test
  PRIVATE
  cxx_std_17
)


