#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

void write_nvram(off_t target, const uint8_t *data, size_t length) {
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
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

        void *map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (target + offset) & ~MAP_MASK);
        if (map_base == MAP_FAILED) {
            perror("mmap");
            close(fd);
            exit(1);
        }

        volatile uint8_t *virt_addr = (volatile uint8_t *)map_base + ((target + offset) & MAP_MASK);

        // Write data to memory
        memcpy((void *)virt_addr, data + offset, chunk_size);

        munmap(map_base, MAP_SIZE);
        offset += chunk_size;
    }

    close(fd);
}

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <phys_addr_hex> <data_hex> <length>\n", argv[0]);
        return 1;
    }

    off_t target = strtoul(argv[1], NULL, 16);
    size_t length = strtoul(argv[3], NULL, 10);

    uint8_t *data = malloc(length);
    if (!data) {
        perror("malloc");
        return 1;
    }

    // Parse the data from the hex string
    const char *data_hex = argv[2];
    for (size_t i = 0; i < length; i++) {
        sscanf(data_hex + i * 2, "%2hhx", &data[i]);
    }

    write_nvram(target, data, length);

    free(data);
    return 0;
}