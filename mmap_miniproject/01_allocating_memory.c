#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h> //atoi


#define PAGESIZE 4096
#define FIBO 20


int main(int argc, char* argv[])
{
    int* ptr = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);

    if(ptr == MAP_FAILED)
    {
        perror("Memory allocation unsuccessful");
        return -1;
    }

    int fibo;
    if(argc > 1)
        fibo = atoi(argv[1]);
    else
        fibo = FIBO;

    if(fibo >= 1)
    {
        ptr[0] = 1;
        printf("%d ", ptr[0]);
    }


    for(int i = 1; i < fibo; i++)
    {
        ptr[i] = ptr[i-1] + ptr[i -2];
        printf("%d ", ptr[i]);
    }
    printf("\n");


    
    munmap(ptr, PAGESIZE);
    return 0;
}