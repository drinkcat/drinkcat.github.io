---
layout: post
title: "Running Chromium OS in QEMU"
date: 2013-10-06 19:00:00+08:00
categories: linux blogspot
---

If you'd like to have a taste of Chrome/Chromium OS before buying an actual Chromebook, the recommended way used to be to download the [Hexxeh images](http://chromeos.hexxeh.net/), and run them in a virtual machine (Virtualbox). However, these images are now quite outdated.

The steps I mention here use development Chromium OS builds, made available by the Chromium developers. The resulting "experience" is still quite different from running Chrome OS on a proper device: there is no graphics acceleration, there are no applications installed by default, and the user interface is painfully slow. However, this is still good enough to get a general idea (or do some development).

### Fetch the image

First, go the [Chromium OS builders page](http://build.chromium.org/p/chromiumos/builders), then click the architecture you want. This following assumes you are picking `amd64 generic full` (only use `full` images, don't even try the other ones), but other architectures should also work. On the next page, choose one of the builds, for example, [`#9624`](http://build.chromium.org/p/chromiumos/builders/amd64%20generic%20full/builds/9624). Then, under step `16. Report`, click on `Artifacts`. You are presented with a list of files: download `chromiumos_qemu_image.tar.xz`.

### Extract the image

Now, let's extract the image. It is a 8GB file, but contains mostly zeros. We use dd to recreate a [sparse file](http://en.wikipedia.org/wiki/Sparse_file), to save a significant amount of disk space:

```
$ tar xvfO chromiumos_qemu_image.tar.xz chromiumos_qemu_image.bin |
dd of=chromiumos_qemu_image.bin conv=sparse
```

If you have a lot of hard drive space, you can ignore that, and simply run:

```
$ tar xvf chromiumos_qemu_image.tar.xz
```

### Start QEMU

Now, let's start QEMU:

```
$ qemu-system-x86_64 -enable-kvm -m 1024 -net nic -net user -redir tcp:9922::22 -hda chromiumos_qemu_image.bin
```

If permissions are setup properly, you do not need to run this as root. The parameters serve the following purpose:

- `-enable-kvm`: Makes sure we use hardware virtualisation extensions.
- `-m 1024`: Give 1GB of memory to Chromium OS. If you don't specify this, the default is 128MB, which is far from enough to run Chromium OS.
- `-net nic -net user`: creates a virtual ethernet interface
- `-redir tcp:9922::22`: Since those are development builds, they come with an SSH server. This redirects local port 9922 to the chroot's port 22 (more on this later)
- `-hda chromiumos_qemu_image.bin`: Use the image as hard drive.

And now you should see Chromium OS booting. In some cases, you may be unlucky, and the build you have picked may be broken (remember, those are development builds). In this case, pick an older build (2-3 days older at least), and retry.

### Access the virtual machine via SSH

This is mostly useful for developers. The SSH server only allows login via public keys (no password authentication). To setup the public key, follow the following steps:

- Setup a [SSH private/public key pair](http://www.debian.org/devel/passwordlessssh) on the host machine, if you don't have one already.
- Copy the content of `~/.ssh/id_rsa.pub` into a pastebin, Google Doc, or whatever web page you can access from the Chromium OS.
- In Chromium OS, browse to the pastebin/Google Doc, and copy the key information.
- Then press `Ctrl-Alt-T`: this opens a new crosh shell, and type:

```
> shell
$ cd
$ mkdir .ssh
$ cat > authorized_keys
```

- Then paste the content of the key, using `Ctrl-Shift-V`, and press `Ctrl-D` to terminate.

Now you can access your QEMU "Chromiumbook" from your host, using:

```
ssh chronos@127.0.0.1 -p 9922
```

And you can even mount a shared directory on the virtual machine with something like:

```
sshfs chronos@127.0.0.1:Downloads/ssh ssh.qemu -p 9922
```

If you later restart QEMU, you will need to login in Chromium OS before the steps above work again.

That's it! I find this particularly useful when I develop for [crouton](https://github.com/dnschneid/crouton)/[chroagh](https://github.com/drinkcat/chroagh), as I only have a [Samsung ARM Chromebook](/blog/2013/06/02/chromebook/). With this setup, I can test my code on `x86` and `x86_64` architectures.
