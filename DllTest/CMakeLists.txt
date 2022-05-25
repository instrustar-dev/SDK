# CMakeList.txt: DllTest 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project(DllTest LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#查询系统
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    string(FIND ${CMAKE_CXX_COMPILER} "arm-linux-gnueabi-g++" IS_LINUX_ARM_EABI)
    string(FIND ${CMAKE_CXX_COMPILER} "arm-linux-gnueabihf-g++" IS_LINUX_ARM_EABIHF)
    string(FIND ${CMAKE_CXX_COMPILER} "aarch64-linux" IS_LINUX_AARCH64)
    if(IS_LINUX_ARM_EABI GREATER -1)
        set (PLAT_DIR "arm-linux-gnueabi")
        set (LIB_USB_DIR "/usr/libusb_gnueabi/libusb")
    elseif(IS_LINUX_ARM_EABIHF GREATER -1)
        set (PLAT_DIR "arm-linux-gnueabihf")
        set (LIB_USB_DIR "/usr/libusb_gnueabihf/libusb")
    elseif(IS_LINUX_AARCH64 GREATER -1)
        set (PLAT_DIR "aarch64-linux")
        set (LIB_USB_DIR "/usr/libusb_aarch64/libusb")
    else()
        set (PLAT_DIR "X64")
        set (LIB_USB_DIR "/usr/libusb_x86_64/libusb")
    endif()
elseif(CMAKE_EXE_LINKER_FLAGS MATCHES "/machine:ARM64")
    set (PLAT_DIR "AMR64")
elseif(CMAKE_EXE_LINKER_FLAGS MATCHES "/machine:X86")
    set (PLAT_DIR "X86")
else()
    set (PLAT_DIR "X64")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../SharedLibrary/${CMAKE_SYSTEM_NAME}/${PLAT_DIR}/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../SharedLibrary/${CMAKE_SYSTEM_NAME}/${PLAT_DIR}/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../SharedLibrary/${CMAKE_SYSTEM_NAME}/${PLAT_DIR}/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../SharedLibrary/${CMAKE_SYSTEM_NAME}/${PLAT_DIR}/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../SharedLibrary/${CMAKE_SYSTEM_NAME}/${PLAT_DIR}/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../SharedLibrary/${CMAKE_SYSTEM_NAME}/${PLAT_DIR}/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>)


#head file path，头文件目录 ${CMAKE_SOURCE_DIR}
INCLUDE_DIRECTORIES(
            ./
            ../SharedLibrary
)

get_property(ALL_INCLUDE DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message(">>> include_dirs=${ALL_INCLUDE}") #打印一下目录情况

link_directories( ${LIB_USB_DIR}/lib )

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

link_directories( ../SharedLibrary/${CMAKE_SYSTEM_NAME}/${PLAT_DIR}/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>)
if (CMAKE_SYSTEM_NAME MATCHES "Linux") 
    link_directories( ${LIB_USB_DIR}/lib )
endif ()

# 将源代码添加到此项目的可执行文件。
add_executable (${PROJECT_NAME} ${ALL_INCLUDE} ${ALL_SOURCES})

if (CMAKE_SYSTEM_NAME MATCHES "Linux") 
target_link_libraries(${PROJECT_NAME} vdso usb-1.0)
else()
target_link_libraries(${PROJECT_NAME} vdso)
endif ()

# TODO: 如有需要，请添加测试并安装目标。
