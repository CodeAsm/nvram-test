#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <pci/pci.h>

// Define colors for terminal output
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"

// PCI configuration space size
#define PCI_CONFIG_SPACE_SIZE 256

// Function to print colored output
void print_colored(const char *color, const char *format, ...) {
    va_list args;
    printf("%s", color);
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("%s", COLOR_RESET);
}

void print_config_space(uint8_t *config) {
    printf("PCI Configuration Space:\n\n");
    printf("    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
    for (int i = 0; i < PCI_CONFIG_SPACE_SIZE; i++) {
        if (i % 16 == 0) printf("\n%02x: ", i);

        // Highlight specific fields based on the PCI configuration space diagram
        if (i == 0 || i == 1) {
            // Vendor ID (bytes 0-1)
            print_colored(COLOR_GREEN, "%02x ", config[i]);
        } else if (i == 2 || i == 3) {
            // Device ID (bytes 2-3)
            print_colored(COLOR_BLUE, "%02x ", config[i]);
        } else if (i == 4 || i == 5) {
            // Command Register (bytes 4-5)
            print_colored(COLOR_RED, "%02x ", config[i]);
        } else if (i == 6 || i == 7) {
            // Status Register (bytes 6-7)
            print_colored(COLOR_RED, "%02x ", config[i]);
        } else if (i == 8) {
            // Revision ID (byte 8)
            print_colored(COLOR_YELLOW, "%02x ", config[i]);
        } else if (i == 9 || i == 10 || i == 11) {
            // Class Code (bytes 9-11)
            print_colored(COLOR_YELLOW, "%02x ", config[i]);
        } else if (i == 12) {
            // Cache Line Size (byte 12)
            print_colored(COLOR_BLUE, "%02x ", config[i]);
        } else if (i == 13) {
            // Latency Timer (byte 13)
            print_colored(COLOR_RED, "%02x ", config[i]);
        } else if (i == 14) {
            // Header Type (byte 14)
            print_colored(COLOR_CYAN, "%02x ", config[i]);
        } else if (i == 15) {
            // BIST (byte 15)
            print_colored(COLOR_CYAN, "%02x ", config[i]);
        } else if (i >= 0x10 && i < 0x28) {
            // BARs (Base Address Registers, bytes 16-39)
            print_colored(COLOR_MAGENTA, "%02x ", config[i]);
        } else {
            // Default color for unknown or reserved bytes
            printf("%02x ", config[i]);
        }
    }
    printf("\n");
}

// Function to read PCI configuration space
int read_pci_config_space(const char *path, uint8_t *buffer, size_t size) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    ssize_t bytes_read = read(fd, buffer, size);
    if (bytes_read < 0) {
        perror("read");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

// Function to decode PCI configuration space
void decode_pci_config_space(uint8_t *config) {
    printf("Decoded PCI Configuration Space:\n");

    // Vendor ID and Device ID
    uint16_t vendor_id = config[0] | (config[1] << 8);
    uint16_t device_id = config[2] | (config[3] << 8);
    print_colored(COLOR_GREEN, "  Vendor ID: %04x\n", vendor_id);
    print_colored(COLOR_BLUE, "  Device ID: %04x\n", device_id);

    // Command and Status Registers
    uint16_t command = config[4] | (config[5] << 8);
    uint16_t status = config[6] | (config[7] << 8);
    print_colored(COLOR_RED, "  Command: %04x\n", command);
    print_colored(COLOR_RED, "  Status: %04x\n", status);

    // Class Code and Subclass
    uint8_t class_code = config[11];
    uint8_t subclass = config[10];
    print_colored(COLOR_YELLOW, "  Class Code: %02x\n", class_code);
    print_colored(COLOR_YELLOW, "  Subclass: %02x\n", subclass);

    // Cache Line Size (Offset 0x0C)
    uint8_t cache_line_size = config[0x0C];
    print_colored(COLOR_BLUE, "  Cache Line Size: %02x\n", cache_line_size);

    // Latency Timer (Offset 0x0D)
    uint8_t latency_timer = config[0x0D];
    print_colored(COLOR_RED, "  Latency Timer: %02x (%d PCI clock cycles)\n", latency_timer, latency_timer);

    // Header Type (Offset 0x0E)
    uint8_t header_type = config[0x0E] & 0x7F; // Mask out the multifunction bit
    uint8_t multifunction = (config[0x0E] & 0x80) >> 7; // Extract the multifunction bit
    print_colored(COLOR_CYAN, "  Header Type: %02x\n", header_type);
    if (multifunction) {
        print_colored(COLOR_YELLOW, "  This is a multifunction device.\n");
    } else {
        print_colored(COLOR_YELLOW, "  This is a single-function device.\n");
    }

    // Base Address Registers (BARs)
    for (int i = 0; i < 6; i++) {
        uint32_t bar = config[0x10 + i * 4] |
                       (config[0x11 + i * 4] << 8) |
                       (config[0x12 + i * 4] << 16) |
                       (config[0x13 + i * 4] << 24);
        if (bar) {
            print_colored(COLOR_MAGENTA, "  BAR%d: 0x%08x\n", i, bar);
        }
    }

    // If the device is a bridge (Header Type 0x01), decode bridge-specific fields
    if (header_type == 0x01) {
        uint8_t primary_bus = config[0x18];
        uint8_t secondary_bus = config[0x19];
        uint8_t subordinate_bus = config[0x1A];
        print_colored(COLOR_YELLOW, "  Primary Bus: %02x\n", primary_bus);
        print_colored(COLOR_YELLOW, "  Secondary Bus: %02x\n", secondary_bus);
        print_colored(COLOR_YELLOW, "  Subordinate Bus: %02x\n", subordinate_bus);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <pci_device_path>\n", argv[0]);
        return 1;
    }

    const char *pci_device_path = argv[1];
    uint8_t config[PCI_CONFIG_SPACE_SIZE];

    // Read PCI configuration space
    if (read_pci_config_space(pci_device_path, config, PCI_CONFIG_SPACE_SIZE) < 0) {
        fprintf(stderr, "Failed to read PCI configuration space from %s\n", pci_device_path);
        return 1;
    }

    // Decode PCI configuration space
    decode_pci_config_space(config);
    print_config_space(config);
    return 0;
}