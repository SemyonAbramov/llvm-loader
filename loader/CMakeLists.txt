project (LLVMLoader)
cmake_minimum_required (VERSION 2.8)
cmake_policy(SET CMP0015 NEW) # relative pathes given to link_directories

set(LLVM_DEFAULT_TARGET_TRIPLE "${LLVM_HOST_TRIPLE}" CACHE STRING
  "Default target for which LLVM will generate code." )
set(TARGET_TRIPLE "${LLVM_DEFAULT_TARGET_TRIPLE}")

add_definitions(-D__linux__)

set (EXECUTABLE_OUTPUT_PATH "out/bin")
set (LIBRARY_OUTPUT_PATH "out/libs")

set (UNIX_HOST	ON   CACHE BOOL   "To enable LLVM_ON_UNIX")

if (UNIX_HOST)
	add_definitions (-DLLVM_ON_UNIX)
endif (UNIX_HOST)

#set(LLVM_TARGET_DEFINITIONS /llvm/IR/Attributes.td)
#tablegen(LLVM /llvm/IR/Attributes.inc -gen-attrs)

list (APPEND DIR_LIST
	llvm/ADT
	llvm/Config
	llvm/ExecutionEngine
	llvm/MC
	llvm/Object			
	llvm/Support
	llvm/Target

	llvm-c
	
	Analysis
	Bitcode
	ExecutionEngine
	IR
	MC
	Object
	ProfileData
	Support
	Target
	ExecutionEngine/RuntimeDyld
)

add_definitions(-std=c++11)

#include(include/add_headers.cmake)

include_directories(${DIR_LIST})
set(CMAKE_INCLUDE_CURRENT_DIR ON)

foreach (DIR ${DIR_LIST})
	aux_source_directory ("${DIR}" LOADER_SOURCES)
endforeach (DIR)

list (APPEND ELF64_LOADER_SOURCES ELF64-loader.cpp)

add_executable (elfloader ${LOADER_SOURCES} ${ELF64_LOADER_SOURCES})
