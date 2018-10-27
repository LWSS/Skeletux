cmake_minimum_required(VERSION 2.8)
project(skeletux)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.0)
        message(FATAL_ERROR "Your GCC seems outdated. ( Requires >=6.0 )")
    endif()
else()
    message(FATAL_ERROR "You must have GCC >= 5.2 installed in order to compile")
endif()

FUNCTION (EXCLUDE_FILES_FROM_DIR_IN_LIST _InFileList _excludeDirName _verbose)
    foreach (ITR ${_InFileList})
        if ("${_verbose}")
            message(STATUS "ITR=${ITR}")
        endif ("${_verbose}")

        if ("${ITR}" MATCHES "(.*)${_excludeDirName}(.*)") # Check if the item matches the directory name in _excludeDirName
            message(STATUS "Remove Item from List:${ITR}")
            list (REMOVE_ITEM _InFileList ${ITR}) # Remove the item from the list
        endif ("${ITR}" MATCHES "(.*)${_excludeDirName}(.*)")

    endforeach(ITR)
    set(SOURCE_FILES ${_InFileList} PARENT_SCOPE) # Return the SOURCE_FILES variable to the calling parent
ENDFUNCTION (EXCLUDE_FILES_FROM_DIR_IN_LIST)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3 -g -std=c++17 -Wall -ldl -fpic -shared --no-gnu-unique")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -std=c++17 -Wall -ldl -fpic -shared --no-gnu-unique")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE RELEASE)
endif(NOT CMAKE_BUILD_TYPE)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
EXCLUDE_FILES_FROM_DIR_IN_LIST("${SOURCE_FILES}" "${CMAKE_SOURCE_DIR}/src/ImGUI/examples/" FALSE)
EXCLUDE_FILES_FROM_DIR_IN_LIST("${SOURCE_FILES}" "${CMAKE_SOURCE_DIR}/src/ImGUI/extra_fonts" FALSE)

include_directories(${INCLUDE_DIRS})

add_library(skeletux SHARED ${SOURCE_FILES})
target_link_libraries(skeletux GL dl)
