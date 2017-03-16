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

object::ELFFile<object::ELFType<support::little, false>>::Elf_Shdr getSection(const object::ELFFile<object::ELFType<support::little, false>>* elf, unsigned section_name)
{
	uint64_t num_of_sections = elf->getNumSections();

//	printf("Required section name: %ld\n", static_cast<uint64_t>(section_name));
//	printf("Num of sectons: %ld\n", num_of_sections);

	for (int i = 0; i < num_of_sections; i++) {
//		printf("sh type: %ld\n", ((elf->sections())[i]).sh_type);
		if (((elf->sections())[i]).sh_type == section_name)
			return (elf->sections())[i];
	}	

	printf("Required section was not found\n");
	abort();
}

StringRef getStrtab(const object::ELFFile<object::ELFType<support::little, false>>* elf, const object::ELFFile<object::ELFType<support::little, false>>::Elf_Shdr* Section)
{
	ErrorOr<StringRef> strtab = elf->getStringTable(Section);
	
	if (strtab.getError()) {
		printf("Strtab was not found.\n Error occured !!! \n"); 	
		abort();
	}

	StringRef storage = strtab.get();

	return storage;
}

StringRef getSymbolName(const object::ELFFile<object::ELFType<support::little, false>>::Elf_Sym* symbol, StringRef strtab)
{
	Expected<StringRef> name = symbol->getName(strtab);

	if (!name)
		printf("error\n");
	
	StringRef sym_name = name.get();	

	return sym_name;
}

unsigned searchMain(const object::ELFFile<object::ELFType<support::little, false>>* elf)
{
	const char* main_name = "main";

	uint64_t num_of_sections = elf->getNumSections();

	object::ELFFile<object::ELFType<support::little, false>>::Elf_Shdr dynsym_section = getSection(elf, SHT_DYNSYM);

	object::ELFFile<object::ELFType<support::little, false>>::Elf_Shdr strt = getSection(elf, SHT_STRTAB);
	StringRef strtab = getStrtab(elf, &strt);

	unsigned entities_num = dynsym_section.getEntityCount();

	for (int j = 0; j < entities_num; j++) {
		StringRef sym_name = getSymbolName(elf->getSymbol(&(dynsym_section), j), strtab);
		
		if (!strcmp(main_name, sym_name.data())) {
			printf("entry point: %llx\n", (elf->getSymbol(&(dynsym_section), j))->st_value);			
			return j;
		}		
				
	}
	
	return -1;
}

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

/*
	object::ELFFile<object::ELFType<support::little, false>>::Elf_Shdr shdr = getSection(elf, SHT_DYNSYM);

	printf("entities: %d\n", shdr.getEntityCount());

	printf("value: %lx\n", (elf->getSymbol(&(shdr), 3))->getValue());

	object::ELFFile<object::ELFType<support::little, false>>::Elf_Shdr strt = getSection(elf, SHT_STRTAB);

	StringRef strtab = getStrtab(elf, &strt);

	StringRef sym_name = getSymbolName(elf->getSymbol(&(shdr), 3), strtab);

	printf("sym_name: %s\n", sym_name.data());
*/

	printf("main entry: %d\n", searchMain(elf));

	printf("ELF 64 Loader Success !!!\n");

	munmap(filemap, len);

	return 0;
}