#include "llvm/Object/ELFObjectFile.h"
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>

#include <unistd.h>

#include <sys/mman.h>

#include "llvm/Support/MemoryBuffer.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Object/ELF.h"
#include "llvm/Object/ELFTypes.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Object/ELFObjectFile.h"


using namespace std;
using namespace llvm;


int main(int argc, char** argv)
{
//	const char* path = "/home/sabramov/libtest3.so";
//	const char* path = "/home/sabramov/arm-build/out-linux-x86_64/bin/emulator_standalone";
	const char* path = "/home/sabramov/work/test";

	Elf64_Ehdr ehdr;

	int fd = open(path, O_RDONLY);

	if (fd == -1)
		printf("Open file failed\n"); 

	long long len = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	void* filemap = mmap(NULL, len, PROT_READ, MAP_SHARED, fd, 0);

	memcpy(&ehdr, filemap, sizeof(Elf64_Ehdr));

	if (filemap == MAP_FAILED)
		printf("MAP FAILED\n");


	StringRef* strref = new StringRef((char*)filemap, len);
	StringRef* id = new StringRef("Elf file\n");

 	MemoryBufferRef membuf((*strref), (*id));

	size_t buf_size = membuf.getBufferSize();

	std::error_code EC;

//	object::ELFObjectFile<object::ELFType<support::little, false>> * elffile = new object::ELFObjectFile<object::ELFType<support::little, false>>(membuf, EC);  

	object::ELFObjectFile<object::ELFType<support::little, true>> elffile(membuf, EC);

	unsigned arch = elffile.getArch();

	printf("arch: %d\n", arch);

	printf("ELF 64 Loader Success !!!\n");

	munmap(filemap, len);

	return 0;
}