cmake_minimum_required(VERSION 3.30)
project(search_engine)

set(CMAKE_CXX_STANDARD 14)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/90a41521142c978131f38c6da07b4eb96a9f1ff6.zip
)

set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(search_engine
        main.cpp
        src/ConverterJSON.cpp
        include/ConverterJSON.h
        src/InvertedIndex.cpp
        include/InvertedIndex.h
        src/SearchServer.cpp
        include/SearchServer.h
)

add_custom_command(TARGET search_engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/bin/config.json
        ${CMAKE_BINARY_DIR}/config.json
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/bin/requests.json
        ${CMAKE_BINARY_DIR}/requests.json
)

target_link_libraries(search_engine
        PRIVATE
        nlohmann_json::nlohmann_json
        gtest_main
)

add_executable(search_engine_tests
        tests/mainTest.cpp
        tests/gTestIndex.h
        src/ConverterJSON.cpp
        include/ConverterJSON.h
        src/InvertedIndex.cpp
        include/InvertedIndex.h
        src/SearchServer.cpp
        include/SearchServer.h
        tests/gTestSearch.h
)

target_link_libraries(search_engine_tests
        PRIVATE
        nlohmann_json::nlohmann_json
        gtest_main
)

enable_testing()

include(GoogleTest)
gtest_discover_tests(search_engine_tests)