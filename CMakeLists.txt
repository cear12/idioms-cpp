cmake_minimum_required(VERSION 3.16)
project(idioms_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)

find_package(Threads REQUIRED)

include(CTest)
enable_testing()

# Each idiom under idioms/<name>/ is a single self-contained example.cpp
# with its own main(), independent of every other idiom (deliberately --
# these are meant to be readable and copy-pasteable in isolation, unlike
# leetcode-cpp's solutions, which share a common prelude). Every one
# becomes its own executable and CTest case.
file(GLOB IDIOM_DIRS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/idioms/*")

foreach(DIR ${IDIOM_DIRS})
    if(IS_DIRECTORY "${DIR}" AND EXISTS "${DIR}/example.cpp")
        get_filename_component(IDIOM_NAME "${DIR}" NAME)

        add_executable(${IDIOM_NAME} "${DIR}/example.cpp")

        # thread-safe-interface uses std::thread and needs to link pthread
        # on Linux; harmless to link for every target on platforms where
        # Threads::Threads is a no-op.
        target_link_libraries(${IDIOM_NAME} PRIVATE Threads::Threads)

        add_test(NAME ${IDIOM_NAME} COMMAND ${IDIOM_NAME})
    endif()
endforeach()

list(LENGTH IDIOM_DIRS NUM_IDIOMS)
message(STATUS "idioms-cpp: configured ${NUM_IDIOMS} idiom executables/tests")
