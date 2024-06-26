# CMakeLists files in this project can
# refer to the root source directory of the project as ${ConnectAndGrabExample_SOURCE_DIR} and
# to the root binary directory of the project as ${ConnectAndGrabExample_BINARY_DIR}.
cmake_minimum_required (VERSION 3.10)

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall --std=c++11 -O3 -fPIC" )

include(FetchContent)

if(POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()

project (phoxi-sensor)

set(CMAKE_RELEASE_POSTFIX "_Release")
set(CMAKE_DEBUG_POSTFIX "_Debug")

set(CMAKE_CXX_STANDARD 11)

if (UNIX AND NOT APPLE)
    add_compile_options(-std=c++1y)
    add_compile_options(-pthread)
endif(UNIX AND NOT APPLE)

add_library (phoxi-sensor 
    src/phoxi_sensor.cpp
)

add_executable(grab-frame 
    src/grab_frame.cpp
)

if (NOT PHO_API_CMAKE_CONFIG_PATH)
    set(PHO_API_CMAKE_CONFIG_PATH "$ENV{PHOXI_CONTROL_PATH}")
endif()

find_package(PhoXi REQUIRED CONFIG PATHS "${PHO_API_CMAKE_CONFIG_PATH}")

target_link_libraries(phoxi-sensor
    ${PHOXI_LIBRARY}
    $<$<PLATFORM_ID:Linux>:rt>
)

target_link_libraries(grab-frame PUBLIC phoxi-sensor)
target_include_directories(grab-frame PUBLIC include ${PHOXI_INCLUDE_DIRS})

get_target_property(ReleaseBinFolder phoxi-sensor RUNTIME_OUTPUT_DIRECTORY_RELEASE)
get_target_property(DebugBinFolder phoxi-sensor RUNTIME_OUTPUT_DIRECTORY_DEBUG)

target_include_directories (phoxi-sensor PUBLIC include ${PHOXI_INCLUDE_DIRS})

set_target_properties(phoxi-sensor
    PROPERTIES
    #for executables, inheritance of variables CMAKE_DEBUG_POSTFIX... does not work
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
    RELEASE_POSTFIX ${CMAKE_RELEASE_POSTFIX}
)
set_target_properties(phoxi-sensor PROPERTIES POSITION_INDEPENDENT_CODE ON)

FetchContent_Declare(pybind11 GIT_REPOSITORY https://github.com/pybind/pybind11.git GIT_TAG v2.12.0)
FetchContent_MakeAvailable(pybind11)

pybind11_add_module(python_phoxi_sensor
    src/python_phoxi_sensor.cpp
)
target_include_directories(python_phoxi_sensor PUBLIC include ${PHOXI_INCLUDE_DIRS})
target_link_libraries(python_phoxi_sensor PRIVATE phoxi-sensor)
set_target_properties(python_phoxi_sensor PROPERTIES OUTPUT_NAME phoxi)
set_target_properties(python_phoxi_sensor PROPERTIES ARCHIVE_OUTPUT_NAME python_phoxi)

install(TARGETS phoxi-sensor LIBRARY DESTINATION .)
install(TARGETS python_phoxi_sensor LIBRARY DESTINATION .)