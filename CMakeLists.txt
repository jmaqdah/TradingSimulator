# 1. Set the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# 2. Name the project
project(TradingSimulator)

# 3. Set the C++ standard (C++17 or later for modern features)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 4. Add the executable and specify the source files
add_executable(TradingSimulator src/main.cpp)
