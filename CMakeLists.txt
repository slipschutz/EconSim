cmake_minimum_required(VERSION 3.10)

# set the project name
project(EconSim VERSION 1.0)


# add the executable
add_executable(EconSim test.C)
add_subdirectory ("modules/actors")

target_include_directories(EconSim PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/modules/"
                           )

                           