cmake_minimum_required (VERSION 3.11)

project ("raytracing")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STRANDARD_REQUIRED ON) 

find_package(OpenMP REQUIRED)
find_package(benchmark CONFIG REQUIRED)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    include("~/Documents/GitHub/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()
if(UNIX AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET x64-linux)
endif()
if(WIN32 AND NOT DEFINED VCPKG_TARGET_TRIPLET) 
    set(VCPKG_TARGET_TRIPLET x64-windows)
endif()

# Tracy
option(ENABLE_PROFILING "Enable profiling" ON)
if (ENABLE_PROFILING)
    add_subdirectory(externals/tracy)
    set_target_properties(TracyClient PROPERTIES FOLDER externals/)
    option(TRACY_NO_EXIT "" ON)
endif()

# PROGRAM FILES
file(GLOB_RECURSE SRC_FILES src/*.cpp include/*.hpp)
add_executable(Raytracer main/main.cpp ${SRC_FILES})
target_include_directories(Raytracer PUBLIC include/)
target_link_libraries(Raytracer PUBLIC OpenMP::OpenMP_CXX)

# Tracy
if (ENABLE_PROFILING)
    target_link_libraries(Raytracer PUBLIC Tracy::TracyClient)
    target_compile_definitions(Raytracer PUBLIC TRACY_ENABLE)
endif()

file(GLOB BENCH_FILES bench/*.cpp)

# Benchmarks
foreach(BENCH_FILE ${BENCH_FILES})
    get_filename_component(BENCH_NAME ${BENCH_FILE} NAME_WE)
    add_executable(${BENCH_NAME} ${BENCH_FILE} ${SRC_FILES})
    target_link_libraries(${BENCH_NAME} PRIVATE benchmark::benchmark benchmark::benchmark_main OpenMP::OpenMP_CXX)
    target_include_directories(${BENCH_NAME} PRIVATE include/)
    if(MSVC)
        target_compile_definitions(${BENCH_NAME} PUBLIC "_USE_MATH_DEFINES")
        target_compile_options(${BENCH_NAME} PUBLIC /arch:AVX2 /Oi /GR- /EHs-c- /FA /Oy- /GL)
        target_link_options(${BENCH_NAME} PUBLIC /LTCG)
    else()
        target_compile_options(${BENCH_NAME} PUBLIC "-march=haswell" "-masm=intel" -fno-rtti -fno-exceptions -save-temps
                -fno-omit-frame-pointer -flto -ffast-math)
        target_link_options(${BENCH_NAME} PUBLIC -flto)
    endif()
    
    set_target_properties (${BENCH_NAME} PROPERTIES FOLDER Bench)
endforeach(BENCH_FILE ${BENCH_FILES})