project (LLVMLoader)
cmake_minimum_required (VERSION 2.8)
cmake_policy(SET CMP0015 NEW) # relative pathes given to link_directories

set (EXECUTABLE_OUTPUT_PATH "out/bin")
set (LIBRARY_OUTPUT_PATH "out/libs")

set (UNIX_HOST	ON   CACHE BOOL   "To enable LLVM_ON_UNIX")

if (UNIX_HOST)
	add_definitions (-DLLVM_ON_UNIX)
endif (UNIX_HOST)

#list (APPEND DIR_LIST
#	llvm/ADT
#	llvm/Config
#	llvm/MC
#	llvm/Object			
#	llvm/Support
#
#	llvm-c
#	
#	lib/MC
#	lib/Object
#	lib/Support
#)

add_definitions(-std=c++11)

include(include/headers.cmake)

#include_directories(${DIR_LIST})
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

#foreach (DIR ${DIR_LIST})
#	aux_source_directory ("${DIR}" LOADER_SOURCES)
#endforeach (DIR)

#list (APPEND ELF64_LOADER_SOURCES ELF64-loader.cpp)

#add_executable (elfloader ${LOADER_SOURCES} ${ELF64_LOADER_SOURCES})
