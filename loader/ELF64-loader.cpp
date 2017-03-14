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
#include "llvm/ADT/ArrayRef.h"

#include "llvm/Support/raw_ostream.h"


using namespace std;
using namespace llvm;


int main(int argc, char** argv)
{
	const char* path = "/home/sabramov/libtest3.so";
//	const char* path = "/home/sabramov/arm-build/out-linux-x86_64/bin/emulator_standalone";
//	const char* path = "/home/sabramov/work/test";

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

	object::ELFObjectFile<object::ELFType<support::little, false>> elffile(membuf, EC);
	const object::ELFFile<object::ELFType<support::little, false>> *elf = elffile.getELFFile();

//	printf("value: %lx\n", (elf->getSymbol((&(elf->sections())[2]), 3))->getValue());

	ErrorOr<StringRef> strtab = elf->getStringTable((&(elf->sections())[3]));
	
	if (strtab.getError())
		printf("error %d\n", strtab.getError()); 	

	StringRef storage = strtab.get();

	Expected<StringRef> name = (elf->getSymbol((&(elf->sections())[2]), 3))->getName(storage);

	if (! name)
		printf("error\n");
	
	StringRef hname = name.get();


	printf("name: %s\n", hname.data());

 	uint64_t numsec = elf->getNumSections();
 	printf("numsec: %ld\n", numsec);


//	object::elf_symbol_iterator iter = elffile.dynamic_symbol_begin();  
//	++iter;
//	const object::ELFSymbolRef* ref = iter.operator->();
//	const object::BasicSymbolRef* ref = iter.operator->();
//	llvm::raw_ostream &os();
//	raw_ostream OS;
//	std::error_code cd = ref->printName(OS);
//	printf("size: %lld\n", ref->getSize());
//	Expected<StringRef> st = ref->getName();
//	printf("name: %s\n", st.data());
//	object::ELFSymbolRef* ref = new object::ELFSymbolRef(iter);
//	object::ELFSymbolRef sym(iter->());

	unsigned arch = elffile.getArch();

	printf("arch: %d\n", arch);

	printf("ELF 64 Loader Success !!!\n");

	munmap(filemap, len);

	return 0;
}