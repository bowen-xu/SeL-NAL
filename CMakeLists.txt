add_subdirectory(Bodhi/cNARS)

# set cmake version
cmake_minimum_required(VERSION 3.25)

# Exclude all targets of cNARS except for narsese
get_property(CNARS_TARGETS DIRECTORY Bodhi/cNARS PROPERTY BUILDSYSTEM_TARGETS)

foreach(target IN LISTS CNARS_TARGETS)
    if(NOT ${target} STREQUAL "narsese")
        set_target_properties(${target} PROPERTIES EXCLUDE_FROM_ALL ON)
    endif()
endforeach()

# C++ settings
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_SH "CMAKE_SH-NOTFOUND")
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
# target_compile_definitions(narsese PRIVATE DEBUG)

project(BODHI)

# add_subdirectory(Bodhi/cNARS/pybind11)

find_package(fmt)

# Add Boost header files and Python header files
include_directories(
    ${Boost_INCLUDE_DIRS}
)

file(GLOB SOURCES_BODHI
    Bodhi/*.cpp

    # Bodhi/Representation/src/*.cpp
    # Bodhi/Representation/*.cpp
    # Bodhi/Models/SequentialGroup/src/*.cpp
    # Bodhi/Models/SpatialGroup/src/*.cpp
    # /Bodhi/functions/src/*.cpp
)
include_directories(
    "${PROJECT_SOURCE_DIR}/Bodhi"

    # "${PROJECT_SOURCE_DIR}/Bodhi/Models/SequentialGroup/include"
    # "${PROJECT_SOURCE_DIR}/Bodhi/Models/SpatialGroup/include"
    # "${PROJECT_SOURCE_DIR}/Bodhi/functions/include"
    "Bodhi/cNARS"
    ${pybind11_INCLUDE_DIR}
    ${pybind11_INCLUDE_DIRS}
)

# message(SOURCES_NARSESE=${SOURCES_NARSESE})
# pybind11_add_module(Foo ${SOURCES_BODHI} ${SOURCES_NARSESE} ${SOURCES_UTILS} ${SOURCES_PARSER} ${SOURCES_CONFIG} ${SOURCES_INTERPRETER})
file(GLOB SOURCES_SEQUENTIAL_GROUP
    Bodhi/Models/SequentialGroup/src/*.cpp
)

# file(GLOB SOURCES_NARSESE Bodhi/cNARS/Narsese/src/*.cpp)
# file(GLOB SOURCES_PARSER Bodhi/cNARS/Narsese/Parser/*.cpp)
# file(GLOB SOURCES_INTERPRETER Bodhi/cNARS/Interpreter/src/*.cpp)
# file(GLOB SOURCES_UTILS Bodhi/cNARS/utils/*.cpp)
# file(GLOB SOURCES_CONFIG Bodhi/cNARS/Config.cpp Bodhi/cNARS/Global.cpp)
# set(SOURCES_NARSESE ${SOURCES_NARSESE} CACHE INTERNAL "Narsese source files")
# set(SOURCES_PARSER ${SOURCES_PARSER} CACHE INTERNAL "Parser source files")
# set(SOURCES_INTERPRETER ${SOURCES_INTERPRETER} CACHE INTERNAL "Interpreter source files")
# set(SOURCES_UTILS ${SOURCES_UTILS} CACHE INTERNAL "Utils source files")
# set(SOURCES_CONFIG ${SOURCES_CONFIG} CACHE INTERNAL "Config source files")

# pybind11_add_module(SequentialGroup ${SOURCES_SEQUENTIAL_GROUP})
pybind11_add_module(SequentialGroup ${SOURCES_SEQUENTIAL_GROUP} Bodhi/cNARS/Narsese.cpp ${SOURCES_NARSESE} ${SOURCES_UTILS} ${SOURCES_PARSER} ${SOURCES_CONFIG} ${SOURCES_INTERPRETER})

# Python_add_library(SequentialGroup ${SOURCES_SEQUENTIAL_GROUP})
# target_link_libraries(SequentialGroup pybind11::headers)
# set_target_properties(SequentialGroup PROPERTIES
# INTERPROCEDURAL_OPTIMIZATION ON
# CXX_VISIBILITY_PRESET ON
# VISIBILITY_INLINES_HIDDEN ON)

# add_library(SequentialGroup MODULE ${SOURCES_SEQUENTIAL_GROUP} ${SOURCES_NARSESE} ${SOURCES_UTILS} ${SOURCES_PARSER} ${SOURCES_CONFIG} ${SOURCES_INTERPRETER})
# target_link_libraries(SequentialGroup PUBLIC
# pybind11::module
# # narsese
# )
target_include_directories(SequentialGroup PUBLIC
    "Bodhi/Models"
    "Bodhi/cNARS"
    "Bodhi/cNARS/Narsese"
    "Bodhi/cNARS/Narsese/Parser"
    "Bodhi/cNARS/Interpreter"
    "Bodhi/cNARS/utils"
)
find_package(Boost 1.45.0 COMPONENTS spirit)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(SequentialGroup ${Boost_LIBRARIES})
target_link_libraries(SequentialGroup PRIVATE fmt::fmt)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/pyBodhi)

# message("================")
set(PACKAGE_NAME "pyBodhi")

# # libraries
set(BODHI_PACKAGE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE_NAME}")
file(WRITE "${BODHI_PACKAGE_DIR}/__init__.py" "")

function(add_module MODULE_NAME)
    if(ARGC GREATER_EQUAL 2)
        list(GET ${ARGV} 2 MODULE_SOURCE)
    endif()

    message("-------- add module --------")
    message("name: ${MODULE_NAME}")

    if(DEFINED ${MODULE_SOURCE})
        message("path: ${MODULE_SOURCE}")
        set(BODHI_MODULE "BODHI_${MODULE_NAME}")
    else()
        set(BODHI_MODULE "${MODULE_NAME}")
    endif()

    message(${BODHI_MODULE})

    if(DEFINED MODULE_SOURCE)
        add_library(${BODHI_MODULE} MODULE ${MODULE_SOURCE})
        target_link_libraries(${BODHI_MODULE} BODHI)
    endif()

    set_target_properties(${BODHI_MODULE} PROPERTIES PREFIX "")
    set_target_properties(${BODHI_MODULE} PROPERTIES OUTPUT_NAME "${MODULE_NAME}")

    # set_target_properties(${BODHI_MODULE} PROPERTIES SUFFIX ".so")
    set(file_root "${BODHI_PACKAGE_DIR}/${MODULE_NAME}")

    # set(file_root "${BODHI_PACKAGE_DIR}")
    set_target_properties(${BODHI_MODULE} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${file_root})

    file(WRITE "${BODHI_PACKAGE_DIR}/${MODULE_NAME}/__init__.py" "from .${MODULE_NAME} import *\n")
endfunction()

# add_module(Foo)
add_module(SequentialGroup)

add_module(narsese)

# target_link_libraries(Foo PRIVATE fmt::fmt)
# target_include_directories(Foo PRIVATE ${fmt_INCLUDE_DIRS})
target_link_libraries(SequentialGroup PRIVATE fmt::fmt)
target_include_directories(SequentialGroup PRIVATE ${fmt_INCLUDE_DIRS})

# add_module(Models Bodhi/Models/Models.cpp)
message("================")

# generate stub files
message(PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR})
message(PACKAGE_NAME=${PACKAGE_NAME})
add_custom_target(GENERATE_PYI ALL
    COMMENT "Trying to generate .pyi files ..."
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND pwd
    COMMAND python generate_stub.py -p ${PACKAGE_NAME}
)

add_dependencies(SequentialGroup narsese)
add_dependencies(GENERATE_PYI SequentialGroup)

# list(REMOVE_ITEM all_targets GENERATE_PYI)
# add_dependencies(GENERATE_PYI all_targets)