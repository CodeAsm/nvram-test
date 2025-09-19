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

// Function to write data directly to memory
void write_direct_bytes(off_t target, uint8_t *data, size_t length) {
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

        // Write the data to memory
        memcpy((void *)virt_addr, data + offset, chunk_size);

        munmap(map_base, MAP_SIZE);
        offset += chunk_size;
    }

    close(fd);
}

// Function to wipe memory with a repeating byte pattern
void wipe_nvram(off_t target, const uint8_t *pattern, size_t pattern_length, size_t length) {
    uint8_t *data = malloc(length);
    if (!data) {
        perror("malloc");
        exit(1);
    }

    // Fill the buffer with the repeating pattern
    for (size_t i = 0; i < length; i++) {
        data[i] = pattern[i % pattern_length];
    }

    write_direct_bytes(target, data, length);
    free(data);
}

// Function to write the contents of a file to memory
void write_file_to_nvram(off_t target, const char *file_path, size_t length) {
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        perror("fopen");
        exit(1);
    }

    uint8_t *data = malloc(length);
    if (!data) {
        perror("malloc");
        fclose(file);
        exit(1);
    }

    size_t bytes_read = fread(data, 1, length, file);
    if (bytes_read < length) {
        if (!feof(file)) {
            perror("fread");
            free(data);
            fclose(file);
            exit(1);
        }
    }

    write_direct_bytes(target, data, bytes_read);

    free(data);
    fclose(file);
}

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  %s <phys_addr_hex> <length> <data_hex>\n", argv[0]);
        fprintf(stderr, "  %s <phys_addr_hex> <length> --wipe <pattern>\n", argv[0]);
        fprintf(stderr, "  %s <phys_addr_hex> <length> --write <file>\n", argv[0]);
        return 1;
    }

    // Parse the physical address
    off_t target = strtoul(argv[1], NULL, 16);
    if (target == 0) {
        fprintf(stderr, "Invalid physical address: %s\n", argv[1]);
        return 1;
    }

    // Parse the length
    size_t length = strtoul(argv[2], NULL, 10);
    if (length == 0) {
        fprintf(stderr, "Invalid length: %s\n", argv[2]);
        return 1;
    }

    // Handle the operation
    if (strcmp(argv[3], "--wipe") == 0) {
        if (argc < 5) {
            fprintf(stderr, "Error: Missing pattern for --wipe\n");
            return 1;
        }

        const char *pattern_hex = argv[4];
        size_t pattern_length = strlen(pattern_hex) / 2;

        uint8_t *pattern = malloc(pattern_length);
        if (!pattern) {
            perror("malloc");
            return 1;
        }

        for (size_t i = 0; i < pattern_length; i++) {
            sscanf(pattern_hex + i * 2, "%2hhx", &pattern[i]);
        }

        wipe_nvram(target, pattern, pattern_length, length);
        free(pattern);
    } else if (strcmp(argv[3], "--write") == 0) {
        if (argc < 5) {
            fprintf(stderr, "Error: Missing file path for --write\n");
            return 1;
        }
        write_file_to_nvram(target, argv[4], length);
    } else {
        // Direct byte writing
        const char *data_hex = argv[3];
        size_t data_length = strlen(data_hex) / 2;
        if (data_length != length) {
            fprintf(stderr, "Data length (%zu) does not match specified length (%zu)\n", data_length, length);
            return 1;
        }

        uint8_t *data = malloc(length);
        if (!data) {
            perror("malloc");
            return 1;
        }

        for (size_t i = 0; i < length; i++) {
            sscanf(data_hex + i * 2, "%2hhx", &data[i]);
        }

        write_direct_bytes(target, data, length);
        free(data);
    }

    return 0;
}