cmake_minimum_required (VERSION 3.11)

project ("raytracing")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STRANDARD_REQUIRED ON) 

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    include("~/Documents/GitHub/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()
if(UNIX AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET x64-linux)
endif()
if(WIN32 AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET x64-windows)
endif()

#Tracy
option(ENABLE_PROFILING "Enable profiling" ON)
if (ENABLE_PROFILING)
    add_subdirectory(externals/tracy)
endif()
		
#PROGRAM FILES
file(GLOB_RECURSE SRC_FILES src/*.cpp include/*.hpp)
add_executable(Raytracer ${SRC_FILES})
target_include_directories(Raytracer PUBLIC include/)

#Tracy
if (ENABLE_PROFILING)
    target_link_libraries(Raytracer PUBLIC TracyClient)
    target_compile_definitions(Raytracer PUBLIC TRACY_ENABLE)
endif()