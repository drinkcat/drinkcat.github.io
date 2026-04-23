---
layout: post
title: "Simple remote trigger for Panasonic Lumix"
date: 2012-12-16 20:14:00+08:00
categories: photo blogspot
---

In this post I show how to build a simple remote trigger for a Panasonic camera (Lumix DMC-GX1).

A remote trigger is a cable, connected to the camera on one side, with a button on the other side. The button allows you to take pictures without touching the body of the camera. This is especially useful at night, when taking pictures with a tripod: pressing the camera trigger will invariably shake the camera, leading to blurry pictures. Also, having a remote interface is interesting to automate picture taking (triggering from a laser sensor, timelapse, motion sensor, etc...), but more on that in future posts.

The "official" Panasonic remote trigger is [fairly expensive](http://www.bhphotovideo.com/c/product/459679-REG/Panasonic_DMW_RSL1_DMW_RSL1_Remote_Shutter_Release.html) (~50 USD). You can find much cheaper alternatives on ebay, but, isn't more fun to build it yourself?

The design I choose here consists of 2 parts: a cable, and a stripboard. The cable has a 2.5mm jack on one side, that will fit into the camera connector, and a simple 2-pin header on the other side, that is connected to the stripboard. The stripboard has 2 buttons: One allows to pre-focus (identical to a half-press of the camera trigger button), while the other triggers the camera to take a picture (full-press).

{% include img.html src="/images/simple-remote-trigger-for-panasonic/P1120601annot.jpg" alt="The completed remote trigger." %}

The cable part can be disconnected from the stripboard, so that it can be connected to other triggering devices (hint: Arduino!). Also, I tried to keep the stripboard as small as possible (after all, Micro 4/3 systems are supposed to be compact!).

### Pinout

The [SmaTrig website](http://www.doc-diy.net/photo/remote_pinout/) gives a lot of information on how to build a remote trigger. Most camera brands use a simple system, consisting of 3 pins: ground, focus, trigger. Shorting focus or trigger to the ground gives you the respective functions.

The pinout of the Lumix remote trigger is [a little bit special](http://www.doc-diy.net/photo/remote_pinout/#lumix) (actually, just plainly annoying...). You only have 2 pins: At rest, the resistance between the pins must be around 41.1 kOhm. The resistance drops to around 5.1 kOhm when the focus is pressed, and to about 2.2 kOhm when the trigger is pressed.

To summarise:

<table border="1" cellpadding="5px" style="border-collapse: collapse; margin: auto;">
<tbody>
<tr>
<td align="center"><b>Resistance</b></td>
<td align="center"><b>Action</b></td>
</tr>
<tr>
<td align="right">~41.1 kOhm</td>
<td>"Rest" state (used to detect the trigger presence)</td>
</tr>
<tr>
<td align="right">~5.1 kOhm</td>
<td>Pre-focus (i.e. half-press of the camera shutter button)</td>
</tr>
<tr>
<td align="right">~2.2 kOhm</td>
<td>Take picture (i.e. full-press)</td>
</tr>
</tbody></table>

There is some allowed tolerance in the values, but no one knows what they are, so you better stay as close as possible to what is in the table.

### Cable

For the cable, you need the following components:

- A 4-pin 2.5mm jack. This looks like a cell phone jack (stereo earphone+mic), but smaller (normal jacks are 3.5mm). It is a little tricky to find online (here is [one](http://www.tme.eu/en/details/jc-135/jack-connectors/#)), and I found mine at a local supplier (Sim Lim Tower). It is very difficult to solder as well. If you can, I recommend that you find a pre-soldered cable, like [this one](http://www.digikey.com/product-search/en?x=15&y=17&lang=en&site=us&KeyWords=CP-254S-M%2FM) (used on [this post](http://www.robotroom.com/Macro-Photography-2.html)).
- Some length of flexible wire, 2 conductors.
- A 2-pin female connector. I cut off 2 pins of a [40-pin 2.54mm spacing pin header](http://www.taydaelectronics.com/connectors-sockets/pin-headers/40-pin-2-54-mm-single-row-female-pin-header.html).

If you don't have a pre-soldered cable, solder the 4-pin 2.5mm jack to the cable. You only need to solder the 2 pins closest to the plastic cap (see the diagram [here](http://www.robotroom.com/Macro-Photography-2.html), then check with a multimeter).

If your soldering skills are as bad as mine, your connections will be fragile. After getting a good connection, I filled up the connector with super glue to make sure it doesn't break. (By now you must have understood that you should buy a pre-made cable ,-)

Now solder the 2-pin female connector on the other end, put some shrink tube to make it look neater and sturdier, et voilà!

{% include img.html src="/images/simple-remote-trigger-for-panasonic/P1120609x.JPG" alt="The cable." %}

### Stripboard

Now, the fun part. You will need the following components:

- A [stripboard](http://en.wikipedia.org/wiki/Stripboard) (a.k.a. revoboard). Those are useful for semi-permanent boards and do not require special equipment to make (unlike a PCB board). If you never used those before, here is a [nice tutorial](http://www.societyofrobots.com/member_tutorials/node/90).
- A 2-pin male connector. I used something [like this](http://www.taydaelectronics.com/connectors-sockets/pin-headers/40-pin-2-54-mm-angle-single-row-pin-header.html) (the plastic part can easily be pushed to give a longer connector).
- 3 resistors. Values: 2.2 kOhm, 2.9 kOhm, 29 kOhm. Those should not be too hard to find.
- 2 push buttons, with [2 long pins](http://www.taydaelectronics.com/electromechanical/switches-key-pad/tact-switch-6-6mm-5mm-through-hole-spst-no.html) each.
- Some [transparent shrink tube](http://www.hobbyking.com/hobbyking/store/__13838__Turnigy_Heat_Shrink_Tube_20mm_Transparent_1mtr_.html), about 2 cm wide (i.e. 4 cm circumference).

First, cut a piece of the stripboard. You need 3 lines, about 13 holes long. If you live in a humid place like me, sandpaper the copper side of the board: This will help remove the (insulating) copper oxide.

Make a cut on one of the copper lines, as indicated on the picture. Now start soldering the resistors (ignore the red dots for now). The resistors must be on the side of the board that does NOT have copper, so that you can solder the leads on the copper side. Also, you want to keep the resistors as close as possible to the board, so that you keep the whole thing as small as possible. You can also solder the male header.

If you look at the picture, you now have a 2.2K+2.9K+36K=41.1K resistance between the 2 pins (what we want in the "rest" state, see the table above).

{% include img.html src="/images/simple-remote-trigger-for-panasonic/trigger.png" alt="Schematics. Note: The resistors should be on the opposite side of the copper strips, that is, the cut is actually on the back side (=copper side) of the strip board." %}

Now, solder the 2 push buttons in the contacts indicated in red. The focus button shorts the 36K resistors, so you only get 5.1K resistance. The trigger button shorts both the 2.9K and the 36K resistors, so you only get 2.2K of total resistance (again, what we want).

{% include img.html src="/images/simple-remote-trigger-for-panasonic/trigger2.png" alt="Schematics with buttons." %}

It is a good idea to check with a multimeter that the resistance between the 2 pins corresponds to what you expect (some tolerance, probably around 5%, is perfectly fine).

Now, connect the cable to the camera and the stripboard, and check if you can take pictures.

Finally, when everything is working, cut a piece of shrink tube, put it around the stripboard, and gently shrink it down with a lighter. I chose a transparent piece of shrink tube, so that you can still see the circuit. The shrink tube will, to a certain extent, help protecting the circuit, as well as your hands: the resistors wire leads at the back of the circuit are cut after soldering, and can be somehow sharp.

You're done!

{% include img.html src="/images/simple-remote-trigger-for-panasonic/P1120602x.JPG" alt="The completed stripboard" %}
