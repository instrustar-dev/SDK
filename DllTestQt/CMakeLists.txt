cmake_minimum_required(VERSION 3.16)

project(DllTestQt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Widgets REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Widgets REQUIRED)
if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS QuickControls2 REQUIRED)
endif()

#查询系统
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    string(FIND ${CMAKE_CXX_COMPILER} "arm-linux-gnueabi-g++" IS_LINUX_ARM_EABI)
    string(FIND ${CMAKE_CXX_COMPILER} "arm-linux-gnueabihf-g++" IS_LINUX_ARM_EABIHF)
    string(FIND ${CMAKE_CXX_COMPILER} "aarch64-linux" IS_LINUX_AARCH64)
    if(IS_LINUX_ARM_EABI GREATER -1)
        set (PLAT_DIR "arm-linux-gnueabi")
    elseif(IS_LINUX_ARM_EABIHF GREATER -1)
        set (PLAT_DIR "arm-linux-gnueabihf")
    elseif(IS_LINUX_AARCH64 GREATER -1)
        set (PLAT_DIR "aarch64-linux")
    else()
        set (PLAT_DIR "X64")
    endif()
elseif(CMAKE_EXE_LINKER_FLAGS MATCHES "/machine:ARM64")
    set (PLAT_DIR "AMR64")
elseif(CMAKE_EXE_LINKER_FLAGS MATCHES "/machine:X86")
    set (PLAT_DIR "X86")
else()
    set (PLAT_DIR "X64")
endif()
message(">>> PLAT= ${CMAKE_SYSTEM_NAME} DIR=${PLAT_DIR}  ${CMAKE_EXE_LINKER_FLAGS}")

set (SHARED_LIB_SUBDIR ../SharedLibrary/${CMAKE_SYSTEM_NAME}/${PLAT_DIR})

#head file path，头文件目录 ${CMAKE_SOURCE_DIR}
INCLUDE_DIRECTORIES(
            ./
            ./plot
            ../SharedLibrary
)
#get_property(include_dirs DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#message(">>> include_dirs=${include_dirs}") #打印一下目录情况

#link directories
link_directories( ${SHARED_LIB_SUBDIR}/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>)
#link_directories( ${SHARED_LIB_SUBDIR}/Release)

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

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${ALL_INCLUDE}
        ${ALL_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET qt5test APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${ALL_INCLUDE}
            ${ALL_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
          ${ALL_INCLUDE}
          ${ALL_SOURCES}
        )
    endif()
endif()


if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(${PROJECT_NAME})
    qt_finalize_executable(${PROJECT_NAME})
endif()


#qt_add_qml_module(${PROJECT_NAME}
#    URI VMMSO
#   VERSION 1.0
#)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER www.vimu.top
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::QuickControls2
        Qt${QT_VERSION_MAJOR}::Widgets vdso)
else()
    target_link_libraries(${PROJECT_NAME}
        PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::Widgets vdso)
endif()


#copy dll
#message(">>> CMAKE_BINARY_DIR= ${CMAKE_BINARY_DIR}")
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy "${SHARED_LIB_SUBDIR}/${CMAKE_BUILD_TYPE}/vdso.dll"  "${CMAKE_BINARY_DIR}")
else()
#    execute_process(COMMAND ${CMAKE_COMMAND} -E copy "${SHARED_LIB_SUBDIR}/Release/libvdso.so"  "${CMAKE_BINARY_DIR}")
#    execute_process(COMMAND ${CMAKE_COMMAND} -E copy "${SHARED_LIB_SUBDIR}/Release/libvdso.so.1.0"  "${CMAKE_BINARY_DIR}")
endif ()
