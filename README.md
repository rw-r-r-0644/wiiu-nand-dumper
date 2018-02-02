# linux-loader
**An ARM firmware image for the Wii U, designed to work with [linux-wiiu/linux-wiiu](https://gitlab.com/linux-wiiu/linux-wiiu).**

### Compiling
First, edit `castify.py` to add the Starbuck key/IV - this is required to work with current CFW loaders. Then, just run make. You'll need devkitPPC, devkitARM, and armips - you should have all these if you've compiled many CFWs before; I won't go into it here. You should end up with a fw.img, along with a bunch of ELF files.

### Setup
You'll need an SD card normally readable by the Wii U. Simply copy the fw.img to the root of the SD, and your Linux kernel (see [linux-wiiu/linux-wiiu](https://gitlab.com/linux-wiiu/linux-wiiu)'s README for compiling) to a folder named linux, named `kernel`. That's `sd:/linux/kernel` - if you get that wrong, Linux won't boot.

### Running
Once you've done everything in the Setup stage, just run your favorite CFW booter (CBHC works, as does the old wiiubru loader). With any luck, linux-loader will start and boot up the Linux kernel.
