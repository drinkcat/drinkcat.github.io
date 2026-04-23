---
layout: post
title: "Linux DDC/CI control"
date: 2004-07-13 00:00:00+00:00
categories: linux
---

*(Note: imported verbatim from old website)*

*Disclaimer: Use the program I present here at your own risk, I'm not responsible if it destroys your computer, monitor or anything else.*

**The informations on this page are old. Newer versions can be found on the ~~[ddccontrol SourceForge project](http://sourceforge.net/projects/ddccontrol)~~ [github project](https://github.com/ddccontrol/ddccontrol) (not maintained by me anymore).**

On these pages, you can download two programs to control your monitor using DDC/CI, under Linux (a Gnome version, and a command-line version).

## DDC/CI

DDC/CI is a protocol designed by VESA to configure monitor by software, which means without using the OSD (On Screen Display) and the buttons on the monitor.

All the software using this protocol (NEC/Mitsubishi NaviSet, Samsung MagicTune) only runs on Windows, then I decided to make a tool for Linux.

This protocol specifications are not free, so I reverse-engineered these using this [method](/blog/2004/07/12/ddcci-reverse-engineering-howto/). You can find the part of the DDC/CI specifications that I discovered [here](/blog/2004/07/08/ddcci-specifications/).

## Download

You can download the GPLed source code of these tools here:

- Version 0.2 (July 11 2004): [ddccontrol-0.2.tar.gz](https://boichat.ch/nicolas/ddcci/ddccontrol-0.2.tar.gz).
- Version 0.1 (July 8 2004): [ddccontrol-0.1.tar.gz](https://boichat.ch/nicolas/ddcci/ddccontrol-0.1.tar.gz).

For compatibility issues, see [Supported Hardware](#supported-hardware) below.

## Install Notes

First, configure the application:

```
./configure --prefix=/usr
```

Then type:

```
make
```

and finally, as root:

```
make install
```

Then check your kernel is well configured (see below, [Kernel drivers](#kernel-drivers)), and load i2c-dev and your card-specific framebuffer driver (radeonfb, rivafb for example) using this command (you need to be logged as root):

```
# modprobe i2c-dev

# modprobe rivafb
or
# modprobe radeonfb
```

If you want users, and not only the root user, to use ddccontrol, change permissions on /dev/i2c/* (DDC/CI control need read-write permissions).

## Gnome GUI

If GTK+-2.0 is installed on your computer, the configure process will detect it, and build the Gnome application.

The Gnome GUI is very user-friendly, simply launch it using:

```
gddccontrol
```

## Command-line control

The command-line control is absolutely not user-friendly, but should work anyway.

### Usage

```
ddccontrol [-p] [-b #] [-i 0xii [-v [0xvv]]]
```

- `-i 0xii`: read current value at index 0x*ii* (see [specifications](/blog/2004/07/08/ddcci-specifications/) for a table of indexes).
- `-i 0x`*ii*` -v 0x`*vv*: write the new value (0x*vv*), at index 0x*ii*.

Options you shouldn't need to use:

- `-p`: probe all I2C buses for a monitor
- `-b #`: use bus number #

### Example

On my Mitsubishi Diamond Pro 2060u, I read the red value using (comments under brackets):

```
nicolas@tom ddccontrol $ ddccontrol -i 0x16 [reading index 0x16, which is red level, see specs]
DDC/CI control 0.1, (c) Nicolas Boichat 2004
Probing bus 0...Could not write all bytes.
Probably not a DDC bus (error : 11). [bus 0 is not the good bus]
Probing bus 1...OK. [bus 1 works fine]
Manufacturer: MEL [Mitsubishi Electronics]
Model: 4513
Serial: 2881
Date: Year 2001 Week 42 [various informations...]
Current value : 0x16: 95/fe (58%) [current value is 0x95, maximum is 0xfe, if you divide the current value by the maximum, you obtain 58%, which could be read using the OSD]
```

To set a value, simply type:

```
nicolas@tom ddccontrol $ ddccontrol -i 0x16 -v 0xfe [set the red value to the maximum]
DDC/CI control 0.1, (c) Nicolas Boichat 2004
Probing bus 0...Could not write all bytes.
Probably not a DDC bus (error : 11).
Probing bus 1...OK.
Manufacturer: MEL
Model: 4513
Serial: 2881
Date: Year 2001 Week 42
Old value : 0x16: 95/fe (58%)
New value : 0x16: fe/fe (100%) [the new value has been set correctly]
```

The screen now looks ugly, let's set it back to the previous value by typing:

```
nicolas@tom ddccontrol $ ddccontrol -i 0x16 -v 0x95 [put the red value to the initial value 0x95/0xfe = 58%]
```

And your screen looks like before!

## Kernel drivers {#kernel-drivers}

*Note: All the instruction I give here should work with the linux kernel 2.6.7 or greater. If you are using an older one, please upgrade to the [latest version](http://www.kernel.org/).*

In order to run both applications, you need to run a kernel with:

- **I2C support** enabled (see under Devices drivers->I2C support);
- **I2C device interface** enabled. If you compile it as a module, load it using:

  ```
  modprobe i2c-dev
  ```

- **Framebuffer support** enabled (Devices drivers->Graphics support)
- and a specific driver for your graphic card (see below).

#### ATI Radeon notes

*(not tested yet)*

Simply build your kernel with **ATI Radeon display support** and **DDC/I2C for ATI Radeon support** (Devices drivers->Graphics support).

Load this driver using:

```
modprobe radeonfb
```

#### NVIDIA notes

If you're using the kernel 2.6.7, apply kernel-patches/riva-2.6.7.patch to your kernel sources (this patch is a part of kernel 2.6.7 to 2.6.7-mm6 patch). Then apply kernel-patches/riva-2.6.7-mm6.patch (fix a problem with I2C bus in riva driver).

Note: if you are using kernel 2.6.7-mm6 or greater, or kernel-2.6.8-rc1 or greater, you don't need to install the first patch. If you are using kernel-2.6.8-rc1-mm1 or greater, you need to apply none of these patches.

Then build your kernel with **nVidia Riva support** and **Enable DDC Support** (Devices drivers->Graphics support).

Load this driver using:

```
modprobe rivafb
```

#### Others cards

Matrox millenium framebuffer driver should also work (module name: matroxfb).

For other cards, it could work if a framebuffer driver exists for your card, and if it supports I2C bus.

## Supported Hardware {#supported-hardware}

This list is of course incomplete, please contact me if this software also works with other graphic cards and monitors.

### Graphic Cards

Working:

- nVidia Geforce 3 Ti200 (by Hercules)

Not working:

- nVidia nForce2 GPU (on a Shuttle SN41G2): Can't read/write anything from the I2C bus, not even the EDID.

### Monitors

Working:

- Mitsubishi Diamond Pro 2060u

---

*DDC is a trademark owned by [VESA](http://www.vesa.org/) (Video Electronics Standard Association).*

*I2C is a trademark owned by Philips.*
