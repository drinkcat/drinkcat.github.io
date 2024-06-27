---
layout: post
title:  "Reverse engineering (in embedded world)"
categories: embedded software
---
The next few posts will cover reverse engineering of various pieces of
hardware. Or, to be precise, figuring out how to _interact_ with
various pieces of hardware -- hacking the embedded software running on
the device itself is often also possible, but also usually much
harder.

I've been doing this kind of things for a very long time, my main
purpose had mostly been to be able to add Linux/OSS support for
hardware that would normally only support Windows (and maybe Mac OS
X). The hardware in question could be some printer, card reader,
internal peripherals in a laptop, or so many other things.

Thankfully, on the peripheral side, standardization has been helping
a lot. For example, there are well-specified standard USB protocol
classes, making it less likely that the device uses some obscure
prioritary protocol, and more likely that a standard Linux driver
would support the device out of the box.

And additionally, Linux support from the vendor is quite a bit more
common than it used to be.

This makes these kind of projects quite a bit harder to find than they
used to be, which is, ironically, a pity, because these projects are
_fun_. Trying to understand how a protocol works, finding out if it
looks similar to some other documented protocol, is a really nice
real world puzzle. It's probably the real reason I undertook all of
them in the first place.

What I realized, much later, is that this sort of skills translates
very well when doing any kind of hardware/software debugging. In
those cases, you may have access to specs, but if the hardware
doesn't behave exactly like intended, you fall back solidly into
reverse engineering world, and you have to understand why a piece of
hardware is not behaving the way it should.

## Basic flow

The basic flow for this kind of project usually starts by acquiring
traces for the peripheral you want to analyze, using a driver provided
by the vendor, on a platform supported by the vendor (e.g. Windows).

The techniques to do so are very varied, from probing external
signals with an oscilloscope, enabling protocol tracing provided by
the OS (easy with USB and Bluetooth for example), smart uses of
debugger, to binary inspection (usually not something I'd
recommend).

Once a method has been figured out, one needs to collect data in
different scenarios (e.g., what if the printer runs out of paper,
use different apps to read the content of a smart card, etc.).

Then the fun begins. What does each of the bytes mean? Is there,
by any chance, a documented protocol somewhere out there that is
at least _somewhat_ similar to what is seen in the traces?

Then comes writing proof of concept drivers. For USB for example,
it is possible to write userspace drivers, making it very easy to
test and iterate. Typically, it might only be at that stage that
I would realize the code I'm writing is actually something that
already exists.

Finally, comes integration into production-quality software. This
could be something written from scratch, or, more commonly, adding
support for an existing piece of software (e.g. Linux kernel).
