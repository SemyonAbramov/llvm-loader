#include "llvm/Object/ELFObjectFile.h"
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#include <sys/mman.h>

#include "llvm/Support/MemoryBuffer.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Object/ELF.h"
#include "llvm/Object/ELFTypes.h"
#include "llvm/Support/raw_ostream.h"


using namespace std;
using namespace llvm;


int main(int argc, char** argv)
{
	const char* path = "/home/sabramov/libtest3.so";

	int fd = open(path, O_RDONLY);

	if (fd == -1)
		printf("Open file failed\n"); 

	long long len = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	void* filemap = mmap(NULL, len, PROT_READ, MAP_SHARED, fd, 0);

	if (filemap == MAP_FAILED)
		printf("MAP FAILED\n");


	StringRef* strref = new StringRef((char*)filemap);
	StringRef* id = new StringRef("Elf file");

//	MemoryBufferRef* membuf = new MemoryBufferRef(*strref, *id); 
	MemoryBufferRef membuf((*strref), (*id));


	std::error_code EC;

	object::ELFObjectFile<object::ELFType<support::little, false>> * elffile = new object::ELFObjectFile<object::ELFType<support::little, false>>(membuf, EC);  

//	object::ELFObjectFile<object::ELFType<support::little, false>> elffile(membuf, EC);

	printf("ELF 64 Loader !!!\n");

	munmap(filemap, len);

	return 0;
}