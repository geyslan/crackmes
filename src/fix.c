#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/procfs.h>
#include <fcntl.h>
 
 
int main(int argc, char** argv){
  printf(".: Elf fix :.\n");
 
  if(argc < 2){
    printf("Usage: %s file", argv[0]);
    return 1;
  }
  else{
 
    int f;
    static Elf32_Ehdr* header;
 
    if((f = open(argv[1], O_RDWR)) < 0){
      perror("open");
      return 1;
    }
 
    if((header = (Elf32_Ehdr *) mmap(NULL, sizeof(header), PROT_READ | PROT_WRITE, MAP_SHARED, f, 0)) == MAP_FAILED){
      perror("mmap");
      close(f);
      return 1;
    }
    printf("[*] Current e_shnum: %d\n", header->e_shnum);
 
    header->e_shoff = 0;
    header->e_shnum = 0;
    header->e_shstrndx = 0;
 
    printf("[*] Patched e_shnum: %d\n", header->e_shnum);
 
    if(msync(NULL, 0, MS_SYNC) == -1){
      perror("msync");
      close(f);
      return 1;
    }
 
    close(f);
    munmap(header, 0);
  }
  return 0;
}
