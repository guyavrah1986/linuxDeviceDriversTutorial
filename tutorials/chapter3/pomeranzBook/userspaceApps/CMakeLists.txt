# ======================================================================================================================================
# general project settings
# ======================================================================================================================================
cmake_minimum_required(VERSION 3.5)

project("userSpaceAppForSimpleCharDev")

message(STATUS "processing CMakeLists of project:${CMAKE_PROJECT_NAME} within folder:${CMAKE_CURRENT_LIST_DIR}")

# ======================================================================================================================================
# include directories
# ======================================================================================================================================


# ======================================================================================================================================
# group all source files of the executable
# ======================================================================================================================================

# add all the cpp files manually in order to aviod bug in cmake that adds an additional "internal main" of its own 
set(MAIN_EXE_SRC userSpaceApp1.cpp)

# for debug
message(STATUS "collected into MAIN_EXE_SRC the following files:" ${MAIN_EXE_SRC})

# ======================================================================================================================================
# add relevant sub directories -- invoke their CMakeLists.txt files
# ======================================================================================================================================


# ======================================================================================================================================
# create the main executable
# ======================================================================================================================================
add_executable(userSpaceAppExe ${MAIN_EXE_SRC})
   
# ======================================================================================================================================
# link the executable with all relevant libraries
# ======================================================================================================================================
                 
