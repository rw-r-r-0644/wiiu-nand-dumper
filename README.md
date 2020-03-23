# wiiu-nand-dumper
**A simple wiiu slc/slccmpt/otp/seeprom dumper**

### Getting wiiu-nand-dumper
##### Prebuilt Download
A precompiled fw.img is available . Don't think about it too much. Once you've got that, jump down to "Setup" and keep reading.

##### Compiling
First, edit `castify.py` to add the Starbuck key/IV - this is required to work with current CFW loaders. Then, just run make. You'll need devkitPPC, devkitARM, and armips - you should have all these if you've compiled many CFWs before; I won't go into it here. You should end up with a fw.img, along with a bunch of ELF files.

### Setup
You'll need an SD card normally readable by the Wii U. Simply copy the fw.img to the root of the SD.

### Running
Once you've done everything in the Setup stage, just run your favorite CFW booter (CBHC works, as does the old wiiubru loader). With any luck, wiiu-nand-dumper will start the nand dump process.

### Advantages over other NAND dumpers
Really not much. The main difference is that this dumper does not rely on IOSU to function, it's a standalone firmware that can work in a couple more particoular situations (deatached ahci drive, possibly other hardware modifications). Also it has a progress bar :P

### Credits
- This tool is based on [linux-loader](https://gitlab.com/linux-wiiu/linux-loader) and only applies minor changes  
- linux-loader is based on [minute](https://github.com/Dazzozo/minute)  
