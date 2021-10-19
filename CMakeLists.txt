cmake_minimum_required(VERSION 3.14)

project(
    "Imp"
    VERSION 0.0.1
    DESCRIPTION "Code Generator for IMP language, written in C++"
    HOMEPAGE_URL "https://github.com/codeinred/Imp/"
    LANGUAGES CXX
)

add_library(Imp INTERFACE)
add_library(Imp::Imp ALIAS Imp)

# For ${CMAKE_INSTALL_<DIR>} variables that are standarized
include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_compile_features(
    Imp
    INTERFACE
    cxx_std_20
)

target_include_directories(
    Imp
    INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Installation
# See: https://dominikberner.ch/cmake-interface-lib/

install(
    TARGETS Imp
    EXPORT Imp_Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "ImpConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/ImpConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/ImpConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/Imp/cmake
)

install(
    EXPORT Imp_Targets
    FILE ImpTargets.cmake
    NAMESPACE Imp::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/Imp/cmake
)

install(FILES "${PROJECT_BINARY_DIR}/ImpConfig.cmake"
              "${PROJECT_BINARY_DIR}/ImpConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/Imp/cmake
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/Imp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

include(helper)

find_or_fetch(
    noam
    https://github.com/codeinred/noam.git
    main)
find_or_fetch(
    rva
    https://github.com/codeinred/recursive-variant.git
    main)
find_or_fetch(
    fmt
    https://github.com/fmtlib/fmt.git
    master)
target_link_libraries(Imp INTERFACE noam::noam rva::rva fmt::fmt)

# The standard for the whole project should be set to C++20 so that Catch2
# builds with C++20. This will prevent linking errors.
set(CMAKE_CXX_STANDARD 20)
if(PROJECT_IS_TOP_LEVEL)
    include(CTest)

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    # Add all files in directory test as tests, and link against Imp
    find_or_fetch(
        Catch2
        https://github.com/catchorg/catch2.git
        devel
        3.0.0)
    FetchContent_MakeAvailable(${remote_dependencies})

    add_source_dir(
        src
        Imp::Imp)
    add_executable(
        test_imp
        test/test_imp.cpp)
    target_link_libraries(
        test_imp
        Imp::Imp
        Catch2::Catch2WithMain)

    list(APPEND CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake/Catch2)
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(CTest)
    include(Catch)
    catch_discover_tests(test_imp)
endif()


