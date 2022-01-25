# CMakeList.txt: DllTest 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project(DllTest LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../SharedLibrary/${CMAKE_SYSTEM_NAME}/X64/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../SharedLibrary/${CMAKE_SYSTEM_NAME}/X64/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../SharedLibrary/${CMAKE_SYSTEM_NAME}/X64/Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../SharedLibrary/${CMAKE_SYSTEM_NAME}/X64/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../SharedLibrary/${CMAKE_SYSTEM_NAME}/X64/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../SharedLibrary/${CMAKE_SYSTEM_NAME}/X64/Release)


#head file path，头文件目录 ${CMAKE_SOURCE_DIR}
INCLUDE_DIRECTORIES(
            ./
            ../SharedLibrary
)

get_property(ALL_INCLUDE DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message(">>> include_dirs=${ALL_INCLUDE}") #打印一下目录情况

#4.source directory，源文件目录
aux_source_directory(./ SOURCE_LIST)

set ( ALL_SOURCES ${SOURCE_LIST}
                    )
message(">>> source_dirs=${ALL_SOURCES}") #打印一下目录情况


# 对inc src的源码分组到组里
#vs
source_group(inc FILES ${ALL_INCLUDE})
source_group(src FILES ${ALL_SOURCES})
#qt
add_custom_target(files SOURCES ${ALL_INCLUDE} ${ALL_SOURCES})

link_directories( ../SharedLibrary/${CMAKE_SYSTEM_NAME}/X64/Release)

# 将源代码添加到此项目的可执行文件。
add_executable (${PROJECT_NAME} ${ALL_INCLUDE} ${ALL_SOURCES})


target_link_libraries(${PROJECT_NAME} vdso)


# TODO: 如有需要，请添加测试并安装目标。
