---
layout: post
title: "OpenCL on the Samsung Chromebook ARM, under crouton"
date: 2013-11-23 18:27:00+08:00
categories: linux blogspot
---

In this post we're going to look into OpenCL development on the [Samsung Chromebook ARM](/blog/2013/06/02/chromebook/), using crouton, and turn your Chromebook into a tiny supercomputer for CFD or bioinformatics.

ARM recently posted some [instructions about OpenCL development on the Chromebook ARM](http://malideveloper.arm.com/develop-for-mali/features/graphics-and-compute-development-on-samsung-chromebook/), but these require creating a separate Ubuntu installation that you boot off a USB drive, and are quite lenghty. The instructions I provide here are very simple: They make use of [crouton](https://github.com/dnschneid/crouton), that allows you to run Chrome OS and Ubuntu in parallel, so that you can develop OpenCL applications without rebooting.

### Download required files

Download the following files, and put them in Chrome OS `Downloads` folder:

- [crouton](https://github.com/dnschneid/crouton): Read the information there if you do not know (yet) what crouton is all about. One important note, crouton requires that you switch your Chromebook to developer mode, which wipes all the data on the Chromebook (but that's required by the ARM approach as well).
- Chrome OS does not provide an OpenCL driver, but you can get it from [this page](http://malideveloper.arm.com/develop-for-mali/drivers/mali-t6xx-gpu-user-space-drivers/): Mali Binary User Space Driver, Linux r3p0-02rel0 (22nd October 2013), X11 version. You can use this [direct link](http://malideveloper.arm.com/downloads/drivers/binary/r3p0-02rel0/linux-x11-hf-r3p0-02rel0.tgz) to get the tarball. Luckily, the newer userspace drivers work with the current kernel drivers in Chrome OS.
- [ARM Mali OpenCL SDK](http://malideveloper.arm.com/develop-for-mali/sdk/mali-opencl-sdk/), so you can test some pretty examples.

### Create crouton chroot

We are going to create a `precise` chroot (Ubuntu 12.04, but other releases should also work). You need at least the `x11` target to be installed, as Mali libraries depend on X11 libraries. This is actually an artificial dependency: OpenCL itself does not require X11, but the ARM Mali library is a single blob that provides OpenCL and OpenGL ES, and the latter depends on X11 libraries.

We're going to install `xfce`, as it pulls in the `x11` target, and gives you a nice and light desktop environment.

Open a crosh shell, and create the chroot:

```
sudo sh -e crouton -r precise -t xfce
```

This will take a while, depending on your Internet connection speed.

Then start XFCE:

```
sudo startxfce4 -n precise
```

Alternatively, if you'd rather stay in the command line, you can simply type:

```
sudo enter-chroot -n precise
```

### Install OpenCL userspace drivers

Then, inside the chroot (if you have started XFCE, open a terminal), extract the Mali libraries:

```
mkdir -p ~/mali/lib
tar xvf ~/Downloads/linux-x11-hf-r3p0-02rel0.tgz -C ~/mali/lib/
```

You also need to install 2 extra dependencies:

```
sudo apt-get install libxcb-dri2-0 libxcb-render0
```

The libraries that we need to run OpenCL code, in particular `libOpenCL.so`, are now located in `~/mali/lib`. We could install these in `/usr/lib` or `/usr/local/lib`, so that the dynamic linker could find them automatically. However, in this case, we prefer setting an environment variable, in order to avoid touching the original Ubuntu filesystem:

```
export LD_LIBRARY_PATH=$HOME/mali/lib
```

### Try out an example from Mali OpenCL SDK

Let's try an example from the SDK. First, you need to install some essential tools:

```
sudo apt-get install make g++
```

Then, extract the SDK:

```
cd ~/mali
tar xvf /home/nicolas/Downloads/Mali_OpenCL_SDK_v1.1.0.0a36a7_Linux.tar.gz
cd Mali_OpenCL_SDK_v1.1.0
```

Now modify `platform.mk`. We are not doing any cross-compilation here, so `CC` and `AR` can simply be set as:

```
CC:=g++
AR=ar
```

You can pick any example in the `samples` directory, but `mandelbrot` is probably the most "spectacular". First compile it:

```
cd samples/mandelbrot
make
```

Then run it:

```
./mandelbrot
```

The output is quite verbose, I'm yet to figure out how to make use of instrumentation and debugging features of the Mali OpenCL library:

```
[PLUGIN INFO] Plugin initializing
[PLUGIN DEBUG]  './override.instr_config' not found, trying to open the process config file
[PLUGIN DEBUG]  './mandelbrot.instr_config' not found, trying to open the default config file
[PLUGIN ERROR] Couldn't open default config file './default.instr_config'.
[PLUGIN INFO] No configuration file found, attempting to use environment
[PLUGIN INFO] CINSTR GENERAL: Output directory set to: .
[PLUGIN INFO] No instrumentation features requested.
Profiling information:
Queued time:    0.115ms
Wait time:      0.605292ms
Run time:       2218.38ms
```

In exchange for 2 seconds of GPU time, you get a nice Mandelbrot fractal in `output.bmp`.

{% include img.html src="/images/opencl-on-samsung-chromebook-arm-under/output.png" alt="Mandelbrot fractal rendered on Mali T604 GPU" %}

That's it! The [next post](/blog/2013/12/01/opencl-on-samsung-chromebook-arm/) will focus on some benchmarks, and comparison with a x86 laptop GPU and OpenMP implementations.

---

*Imported from "[Random ideas of a drinking cat](https://drinkcat.blogspot.com/)" blog.*
