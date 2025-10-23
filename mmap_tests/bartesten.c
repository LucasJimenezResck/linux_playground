#include <stdio.h>
#include <unistd.h> //close
#include <string.h>
#include <stdlib.h> //strtol
#include <stdint.h> //uint8_t, uint32_t, etc
#include <fcntl.h> //open
#include <sys/mman.h> //mmap
#include <sys/stat.h> //stat, fstat more info in man 2 stat
#include <time.h>


#define BAR0SIZE 1048576
#define PAGESIZE 4096
#define IO_CONFIG 0x0000
#define IO_BYTE_1 0x0000

int main(int argc, char* argv[])
{
    int fd;
    time_t currentTime;
    time(&currentTime);
    //Unused variables unless used for smaller BARs
    //uint32_t offset, config[5];
    
    if(argc != 3)
    {
        printf("Command: %s <bus>:<device>.<function> <BAR-number>", argv[0]);
        return 1;
    }
    

    char resourcePath[128];

    sprintf(resourcePath, "/sys/bus/pci/devices/0000:%s/resource%s", argv[1], argv[2]); 

    int i;
    uint8_t value;
    //char charvalue[5];
    void* bar0;
    fd = open(resourcePath, O_RDONLY | O_SYNC, S_IRUSR | S_IWUSR);

    struct stat sb;

    if(fstat(fd, &sb) != 0 )
    {
        perror("Error: file could not be open");
    }

    printf("File size: %ld\n", sb.st_size);
    //printf("Device-ID: %lx\n", sb.st_rdev);
    if(fd < 0)
    {
        perror("Error opening BAR's Resource File");
        return -1;
    }

    
    
    bar0 = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    

    if(bar0 == MAP_FAILED)
    {
        perror("Memory Mapping of BAR failed");
        return -1;
    }


    //Access the card
    FILE* foutput = fopen("./output1.txt", "a");
    fprintf(foutput, "Device %s BAR%s memory output on: %s\n", argv[1], argv[2], ctime(&currentTime));
    
   //*(bar0 + IO_CONFIG) = 0; //Set all configuration bytes to 0
    for(i = 0; i < sb.st_size; i++)
    {
        value =  *((uint8_t *) bar0 + i); //Read into the GPIO values

        //printf("%x ", value);
        //sprintf(charvalue, "%d ", value);
        fprintf(foutput, "%02x ", value);
        //Every 10 bytes add a new line
        if(i % 10 == 9 && i > 0)
        {
            fprintf(foutput, "\n");
        }
        //printf("\tAddress of value: %p\n", bar0 + PAGESIZE - 1);

    }
    fprintf(foutput, "\n");
    munmap(bar0, sb.st_size);
    fclose(foutput);
    close(fd);
    return 0;
}



#if 0 //Code in case less than a page is to be accessed
    fd = open("/sys/bus/pci/devices/0000:04:00.0/config", O_RDONLY);

    if(fd < 0)
    {
        perror("Error opening Configuration Space");
        return -1;
    }


    i = read(fd, config, 0x14); //Read first 20 bytes of configuration file, where the address of BAR0 is mapped to
    if(i != 0x14)
    {
        perror("Error reading Configuration Space");
        munmap(bar0, PAGESIZE);
        return -1;
    }
    //Calculate Offset, in this case we pick up the upper 28 bits of the BAR0 address
    offset = (config[4] & 0xfffffff0) % PAGESIZE;

    //Adjust according to offset
    bar0 += offset;
#endif