cmake_minimum_required(VERSION 3.5)

project(diff_sync LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(diff_sync_m main.cpp)
add_executable(diff_sync_s main.cpp)
add_executable(diff_sync_n main.cpp)

include(GNUInstallDirs)
install(TARGETS diff_sync_m
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS diff_sync_s
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS diff_sync_n
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set_target_properties( diff_sync_m PROPERTIES COMPILE_FLAGS "-DUSE_MUTEX" )
set_target_properties( diff_sync_s PROPERTIES COMPILE_FLAGS "-DUSE_SPINLOCK" )
set_target_properties( diff_sync_n PROPERTIES COMPILE_FLAGS "" )


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3 -Wpedantic -Wall -Wextra -Wconversion -Wsign-conversion -Werror")
message(STATUS "C++ FLAGS: ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "C++ DEBUG FLAGS: ${CMAKE_CXX_FLAGS_DEBUG}")
