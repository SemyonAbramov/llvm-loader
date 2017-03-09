

list (APPEND HEADER_LIST
	llvm/ADT
	llvm/Config
	llvm/MC
	llvm/Object			
	llvm/Support

	llvm-c
)

include_directories(${HEADER_LIST})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
