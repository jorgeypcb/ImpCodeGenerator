# Requirements
cmake_minimum_required(VERSION 3.14)

################################
## Section 1: Declare project ##
################################
set(project_name cs554_warmup)
set(libname "${project_name}")
project(${project_name} CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
include("${CMAKE_CURRENT_SOURCE_DIR}/helper.cmake")

###################################
## Section 2: Project CMake code ##
###################################

# Add library and register compile options
add_library(${libname} INTERFACE)

target_include_directories(
    ${libname}
    INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include>
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION include
)

if(PROJECT_IS_TOP_LEVEL)
    set(BENCHMARK_ENABLE_TESTING OFF)
    find_or_fetch(fmt "https://github.com/fmtlib/fmt.git" master)
    # find_or_fetch(benchmark "https://github.com/google/benchmark.git" main)

    add_source_dir(src ${libname} fmt)
    # add_source_dir(bench ${libname} benchmark)
    # add_test_dir(test ${libname} fmt)
endif()

# Add install target for execute
install(TARGETS execute RUNTIME DESTINATION bin)
