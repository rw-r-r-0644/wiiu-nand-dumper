# linux-loader
**An ARM firmware image for the Wii U, designed to work with [linux-wiiu/linux-wiiu](https://gitlab.com/linux-wiiu/linux-wiiu).**

### Getting linux-loader
##### Prebuilt Download
A precompiled fw.img is available [here](https://gitlab.com/linux-wiiu/linux-loader/-/jobs/artifacts/master/raw/fw.img?job=master-build). Don't think about it too much. Once you've got that, jump down to "Setup" and keep reading.

##### Compiling (Docker)
The preferred method of compiling is using our Docker image. Clone or download this repository, then:
```sh
#replace with wherever you cloned this repo
cd path/to/linux-loader
#Run the image!
docker run --rm -it -v $(pwd):/linux-loader quarktheawesome/wiiu-cfw-builder
#You should now be in the docker image. Your shell prompt will change.
cd linux-loader
#Replace with the Starbuck key and IV, respectively. Make sure it's in all caps
sed s/B5XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX/STARBUCK_KEY_HERE/g -i castify.py
sed s/91XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX/STARBUCK_IV_HERE/g -i castify.py
#now, compile
make
#Once it's done, assuming no errors, go back to your main OS
exit
```
You should now find a fw.img in your linux-loader folder! Congrats. Read the "Setup" section next!

##### Compiling (from scratch, not recommended)
First, edit `castify.py` to add the Starbuck key/IV - this is required to work with current CFW loaders. Then, just run make. You'll need devkitPPC, devkitARM, and armips - you should have all these if you've compiled many CFWs before; I won't go into it here. You should end up with a fw.img, along with a bunch of ELF files.

### Setup
You'll need an SD card normally readable by the Wii U. Simply copy the fw.img to the root of the SD, and your Linux kernel (see [linux-wiiu/linux-wiiu](https://gitlab.com/linux-wiiu/linux-wiiu)'s README for compiling) to a folder named linux, named `kernel`. That's `sd:/linux/kernel` - if you get that wrong, Linux won't boot.

### Running
Once you've done everything in the Setup stage, just run your favorite CFW booter (CBHC works, as does the old wiiubru loader). With any luck, linux-loader will start and boot up the Linux kernel.
