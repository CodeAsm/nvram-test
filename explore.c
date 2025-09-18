#include <stdio.h>
#include <stdlib.h>
#include <pci/pci.h>

#define NETAPP_VENDOR_ID 0x1275
#define NETAPP_DEVICE_ID 0x0001

void print_config_space(struct pci_dev *dev, struct pci_access *pacc) {
    unsigned char config[256];
    int i; // Declare the variable outside the for loop
    pci_read_block(dev, 0, config, sizeof(config));

    printf("PCI Configuration Space:\n");
    for (i = 0; i < 256; i++) {
        if (i % 16 == 0) printf("\n%02x: ", i);
        printf("%02x ", config[i]);
    }
    printf("\n");
}

int main() {
    struct pci_access *pacc;
    struct pci_dev *dev;

    // Initialize PCI library
    pacc = pci_alloc();
    pci_init(pacc);
    pci_scan_bus(pacc);

    // Iterate through PCI devices
    for (dev = pacc->devices; dev; dev = dev->next) {
        pci_fill_info(dev, PCI_FILL_IDENT | PCI_FILL_BASES | PCI_FILL_CLASS);

        // Check for NetApp device
        if (dev->vendor_id == NETAPP_VENDOR_ID && dev->device_id == NETAPP_DEVICE_ID) {
            printf("NetApp PCI Device Found:\n");
            printf("  Bus: %02x, Device: %02x, Function: %02x\n", dev->bus, dev->dev, dev->func);
            printf("  Vendor ID: %04x, Device ID: %04x\n", dev->vendor_id, dev->device_id);

            // Print configuration space
            print_config_space(dev, pacc);
        }
    }

    // Cleanup
    pci_cleanup(pacc);
    return 0;
}