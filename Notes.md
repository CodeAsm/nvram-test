# NetApp pci nvram

```bash
 lspci -s 00:14.0 -vv
00:14.0 Bridge: Network Appliance Corporation Device 0001
        Control: I/O+ Mem+ BusMaster+ SpecCycle- MemWINV- VGASnoop- ParErr- Stepping- SERR- FastB2B- DisINTx-
        Status: Cap- 66MHz- UDF- FastB2B+ ParErr- DEVSEL=medium >TAbort- <TAbort- <MAbort- >SERR- <PERR- INTx-
        Latency: 32
        Interrupt: pin A routed to IRQ 12
        Region 0: Memory at e3001000 (32-bit, non-prefetchable) [size=128]
        Region 1: I/O ports at e000 [size=128]
        Region 2: Memory at dc000000 (32-bit, non-prefetchable) [size=64M]
        Expansion ROM at 40000000 [disabled] [size=4K]

debian:/home/user# lspci -s 00:14.0 -xxx
00:14.0 Bridge: Network Appliance Corporation Device 0001
00: 75 12 01 00 07 00 80 02 00 00 80 06 00 20 00 00
10: 00 10 00 e3 01 e0 00 00 00 00 00 dc 00 00 00 00
20: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
30: 00 00 00 40 00 00 00 00 00 00 00 00 0c 01 00 00
40: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
60: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
70: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
90: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
```

## explore.c
```bash
sudo apt-get install make gcc libc6-dev
sudo apt-get install libpci-dev
gcc explore.c -o explore -lpci -std=c99
```
```bash
./explore 
NetApp PCI Device Found:
  Bus: 00, Device: 14, Function: 00
  Vendor ID: 1275, Device ID: 0001
PCI Configuration Space:

00: 75 12 01 00 07 00 80 02 00 00 80 06 00 20 00 00 
10: 00 10 00 e3 01 e0 00 00 00 00 00 dc 00 00 00 00 
20: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
30: 00 00 00 40 00 00 00 00 00 00 00 00 0c 01 00 00 
40: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
60: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
70: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
90: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
```

### 
Na isntalatie van nog eens 2 sticks. 
```bash
00: 75 12 01 00 07 00 80 02 00 00 80 06 00 20 00 00 
10: 00 10 00 e3 01 e0 00 00 00 00 00 dc 00 00 00 00 
20: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
30: 00 00 00 00 00 00 00 00 00 00 00 00 0c 01 00 00 
40: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
60: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
70: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
90: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
```

```bash
lspci -s 00:14.0 -vv
00:14.0 Bridge: Network Appliance Corporation Device 0001
        Control: I/O+ Mem+ BusMaster+ SpecCycle- MemWINV- VGASnoop- ParErr- Stepping- SERR- FastB2B- DisINTx-
        Status: Cap- 66MHz- UDF- FastB2B+ ParErr- DEVSEL=medium >TAbort- <TAbort- <MAbort- >SERR- <PERR- INTx-
        Latency: 32
        Interrupt: pin A routed to IRQ 12
        Region 0: Memory at e3001000 (32-bit, non-prefetchable) [size=128]
        Region 1: I/O ports at e000 [size=128]
        Region 2: Memory at dc000000 (32-bit, non-prefetchable) [size=64M]
        [virtual] Expansion ROM at 40000000 [disabled] [size=4K]
```

## nvramdump
met 2 stcisk
```bash
/nvramdump 0xe3001000 64
Dump van 64 bytes @ 0xE3001000:

E3001000  00 00 00 FC 01 00 00 00 00 00 00 00 00 00 00 00 
E3001010  00 F0 FF FF 00 00 00 00 C3 00 03 89 00 00 00 00 
E3001020  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
E3001030  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 

./nvramdump 0xe3001000 256
Dump van 256 bytes @ 0xE3001000:

E3001000  00 00 00 FC 01 00 00 00 00 00 00 00 00 00 00 00 
E3001010  00 F0 FF FF 00 00 00 00 C3 00 03 89 00 00 00 00 
E3001020  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
E3001030  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
E3001040  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
E3001050  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
E3001060  00 00 00 00 00 00 00 00 00 01 01 00 00 76 01 98 
E3001070  B5 10 6E 90 B5 10 6E 90 B5 10 6E 90 B5 10 6E 90 
E3001080  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF 
E3001090  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF 
E30010A0  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF 
E30010B0  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF 
E30010C0  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF 
E30010D0  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF 
E30010E0  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF 
E30010F0  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF 
```

### 4 sticks

```bash
./nvramdump 0xe3001000 128
nvramdump 0.2 - Dump NVRAM content from physical memory
Dump of 128 bytes @ 0xE3001000:

E3001000  00 00 00 FC 01 00 00 00 00 00 00 00 00 00 00 00   ................
E3001010  00 F0 FF FF 00 00 00 00 C3 00 03 89 00 00 00 00   ................
E3001020  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
E3001030  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
E3001040  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
E3001050  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
E3001060  00 00 00 00 00 00 00 00 00 01 01 00 00 76 01 98   .............v..
E3001070  B5 10 6E 90 B5 10 6E 90 B5 10 6E 90 B5 10 6E 90   ..n...n...n...n.
```


