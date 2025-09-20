#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <errno.h>

#define MAP_SIZE 4096UL   // 1 pagina
#define MAP_MASK (MAP_SIZE - 1)

void dump_nvram(off_t target, size_t length) {
    int fd = open("/dev/mem", O_RDONLY | O_SYNC);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    size_t offset = 0;
    while (offset < length) {
        size_t chunk_size = MAP_SIZE - (target & MAP_MASK);
        if (chunk_size > length - offset) {
            chunk_size = length - offset;
        }

        void *map_base = mmap(0, MAP_SIZE, PROT_READ, MAP_SHARED, fd, (target + offset) & ~MAP_MASK);
        if (map_base == MAP_FAILED) {
            perror("mmap");
            close(fd);
            exit(1);
        }

        volatile uint8_t *virt_addr = (volatile uint8_t *)map_base + ((target + offset) & MAP_MASK);

        //printf("Dump of %zu bytes @ 0x%08lX:\n", chunk_size, (unsigned long)(target + offset));
        for (size_t i = 0; i < chunk_size; i++) {
            if (i % 16 == 0) {
                if (i > 0) {
                    // Print ASCII representation for the previous line
                    printf("  ");
                    for (size_t j = i - 16; j < i; j++) {
                        char c = virt_addr[j];
                        printf("%c", (c >= 32 && c <= 126) ? c : '.');
                    }
                }
                printf("\n%08lX  ", (unsigned long)(target + offset + i));
            }
            printf("%02X ", virt_addr[i]);
        }

        // Always print the ASCII representation for the last line
        size_t remaining = chunk_size % 16;
        if (remaining == 0) {
            remaining = 16; // If the last line is full, handle it as a complete line
        }
        printf("  ");
        for (size_t i = chunk_size - remaining; i < chunk_size; i++) {
            char c = virt_addr[i];
            printf("%c", (c >= 32 && c <= 126) ? c : '.');
        }
        //printf("\n");

        munmap(map_base, MAP_SIZE);
        offset += chunk_size;
    }
	printf("\n");
    close(fd);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "nvramdump 0.2 - Dump NVRAM content from physical memory\n");
        fprintf(stderr, "Usage: %s <phys_addr_hex> <length>\n", argv[0]);
        return 1;
    }

    off_t target = strtoul(argv[1], NULL, 16);
    size_t length = strtoul(argv[2], NULL, 10);

    if (length == 0) {
        fprintf(stderr, "nvramdump 0.2 - Dump NVRAM content from physical memory\n");
        fprintf(stderr, "Error: length has to be larger then 0.\n");
        return 1;
    }
	printf("nvramdump 0.2 - Dump NVRAM content from physical memory\n");
    dump_nvram(target, length);
	
    return 0;
}
