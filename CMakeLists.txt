cmake_minimum_required(VERSION 3.22)

project(neo)

add_library(${PROJECT_NAME} STATIC
    "src/neo.cpp"

    "src/rectangle.cpp"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "include"
        "src"
)
