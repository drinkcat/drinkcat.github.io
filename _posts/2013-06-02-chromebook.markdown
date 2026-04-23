---
layout: post
title: "Samsung ARM Chromebook"
date: 2013-06-02 19:00:00+08:00
categories: linux others blogspot
---

Since the adventure during my last trip, where [my (work/home/main) laptop suddenly died](/blog/2013/05/26/archlinux-swapping-hard-drives-between/), I realized that carrying a heavy and expensive laptop is not the best idea while travelling (in terms of risk of theft, damage, but also weight to carry around).

A tablet would be an option, but they do not offer the flexibility I want (keyboard, photo editing, etc.), and they are generally not easy to hack. [Netbooks](http://en.wikipedia.org/wiki/Netbook) seems to be a thing of the past, so I went for a [Chromebook](http://www.google.com/intl/en/chrome/devices/).

I went for the second cheapest ([Samsung ARM Chromebook](http://www.google.com/intl/en/chrome/devices/samsung-chromebook.html#ss-cb), USD 249), the cheapest being a bit too heavy and bulky ([Acer C7](http://www.google.com/intl/en/chrome/devices/acer-c7-chromebook.html#ac-c7), USD 199), and maybe less exciting with a standard x86 processor.

It is unfortunately not available in Singapore (at least not through usual channels), and some websites, such as Amazon, do not want to ship it to Singapore. Fortunately for me, [B&H Photo Video](http://www.bhphotovideo.com/) does not care, and shipped it to me for USD 52. Three days later, I received my new toy, with no GST to pay (total amount below SGD 400, see [here](http://www.customs.gov.sg/leftNav/trad/cle/Internet+PurchasesPostal+Parcels.htm)).

{% include img.html src="/images/chromebook/P1190436.JPG" alt="Samsung ARM Chromebook (running Archlinux/XFCE using chroagh, but more on that later)." %}

Well, I won't repeat what other reviews say online (there are plenty), but, basically:

- First, you pay what you get for. Don't expect it to behave like a laptop with a price tag 10 times higher (a friend said: "Fake Macbook!! Oh no, Korean!!", and, well, that's not completely inaccurate): The body is plastic, and this is not a powerhouse. But still, for the price, I find it awesome.
- Very light (1.1kg) and thin (1.8cm).
- No fan, no moving parts: No noise at all, little heat, and long battery life (~6h).
- Dual-core ARM processor (1.7Ghz): This is a bit better than your last generation cell phone (well, it does out-of-order execution so it's probably a step faster), but it seems to do the job fairly well.
- Chrome OS:
  - Very good browsing experience, very fast. Youtube works well in general, but tends to lag when you do other things at the same time: not ideal if you have your music playlist on Youtube.
  - You need connectivity for most stuff. There are a few offline apps (I haven't tried many), but most things will require a Wifi connection.
- Movie playback (from files, non-Youtube) is a problem: It could not read some x264-encoded files, even in SD quality. I mean, it could, but only displayed one frame per second or so... Maybe this could be improved in later versions of Chrome OS by optimizing the video decoder (e.g. use whatever hardware acceleration facilities the processor has).
- Finally, and this is critical for me: hackability. This is not an Apple device or a locked Android phone. Google lets you do whatever you want with your computer, including wiping Chrome OS to install something else, or installing another Linux in parallel ([crouton](https://github.com/dnschneid/crouton)/[chroagh](https://github.com/drinkcat/chroagh)).

In short, mildly recommended for the normal user, Chrome OS is great, but really needs a working connection to be used to its full potential. And this is not always the case while travelling.

Strongly recommended for the more advanced user, as you can install a regular Linux in parallel: either as Dual-boot, or as a chroot inside Chrome OS, where you can switch between Chrome OS and your Linux with a simple key combination. I used the wonderful [crouton](https://github.com/dnschneid/crouton) to install Ubuntu, and ported it to support Archlinux ([chroagh](https://github.com/drinkcat/chroagh)). More on that for a later post.

---

*Imported from "[Random ideas of a drinking cat](https://drinkcat.blogspot.com/)" blog.*
