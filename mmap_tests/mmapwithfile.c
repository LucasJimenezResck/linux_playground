#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h> //close
#include <fcntl.h> //open
#include <sys/stat.h> //stat struct, fstat
#include <ctype.h> //toupper

int main(int argc, char* argv[])
{
    //instead of O_RDONLY
    int fd = open("./poem.txt", O_RDWR, S_IRUSR | S_IWUSR);

    struct stat sb;

    if(fstat(fd, &sb) != 0)
    {
        perror("Error: file couldn't be open");
    }
    
    printf("File size: %ldB\n", sb.st_size);

    //MAP_SHARED allows the changes in memory blocks to be updated into the file
    char* file_in_memory = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    for(int i = 0; i < sb.st_size; i++)
    {


        if(file_in_memory[i-1] == '\n' || i==0) //Single 
        {
            file_in_memory[i] = toupper(file_in_memory[i]);
        }
        printf("%c", file_in_memory[i]);
    }
    printf("\n");
    munmap(file_in_memory, sb.st_size);
    close(fd);
}
