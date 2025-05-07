
# 指定 CMake 的最小版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(MySDL2Project VERSION 1.0)

# 查找 SDL2 库
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# 包含 SDL2 头文件
include_directories(${SDL2_INCLUDE_DIRS} SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

# 添加一个可执行文件
add_executable(game ./src/main.c)

# 链接 SDL2 库
target_link_libraries(game ${SDL2_LIBRARIES} SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)
