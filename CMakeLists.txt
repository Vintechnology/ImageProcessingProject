cmake_minimum_required(VERSION 2.8)
project(impr)
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

set(INCLUDE_DIRS "")
foreach (_headerFile ${HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list(APPEND INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

set(ADDED_ITEMS ${SOURCES} ${HEADERS})

include_directories(${INCLUDE_DIRS})
add_executable(impr ${ADDED_ITEMS})
