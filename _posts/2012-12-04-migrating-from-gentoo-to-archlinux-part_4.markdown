---
layout: post
title: "Migrating from Gentoo to Archlinux (part 2/3)"
date: 2012-12-04 19:01:00+08:00
categories: linux blogspot
---

In the [previous post](/blog/2012/12/01/migrating-from-gentoo-to-archlinux-part/), I explain how I got tired of Gentoo. The obvious distribution to switch to, for a Linux "geek" like me, is Archlinux: A flexible distribution that gives a lot of power to the user, bleeding edge packages, without the hassle of having to recompile everything.

### Requirements

I found several pages on how to migrate from Gentoo to Archlinux, but nothing that fits my requirements:

- Minimum downtime. I only have one computer, so I want to be able to use Gentoo up to the point where Archlinux is ready to boot, fully functional (or almost), with all the packages I need.
- No reformatting: I want to keep my Gentoo install in case something goes wrong
- No installing on another partition: First, I don't have any free partition, and resizing partitions requires some downtime.

The official Archlinux wiki page ([Install from Existing Linux](https://wiki.archlinux.org/index.php/Install_from_Existing_Linux)) doesn't explain fully how to switch to the new system. [This guy](https://bbs.archlinux.org/viewtopic.php?id=93679&p=1) apparently installed Archlinux on top on Gentoo, replacing files as needed, but that doesn't meet my requirement of being able to roll back if things go wrong, and it seems a bit messy.

So, here is what I did:

- **Install Archlinux in a chroot** (`/arch`), on the same partition as my existing Gentoo root filesystem.
- **Install all the packages I need**, on my Archlinux chroot install.
- **Configure** the Archlinux system.
- **Reboot on a bootable Linux** CD/memory stick, move the Gentoo system from `/` to `/gentoo`, then move in the Archlinux one from `/arch` to `/`.
- **Reboot, enjoy Archlinux**.

I will only highlight the key parts of the process here. I assume that if you know how to run Gentoo, and you're willing to switch to Archlinux, you should know enough about Linux to figure out the rest (but feel free to ask ,-)).

### Install Archlinux in a chroot

So, here is what I did: I created a `/arch` folder in my root partition, and followed the instructions on this [page](http://ohnopub.net/~ohnobinki/gentoo/arch/) (replace all instances of `/var/lib/arch` with `/arch`). I believe that the instructions on the Archlinux wiki also work ([Install from Existing Linux](https://wiki.archlinux.org/index.php/Install_from_Existing_Linux)), try it out if you can, and let me know.

You now have a base Archlinux system in `/arch`: time to install the packages you need.

Open `/var/lib/portage/world` on your Gentoo system. It should look like this:

```
media-gfx/gimp
media-video/mplayer
net-im/pidgin
net-im/skype
x11-base/xorg-server
[...]
```

Look at every single package. Those are all the packages that you explicitly installed on your Gentoo system, through a command like:

```
# emerge -a package
```

This list doesn't include dependencies, but that should be fine, Archlinux knows how to install the required dependencies.

For each of these packages, first, think if you still need it (an OS change is a good opportunity for some cleanup ,-)), and if you do need it, run, **in your Archlinux chroot** (you will overwrite your Gentoo system if you forget that part):

```
pacman -S package
```

There is no 1:1 name correspondence between packages in Gentoo and Archlinux. Some have the same name (gcc, firefox, etc.), some may not. In this case, a quick Google search will tell you the package name in Archlinux.

### Install missing packages

For some Gentoo packages, you may not find an equivalent in the official repositories. However, there is the great [AUR](https://aur.archlinux.org/), which has tons of user-contributed unofficial packages. AUR package descriptions are very similar to Gentoo ebuilds: they are recipes describing how to build the package from source.

Installing these packages manually is a bit of a pain, so I recommend that you install [yaourt](https://wiki.archlinux.org/index.php/Yaourt). Yaourt plays a role similar to emerge: it fetches the AUR descriptions, compiles the package, then installs it on your system.

> After installing yaourt, think about running this command:
> ```
> # yaourt -S yaourt package-query
> ```
> This will make sure that yaourt is updated automatically when you update your system with:
> ```
> # yaourt -Syu --aur
> ```

Now, you can install missing packages by running either of these commands:

```
# yaourt -S package
# yaourt package
```

The second one searches for packages that contain "package" in their description as well.

All for now, [next post](/blog/2012/12/07/migrating-from-gentoo-to-archlinux-part_7/) will concentrate on configuring Archlinux, and rebooting on your new system.
