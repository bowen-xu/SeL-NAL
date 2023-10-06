add_subdirectory(cNARS)

# set cmake version
cmake_minimum_required(VERSION 3.25)

# Exclude all targets of cNARS except for narsese
get_property(CNARS_TARGETS DIRECTORY cNARS PROPERTY BUILDSYSTEM_TARGETS)

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

project(SLM) # Sequential Learning Model

find_package(fmt)

# Add Boost header files and Python header files
include_directories(
    ${Boost_INCLUDE_DIRS}
)

include_directories(
    "${PROJECT_SOURCE_DIR}/cNARS"
    "${PROJECT_SOURCE_DIR}/cpp"
    ${pybind11_INCLUDE_DIR}
    ${pybind11_INCLUDE_DIRS}
)

# message(SOURCES_NARSESE=${SOURCES_NARSESE})
file(GLOB SOURCES_SEQUENTIAL_GROUP
    cpp/src/*.cpp
)
pybind11_add_module(SequentialGroup ${SOURCES_SEQUENTIAL_GROUP} cNARS/Narsese.cpp ${SOURCES_NARSESE} ${SOURCES_UTILS} ${SOURCES_PARSER} ${SOURCES_CONFIG} ${SOURCES_INTERPRETER})

target_include_directories(SequentialGroup PUBLIC
    "cNARS"
    "cNARS/Narsese"
    "cNARS/Narsese/Parser"
    "cNARS/Interpreter"
    "cNARS/utils"
)
find_package(Boost 1.45.0 COMPONENTS spirit)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(SequentialGroup ${Boost_LIBRARIES})
target_link_libraries(SequentialGroup PRIVATE fmt::fmt)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# message("================")
# libraries
set(SLM_PACKAGE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
file(WRITE "${SLM_PACKAGE_DIR}/__init__.py" "")

function(add_module MODULE_NAME)
    if(ARGC GREATER_EQUAL 2)
        list(GET ${ARGV} 2 MODULE_SOURCE)
    endif()

    message("-------- add module --------")
    message("name: ${MODULE_NAME}")

    if(DEFINED ${MODULE_SOURCE})
        message("path: ${MODULE_SOURCE}")
        set(SLM_MODULE "SLM_${MODULE_NAME}")
    else()
        set(SLM_MODULE "${MODULE_NAME}")
    endif()

    message(${SLM_MODULE})

    if(DEFINED MODULE_SOURCE)
        add_library(${SLM_MODULE} MODULE ${MODULE_SOURCE})
        target_link_libraries(${SLM_MODULE} SLM)
    endif()

    set_target_properties(${SLM_MODULE} PROPERTIES PREFIX "")
    set_target_properties(${SLM_MODULE} PROPERTIES OUTPUT_NAME "${MODULE_NAME}")

    # set_target_properties(${SLM_MODULE} PROPERTIES SUFFIX ".so")
    set(file_root "${SLM_PACKAGE_DIR}/${MODULE_NAME}")

    # set(file_root "${SLM_PACKAGE_DIR}")
    set_target_properties(${SLM_MODULE} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${file_root})

    file(WRITE "${SLM_PACKAGE_DIR}/${MODULE_NAME}/__init__.py" "from .${MODULE_NAME} import *\n")
endfunction()

# add_module(Foo)
add_module(SequentialGroup)

add_module(narsese)

target_link_libraries(SequentialGroup PRIVATE fmt::fmt)
target_include_directories(SequentialGroup PRIVATE ${fmt_INCLUDE_DIRS})

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