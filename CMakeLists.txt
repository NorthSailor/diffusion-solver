cmake_minimum_required(VERSION 3.0.0)
project(diffusion VERSION 0.1.0)

option(USE_DOUBLES "Build with double precision arithmetic." OFF)

if(USE_DOUBLES)
        message("Using double precision arithmetic.")
        add_definitions(-DUSE_DOUBLES)
endif()

set(CMAKE_C_STANDARD 11)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_executable(diffusion 
        main.c
        matrix.c
        matrix.h
        number.h
        problem.c
        problem.h
        solver.c
        solver.h
)

target_link_libraries(diffusion m)
