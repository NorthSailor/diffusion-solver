cmake_minimum_required(VERSION 3.0.0)
project(diffusion VERSION 0.1.0)

add_executable(diffusion main.c
                         matrix.c
                         matrix.h)
