# NVRAM Tools for (NetApp) PCI Devices

This repository contains tools to interact with NVRAM on older NetApp PCI devices. These tools were developed and tested on an older VIA motherboard with a PCI slot, running Debian Lenny. This repository is very experimental. 
The tools include:

1. **nvramwrite**: Write data to NVRAM.
2. **nvramdump**: Dump the contents of NVRAM.
3. **explore**: Explore PCI devices and their configuration space.

## Warning: Compatibility
These tools are designed for older hardware and software environments. You may need to use an older system or virtual machine to run these tools.

## Prerequisites
1. A system with a PCI slot and `/dev/mem` access.
2. Debian Lenny or a similar older Linux distribution. (or skills, and less issues)
3. Basic knowledge of transferring files to and from the target system.

## Building the Tools
1. **Set up the system:**
   - Add the following to `/etc/apt/sources.list`:
     ```
     deb http://archive.debian.org/debian/ lenny main contrib non-free
     deb http://archive.debian.org/debian-security/ lenny/updates main contrib non-free
     ```
   - Update the package list:
     ```bash
     apt-get update
     ```
   - Install the required packages:
     ```bash
     sudo apt-get install gcc make build-essential manpages-dev libc6-dev libpci-dev
     ```

2. **Build the tools:**
   - Compile `nvramwrite`:
     ```bash
     gcc  -o nvramwrite -std=c99
     ```
   - Compile `nvramdump`:
     ```bash
     gcc  -o nvramdump -std=c99
     ```
   - Compile `explore`:
     ```bash
     gcc  -o explore -lpci -std=c99
     ```

## Usage
### 1. `nvramdump`

Dump the contents of NVRAM from a specific physical address.

```bash
./nvramdump <phys_addr_hex> <length>
```

Example:
```bash
./nvramdump 0xDC000000 67108864
```
```bash
./nvramdump 0xe3001000 64
```

### 2. `nvramwrite`
Write data to NVRAM at a specific physical address.

```bash
./nvramwrite <phys_addr_hex> <data_hex> <length>
```
Example:
```bash
./nvramwrite 0xe3001000 64 8
```

### 3. `explore`
Explore PCI devices and print their configuration space.
Your probably better of using lspci (with -v flags or -xxx). 

```bash
./explore
```

Example output:
```
NetApp PCI Device Found:
  Bus: 00, Device: 14, Function: 00
  Vendor ID: 1275, Device ID: 0001
PCI Configuration Space:
...
```

## Transferring Files
If you are working on a modern system and need to transfer files to the older system:
1. Use `scp` to copy files:
   ```bash
   scp <file> user@<target_ip>:/path/to/destination
   ```
2. Alternatively, use a USB drive or other removable media.

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.
