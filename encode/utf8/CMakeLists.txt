# Marcos
# Group sources
macro(source_group_by_dir SOURCE_FILES)
    set(SGBD_CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    foreach(SGBD_FILE ${${SOURCE_FILES}})
        string(REGEX REPLACE ${SGBD_CUR_DIR}/\(.*\) \\1 SGBD_FILE_PATH ${SGBD_FILE})
        string(REGEX REPLACE "\(.*\)/.*" \\1 SGBD_GROUP_NAME ${SGBD_FILE_PATH})
        string(COMPARE EQUAL ${SGBD_FILE_PATH} ${SGBD_GROUP_NAME} SGBD_NOGROUP)
        string(REPLACE "/" "\\" SGBD_GROUP_NAME ${SGBD_GROUP_NAME})
        if(SGBD_NOGROUP)
            set(SGBD_GROUP_NAME "\\")
        endif()
        source_group(${SGBD_GROUP_NAME} FILES ${SGBD_FILE})
    endforeach()
    unset(SGBD_CUR_DIR)
    unset(SGBD_FILE)
    unset(SGBD_FILE_PATH)
    unset(SGBD_GROUP_NAME)
    unset(SGBD_NOGROUP)
endmacro()

# Make directory
macro(make_directory DIR)
    if(EXISTS ${DIR})
        file(REMOVE_RECURSE ${DIR})
    endif()
    file(MAKE_DIRECTORY ${DIR})
endmacro()

# Cmake version requirements define
cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

# Define project
project(utf8
        LANGUAGES CXX)

# Set language version
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Check basic project properties
if (NOT CMAKE_GENERATOR_PLATFORM)
    set(CMAKE_GENERATOR_PLATFORM x64)
endif()
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
add_definitions("-DUNICODE" "-D_UNICODE")

# Set output directory
string(TOLOWER ${CMAKE_SYSTEM_NAME} CMAKE_SYSTEM_NAME_STR)
string(TOLOWER ${CMAKE_GENERATOR_PLATFORM} CMAKE_GENERATOR_PLATFORM_STR)
set(OUTPUT_BIN_DIR_DEBUG ${PROJECT_SOURCE_DIR}/output/${CMAKE_SYSTEM_NAME_STR}/${CMAKE_GENERATOR_PLATFORM_STR}/debug/bin)
make_directory(${OUTPUT_BIN_DIR_DEBUG})
set(OUTPUT_BIN_DIR_RELEASE ${PROJECT_SOURCE_DIR}/output/${CMAKE_SYSTEM_NAME_STR}/${CMAKE_GENERATOR_PLATFORM_STR}/release/bin)
make_directory(${OUTPUT_BIN_DIR_RELEASE})

# Create executable
add_executable(${PROJECT_NAME})

# Set properties
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/output/${CMAKE_SYSTEM_NAME_STR}/${CMAKE_GENERATOR_PLATFORM_STR}/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>/bin
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    CXX_VISIBILITY_PRESET hidden
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_BIN_DIR_DEBUG}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_BIN_DIR_RELEASE}
)

# Collect all source files
file(GLOB_RECURSE SRC_LIST
    ${PROJECT_SOURCE_DIR}/include/*.h
    ${PROJECT_SOURCE_DIR}/src/*.c   
    ${PROJECT_SOURCE_DIR}/src/*.cpp)
source_group_by_dir(SRC_LIST)

# Set sources
target_sources(${PROJECT_NAME} PRIVATE
               ${SRC_LIST})

# Set include directories
target_include_directories(${PROJECT_NAME} PRIVATE
                           ${PROJECT_SOURCE_DIR}/include)