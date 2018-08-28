cmake_minimum_required(VERSION 3.0.0)
project(diffusion VERSION 0.1.0)

option(USE_DOUBLES "Build with double precision arithmetic." OFF)

if(USE_DOUBLES)
        message("Using double precision arithmetic.")
        add_definitions(-DUSE_DOUBLES)
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
