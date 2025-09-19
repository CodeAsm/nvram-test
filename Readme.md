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

dump the first 128 bytes of what should be ram:
```bash
./nvramdump  0xDC000000 128
nvramdump 0.2 - Dump NVRAM content from physical memory
Dump of 128 bytes @ 0xDC000000:

DC000000  FF 00 55 10 00 95 55 71 15 DD 95 15 21 28 D5 1A   ..U...Uq....!(..
DC000010  0D D4 54 80 48 F7 05 40 49 66 2F 03 BD 93 15 20   ..T.H..@If/.... 
DC000020  74 FC 77 55 11 5F 15 D4 1C 19 01 53 1F C8 15 05   t.wU._.....S....
DC000030  70 21 55 50 04 C7 5D 6C 01 11 44 15 60 97 05 85   p!UP..]l..D.`...
DC000040  B4 A8 E4 E1 DD 8C F1 A8 B5 82 F4 E1 7D 28 D1 A4   ............}(..
DC000050  E5 22 B1 F2 20 E5 E0 E0 A8 CB 22 F3 E1 C9 B5 B5   .".. .....".....
DC000060  A8 40 E1 E9 67 A8 F5 A4 A9 B9 B2 B0 30 CB C0 F3   .@..g.......0...
DC000070  95 80 C0 60 B2 68 E0 E4 63 C9 B8 A0 A4 72 8D E4   ...`.h..c....r..
```


### 2. `nvramwrite`
1. **Direct Byte Writing**:
   - The program writes a sequence of bytes specified as a hex string to the memory region.
   - Example:
     ```bash
     sudo ./nvramwrite 0xDC000000 16 FF00AABBCCDDEEFF1122334455667788
     ```

   - Or better, a tested example:
      ```bash
      ./nvramwrite 0xDC000000 4 deadbeef
      ```
      and when you read now, you get (or should):
      ```bash
      ./nvramdump  0xDC000000 128
      nvramdump 0.2 - Dump NVRAM content from physical memory
      Dump of 128 bytes @ 0xDC000000:
      
      DC000000  DE AD BE EF 00 00 0E 00 15 DD 95 15 21 28 D5 1A   ............!(..
      DC000010  0D D4 54 80 48 F7 05 40 49 66 2F 03 BD 93 15 20   ..T.H..@If/.... 
      DC000020  74 FC 77 55 11 5F 15 D4 1C 19 01 53 1F C8 15 05   t.wU._.....S....
      DC000030  70 21 55 50 04 C7 5D 6C 01 11 44 15 60 97 05 85   p!UP..]l..D.`...
      DC000040  B4 A8 E4 E1 DD 8C F1 A8 B5 82 F4 E1 7D 28 D1 A4   ............}(..
      DC000050  E5 22 B1 F2 20 E5 E0 E0 A8 CB 22 F3 E1 C9 B5 B5   .".. .....".....
      DC000060  A8 40 E1 E9 67 A8 F5 A4 A9 B9 B2 B0 30 CB C0 F3   .@..g.......0...
      DC000070  95 80 C0 60 B2 68 E0 E4 63 C9 B8 A0 A4 72 8D E4   ...`.h..c....r..
      ```

2. **Block Wiping**:
   - The program wipes the memory region with a repeating byte pattern.
   - Example:
     ```bash
     sudo ./nvramwrite 0xDC000000 4096 --wipe 0xFF
     ```

3. **File Writing**:
   - The program writes the contents of a file to the memory region.
   - Example:
     ```bash
     sudo ./nvramwrite 0xDC000000 4096 --write myfile.bin
     ```

### 3. `explore`
Explore PCI devices and print their configuration space.
Your probably better of using lspci (with -v flags or -xxx). 

```bash
./explore <pci_device_path>
```

Example:
```bash
./explore /proc/bus/pci/00/14.0
```

Example output:
```
Decoded PCI Configuration Space:
	Vendor ID: 1275
	Device ID: 0001
	Command: 0007
	Status: 0280
	Class Code: 06
	Subclass: 80
	Cache Line Size: 00
	Latency Timer: 20 (32 PCI clock cycles)
	Header Type: 00
	This is a single-function device.
	BAR0: 0xe3001000
	BAR1: 0x0000e001
	BAR2: 0xdc000000
PCI Configuration Space:

		00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
    00: 75 12 01 00 07 00 80 02 00 00 80 06 00 20 00 00 
    10: 00 10 00 e3 01 e0 00 00 00 00 00 dc 00 00 00 00 
    20: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
    30: 00 00 00 00 00 00 00 00 00 00 00 00 0c 01 00 00 
    40: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
    50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 ...
```

## Transferring Files
If you are working on a modern system and need to transfer files to the older system:
1. Use `scp` to copy files:
   ```bash
   scp <file> user@<target_ip>:/path/to/destination
   ```
2. Alternatively, use a USB drive or other removable media.

## Resources and attribution

Sofar sadly chatgpt has been a great help, but besides that trained model machine BIG thanks to:

- https://unix.stackexchange.com/questions/66830/are-there-any-source-apt-repositories-for-debian-lenny
- https://ally-petitt.com/en/posts/2024-07-05_emulating-with-nvram/ altho not used, very inspirational for further work or different work if you thought my project would be helpfull but isnt. then sure take a look here
- 

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.
