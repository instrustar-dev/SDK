cmake_minimum_required(VERSION 3.16)

project(DllTestQt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 COMPONENTS Core Quick QuickControls2 Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick QuickControls2 Widgets REQUIRED)

#查询系统
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    string(FIND ${CMAKE_CXX_COMPILER} "arm-linux-gnueabi-g++" IS_LINUX_ARM_EABI)
    string(FIND ${CMAKE_CXX_COMPILER} "arm-linux-gnueabihf-g++" IS_LINUX_ARM_EABIHF)
    if(IS_LINUX_ARM_EABI GREATER -1)
        set (PLAT_DIR "arm-linux-gnueabi")
        set (LIB_USB_DIR "/usr/libusb_gnueabi/libusb")
    elseif(IS_LINUX_ARM_EABIHF GREATER -1)
        set (PLAT_DIR "arm-linux-gnueabihf")
        set (LIB_USB_DIR "/usr/libusb_gnueabihf/libusb")
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
message(">>> PLAT= ${CMAKE_SYSTEM_NAME} DIR=${PLAT_DIR}  ${CMAKE_EXE_LINKER_FLAGS}")

set (SHARED_LIB_SUBDIR ../../SharedLibrary/${CMAKE_SYSTEM_NAME}/${PLAT_DIR})

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
add_definitions(-D_WINUSB)
set (LIB_USB "winusb")
else()
set (LIB_USB "usb-1.0")
endif ()

#head file path，头文件目录 ${CMAKE_SOURCE_DIR}
INCLUDE_DIRECTORIES(
            ./
            ./plot
            ../../SharedLibrary
)
#get_property(include_dirs DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#message(">>> include_dirs=${include_dirs}") #打印一下目录情况

#link directories
#link_directories( ${SHARED_LIB_SUBDIR}/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>)
link_directories( ${SHARED_LIB_SUBDIR}/Release)
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    link_directories(  ${LIB_USB_DIR} )
else()
    link_directories( ${LIB_USB_DIR}/lib )
endif ()

# collect sources and other files
file(GLOB_RECURSE ALL_SOURCES "./*.cpp", "./plot/*.cpp")
file(GLOB_RECURSE ALL_INCLUDE "./*.h", "./plot/*.h")
#file(GLOB_RECURSE UI "src/*.ui")
#file(GLOB_RECURSE QRC "res/*.qrc")

# 对inc src的源码分组到组里
#vs
source_group(inc FILES ${ALL_INCLUDE})
source_group(src FILES ${ALL_SOURCES})
#qt
add_custom_target(files SOURCES ${ALL_INCLUDE} ${ALL_SOURCES})

# Resources:
#set(SOURCES main.cpp)
qt_add_resources(ALL_SOURCES vmmso.qrc)

#qt_add_resources(my_app "app_images"
#    PREFIX "/"
#    FILES
#        images/copy.png
#        images/cut.png
#)

qt_add_executable(${PROJECT_NAME} ${ALL_INCLUDE} ${ALL_SOURCES})

qt_add_qml_module(${PROJECT_NAME}
    URI VMMSO
    VERSION 1.0
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER www.vimu.top
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::QuickControls2
    Qt${QT_VERSION_MAJOR}::Widgets vdso)

#copy dll
#message(">>> CMAKE_BINARY_DIR= ${CMAKE_BINARY_DIR}")
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy "${SHARED_LIB_SUBDIR}/${CMAKE_BUILD_TYPE}/vdso.dll"  "${CMAKE_BINARY_DIR}")
else()
#    execute_process(COMMAND ${CMAKE_COMMAND} -E copy "${SHARED_LIB_SUBDIR}/Release/libvdso.so"  "${CMAKE_BINARY_DIR}")
#    execute_process(COMMAND ${CMAKE_COMMAND} -E copy "${SHARED_LIB_SUBDIR}/Release/libvdso.so.1.0"  "${CMAKE_BINARY_DIR}")
endif ()
