---
layout: post
title: "Archlinux: swapping hard drives between laptops"
date: 2013-05-26 19:00:00+08:00
categories: linux blogspot
---

My good old Dell Lattitude E6400 didn't survive my last week-end trip to Vietnam...

For no apparent reason, the power button stays on for a few seconds, then switches off. The num lock light blinks, indicating a processor error... Luckily, I bought a 4th year of warranty, which would expire in 2 months time. Called up Dell on Tuesday, they came on Thursday (next business day), swapped the logic board, and it works again!

In the mean time, I found a Lenovo Thinkpad T410 at the office, which is roughly as old as my Dell (and the keyboard is covered by something that looks like mould, but that's another story). I could use the Windows installation on that computer, but I suffer from a strong allergy to Windows, so I decided to do something slightly smarter instead of risking mental breakdown.

Both the E6400 and the T410 are good professional laptops, and the hard drives can be taken out in less than 5 minutes - read: those are not Macs where you need to dismantle the whole computer to swap a component, only 5 screws need to be removed on each computer (1 to take out the hard drive assembly, and 4 to remove the drive itself).

Anyway, took out the T410 hard drive, replaced it with my drive from the E6400, booted the T410, and immediately got my Archlinux running, with X server, network, and everything. The fact that both computers have similar hardware, and both use a nVidia graphics card, made the task slightly easier (no extra driver to install or xorg.conf to reconfigure), and the new screen resolution was automatically detected (the T410 features a pathetic 1280x800, vs a slightly better 1440x900 for the E6400).

Now, just for a minute, imagine how complicated it would have been to do the same with a Windows system (re-installation? problems with key activation?).

Anyway, that got me thinking that I should carry such a big laptop on holidays, so I ordered a [Samsung Chromebook](http://www.samsung.com/us/computer/chromebook), with the idea of install Archlinux on it. And I will soon need a new laptop to replace my E6400...

Just one picture of my trip to Ho Chi Minh City:

{% include img.html src="/images/archlinux-swapping-hard-drives-between/P1190243.JPG" alt="Somewhere in Saigon: Doll waiting to cross the road. Or celebrating Reunification day, not sure..." %}

---

*Imported from "[Random ideas of a drinking cat](https://drinkcat.blogspot.com/)" blog.*
