---
layout: post
title: "Migrating from Gentoo to Archlinux (part 3/3)"
date: 2012-12-07 19:02:00+08:00
categories: linux blogspot
---

In the [first part](/blog/2012/12/01/migrating-from-gentoo-to-archlinux-part/), I explained why I decided to switch away from Gentoo. In the [second](/blog/2012/12/04/migrating-from-gentoo-to-archlinux-part_4/), I explained how to install Archlinux in a chroot, and install packages identical to those on your Gentoo installation.

In this last post, I give a few hints on how to configure Archlinux, and how to switch to your new system.

### Configuring Archlinux

The general idea is the following: look at files in `/etc` on your Gentoo installation, and update the equivalent ones in `/arch/etc/`. For example, `/etc/fstab` can simply be copied over, so are most application specific files (say, `ssh/*`, `screenrc`, `hibernate/*`, `sudoers`, etc.). Do not overwrite system files (`rc.d/*`, `conf.d/*`), nor `passwd`/`shadow`/`group` files.

#### User configuration

One of the critical things is to recreate the users with the same uid/gid, so that you don't need to change file ownership on your existing filesystems.

In my case, I simply copied over the relevant lines in `shadow`, `groups`, `passwd`, but this can be done more cleanly by using the `-u` parameter on the `useradd` command, e.g.:

```
# useradd -u 1000 -g 1002 user
```

Groups may have different names and meanings on Archlinux, so have a look at the [list](https://wiki.archlinux.org/index.php/Users_and_Groups), and check what groups you need to put your users into.

Don't forget to set a root password as well.

#### X11 configuration

One of the differences between Gentoo and Archlinux, at least on my installation, is that Archlinux uses a set of files in `/etc/X11/xorg.conf.d` instead of a single `/etc/X11/xorg.conf`. But again, nothing that can't be fixed by moving parts of `xorg.conf` to the relevant files in `/etc/X11/xorg.conf.d`.

#### Do not configure the bootloader yet!

Leave that part for later. However install a kernel, and copy the relevant files (`vmlinuz-linux`, `initramfs-linux.img`) from `/arch/boot/` to `/boot`.

I still had a legacy grub bootloader (versions 0.9x) on my Gentoo install, so I just added these lines in `/etc/grub/menu.lst`:

```
title Archlinux
root (hd0,2)
kernel /boot/vmlinuz-linux root=/dev/sda9 ro nofb
initrd  /boot/initramfs-linux.img
```

(read the documentation to figure out the new syntax if you already switched to the new grub 2.x on Gentoo)

#### Follow the guide!

For the rest, the [Archlinux wiki](https://wiki.archlinux.org/) is particularly well written, look for pages on how to configure the network, start required services, etc.

### Rebooting on a rescue CD/USB stick

Now the fun part! As a live Linux system, I used [systemrescuecd](http://www.sysresccd.org/SystemRescueCd_Homepage), installed on a [USB stick](http://www.sysresccd.org/Sysresccd-manual-en_How_to_install_SystemRescueCd_on_an_USB-stick) (who uses CD anymore?). It would also come in handy in case you break everything and cannot boot your system.

Reboot from the USB stick, mount your root filesystem:

```
# mkdir /mnt/sda9 && mount /dev/sda9 /mnt/sda9
```

And simply swap the content of the root filesystem:

```
# cd /mnt/sda9
# mkdir gentoo
# mv * gentoo
```

(this will refuse to move `gentoo` inside itself, which is perfectly fine, however this also moves `arch` to `gentoo/arch`, which we don't want:)

```
# mv gentoo/arch .
```

Now move the `/arch` root filesystem to `/`:

```
# mv arch/* .
```

If `/boot` is a separate partition, you are ready to reboot. If it is not, you want to copy the old `/boot` directory:

```
# mv boot boot.arch
# mv gentoo/boot .
```

And then you can reboot.

### Enjoy Archlinux!

Provided you did everything properly, and configured all services, you should have a working Archlinux system on the next reboot!

It didn't work for me ,-) I did not configure the wireless connection properly, and did not know how to fix it without the help of the Archlinux wiki. So I rebooted on the rescue USB stick, moved back the Gentoo root filesystem, rebooted, fixed the configuration in the chroot, and back again. Easy!

If everything works well, you should [install grub from Archlinux](https://wiki.archlinux.org/index.php/Grub), so that you have a `/boot` directory that comes from Archlinux.

Finally, after making a backup of `/gentoo/etc`, just in case, you can happily:

```
rm -rf /gentoo
```

---

*Imported from "[Random ideas of a drinking cat](https://drinkcat.blogspot.com/)" blog.*
