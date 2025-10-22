#include <stdio.h>
#include <stdint.h> //uint8_t type variables
#include <sys/mman.h> //mmap and constants
#include <unistd.h> //brk, sbrk

#define PAGESIZE 4096

int main()
{
    //sbrk and brk Function move the programm break of the memory. In this example you give how many bytes you want it moved and it returns the pointer to the 
    //location of last break. The other function works the opposite: you give which address you want the break to be and returns 0 in case of success 
    void* brkPoint = sbrk(0);
    printf("Heap Address Program Break: %p\n", brkPoint);
    
    /// CHECK FURTHER WITH: man mmap

    //mmap requests blocks of memory that are multiples of one page size
    //1. asks where you want the access to be (if it doesn't matter NULL), just a hint. Below are examples
    //2. size of block, can be whatever the user wants but ends up reserving at least one page
    //3. Page protections like read and write
    //4. flags
    uint8_t* first = mmap(brkPoint - 1, PAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    uint8_t* second = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    uint8_t* third = mmap((void *) 0xFEEDCAFE, PAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);


    printf("First: %p\n", first);
    printf("Second: %p\n", second);
    printf("Third: %p\n", third);
}