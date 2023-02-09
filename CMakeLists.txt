cmake_minimum_required(VERSION 3.0.0)
project(hero VERSION 0.1.0)







# Disable warnings
add_compile_options(-w)
add_compile_options(-fpermissive -fno-omit-frame-pointer)
add_compile_options(-fmax-errors=1)

# Debug
add_compile_options(-O0 -ggdb)

# Release
#add_compile_options(-02 -ggdb)

# Required for shared library build (position independent code)
add_compile_options(-fPIC)

# Avoid annoying range based errors for std::data.  
#add_compile_options(-std=c++0x)
#add_compile_options(-std=c++11)
#add_compile_options(-std=c99)

# Hero can build entirely without libstdc++
#add_compile_options(-fno-threadsafe-statics -fno-exceptions -fno-rtti -fno-builtin)
add_compile_options(-fno-exceptions -fno-rtti)


#set(MODULE_NAME ${CMAKE_CURRENT_PROJECT})
set(MODULE_NAME ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_library(
    ${MODULE_NAME}_objects
    OBJECT
    ${SRC_FILES}
)       
target_include_directories(${MODULE_NAME}_objects PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src )

# CLang format source files
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ClangFormat)
#clangformat_setup(${SRC_FILES})
target_clangformat_setup(${MODULE_NAME}_objects)

# Reformat from project root using
# cmake --build build --target clangformat

# Static library (libhero.a)
add_library(libhero STATIC $<TARGET_OBJECTS:${MODULE_NAME}_objects>)
set_target_properties(libhero PROPERTIES PREFIX "")

# Dynamic library (libhero.so)
add_library(libheroshared SHARED $<TARGET_OBJECTS:${MODULE_NAME}_objects>)
target_include_directories(libheroshared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src )
set_target_properties(libheroshared PROPERTIES OUTPUT_NAME "libhero")
set_target_properties(libheroshared PROPERTIES PREFIX "")
set_target_properties(libheroshared PROPERTIES SUFFIX ".so")

# Executable (hero)
add_executable(hero $<TARGET_OBJECTS:${MODULE_NAME}_objects>)