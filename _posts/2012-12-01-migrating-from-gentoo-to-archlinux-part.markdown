---
layout: post
title: "Migrating from Gentoo to Archlinux (part 1/3)"
date: 2012-12-01 16:15:00+08:00
categories: linux blogspot
---

Alright. I've been a Gentoo user for close to 10 years. 2 months ago, I decided to do a system upgrade, and I got something like this (actually, not my own output, I copied it from [there](http://forums.gentoo.org/viewtopic-t-935644-start-0.html), but I got very similar output, every time I upgraded for the past few months):

```
# emerge -auDNvt world
[...]
Total: 198 packages (115 upgrades, 1 downgrade, 71 new, 7 in new slots, 4 reinstalls, 2 uninstalls), Size of downloads: 243,450 kB
Conflict: 9 blocks (2 unsatisfied)

 * Error: The above package list contains packages which cannot be
 * installed at the same time on the same system.

  (sys-libs/ldb-1.1.4::gentoo, ebuild scheduled for merge) pulled in by
    sys-libs/ldb required by (net-fs/samba-3.6.7::gentoo, ebuild scheduled for merge)
[...]
The following keyword changes are necessary to proceed:
[...]
The following USE changes are necessary to proceed:
#required by net-fs/samba-3.6.7[client,ads], required by
net-analyzer/nagios-plugins-1.4.16-r2[samba], required by
net-analyzer/nrpe-2.13-r2, required by @selected, required by @world (argument)
=net-fs/cifs-utils-5.4 ads
Use --autounmask-write to write changes to config files (honoring CONFIG_PROTECT).

!!! The following updates are masked by LICENSE changes:
- dev-java/sun-jdk-1.6.0.33-r1::gentoo (masked by: Oracle-BCLA-JavaSE license(s))
A copy of the 'Oracle-BCLA-JavaSE' license is located at '/usr/portage/licenses/Oracle-BCLA-JavaSE'.
```

You get the idea. 200 packages, probably a full night of compilation (on a decent dual-core laptop). Some of these packages will invariably fail to build, so I resort to using a command like:

```
# emerge --auDNvt world; emerge --skipfirst; emerge --skipfirst; emerge --skipfirst; \
  emerge --skipfirst; emerge --skipfirst; emerge --skipfirst; emerge --skipfirst; \
  emerge --skipfirst; emerge --skipfirst; emerge --skipfirst;
```

This maximises my chances of getting most packages compiled (`emerge --skipfirst` skips the first package in the previous emerge command, in case it failed).

And then, even if everything compiles fine (it never does...), I still risk libraries dependencies problems, with essential apps like firefox or mplayer not starting up:

```
mplayer: error while loading shared libraries: libx264.so.120: cannot open shared object file: No such file or directory
```

That means I need to run `revdep-rebuild`, and finally, after a day or two, I get an updated system... Great...

As long as you have plenty of free time to fix things, I guess that was fine... But I started to find that boring.

Time to make a move: in the [next post](/blog/2012/12/04/migrating-from-gentoo-to-archlinux-part_4/) I explain how I smoothly upgraded my system to Archlinux.

---

*Imported from "[Random ideas of a drinking cat](https://drinkcat.blogspot.com/)" blog.*
