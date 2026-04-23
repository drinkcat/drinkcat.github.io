---
layout: post
title: "Macro video"
date: 2013-04-04 19:00:00+08:00
categories: photo blogspot
---

<!-- TODO: update macro-photography-cheap link once 2013-03-macro-photography-cheap is migrated -->
This post is about taking macro videos, that is, videos of small objects, at relatively high magnification. In other posts, you can find a general introduction on taking [macro pictures using a close-up filter](http://drinkcat.blogspot.com/2013/03/macro-photography-cheap.html), and how to [correct chromatic aberration](/blog/2013/03/18/macro-photography-correcting-chromatic-aberration/).

This post will show you how to take videos of small objects. I used my Reuge music box as an example, since it is small, and it moves (playing some nice music in the process, of course)... The final result is shown here:

<div style="text-align: center;">
<iframe allowfullscreen="" frameborder="0" height="315" src="http://www.youtube.com/embed/5IlDgwzRR94" width="560"></iframe>
</div>

My camera, the Panasonic DMC-GX1, is in the medium range of what Panasonic offers, which, unfortunately, means that there is no manual control of aperture or white balance in video mode. There is no technical reasons for that, only commercial ones, and, well, that's a shame: there is enough feature differentiation between the DMC-GH2/GH3 and the GX1 without adding artificial limitations.

An alternative would be to use the [Ptool hacked firmware](http://www.gh1-hack.info/wiki/PToolSoftware). It allows the DMC-GF1 to use manual control in video mode. This patch is also compatible for the DMC-GX1, but, so far, only allows custom video bitrates on that camera.

This post will show you how to circumvent these limitations. To get a nice macro video, we need 3 things:

<!-- TODO: update macro-photography-cheap link once 2013-03-macro-photography-cheap is migrated -->
- Force the lens aperture to its largest value (f/22), to get a decent depth of field. This also mean we need a lot of light on the scene. For this purpose, I put a tabletop lamp right on top of my music box (a few centimeters away).
- Fix the white balance. The camera only allows auto white balance in video mode. In this case it does a particularly bad job at it, because the scene is mostly yellow, and turns everything into a grayish thing.
- [Correct chromatic aberration](/blog/2013/03/18/macro-photography-correcting-chromatic-aberration/) introduced by the [macro filter](http://drinkcat.blogspot.com/2013/03/macro-photography-cheap.html).

### Forcing lens aperture

This part is a bit tricky, and, well, do it at your own risk. I did not break neither my camera nor my lens by doing it 5-6 times, but that's no guarantee it won't break your camera/lens: I don't take any responsibility... ,-)

By default, when it is not connected to a camera, any Micro 4/3 lens is at maximum aperture (lowest f/ number). Actually, even when connected to the camera, the lens is always at maximum aperture until you press the shutter button: this allows for easier manual focus, and gives more light for the live preview on the LCD.

This following picture shows my Panasonic 45-150mm, disconnected from the camera: clearly, the aperture is wide open (f/5.6 at 150mm).

{% include img.html src="/images/macro-video/P1170873.JPG" alt="Lens in its &quot;normal&quot; state: aperture is wide open." %}

There is a trick to force the aperture to stay closed. What you need to do is the following (this idea comes from the always excellent [Micro 4/3 Photography blog](http://m43photo.blogspot.sg/2012/02/reverser-ring-for-macro.html)):

1. Put the camera in M mode, and set the aperture to the desired setting (here: f/22), with a long exposure (say 10 seconds).
2. Put the camera in manual focus, and set it to the desired distance (for macro shots: as close as possible).
3. Press the shutter.
4. While the camera is exposing, remove the lens. For some reasons, this will cause the shutter to close and open, but the aperture of the lens will stay at f/22, with the desired focus.

Look through your lens, it worked!

{% include img.html src="/images/macro-video/P1170874.JPG" alt="Lens with aperture closed." %}

Now, the fun bit. As soon as you mount the lens back on the camera, the aperture will reopen, and, well, you can start over... The trick here is to prevent the camera from communicating with the lens, by blocking the electrical connectors on the left of the picture above.

I do this with masking tape. I first put a big piece of tape on a large part of the lens:

{% include img.html src="/images/macro-video/P1170848.JPG" alt="Masking tape applied to lens contacts area." %}

And then carefully remove the excess tape:

{% include img.html src="/images/macro-video/P1170859.JPG" alt="&quot;Modified&quot; lens: the Micro 4/3 electrical contacts are isolated." %}

Make sure the tape is not loose (the last thing you want is a piece of tape on your sensor). Also make sure that it covers all connectors: The camera and/or lens may not appreciate to have only part of the pins carrying voltage.

Use a low-residue tape (like masking tape), so that you don't end up with dirty connectors on your lens and/or camera. Also, masking tape only leaves low residue if you peel it off quickly: once you're done with your video, remove the tape: don't leave it on for weeks, or you'll get so nasty glue left over...

You now have a "legacy" lens, 150mm, f/22, with the focus set to its closest setting. Put it back on the camera. If the camera complains, you need to set the "Shoot without lens" option. The camera will of course not allow you to change aperture, nor will it tell you what is the aperture (or the focal length, or which lens is mounted: you get the idea). As with all native Micro 4/3 lens, the focus is "by-wire", so you will not be able to change that either. And you won't get image stabilization, since the lens gets no power.

<!-- TODO: update macro-photography-cheap link once 2013-03-macro-photography-cheap is migrated -->
Put the [close-up filter](http://drinkcat.blogspot.com/2013/03/macro-photography-cheap.html) on the lens,, mount the camera on a tripod, and you are now ready to shoot your macro video!

The following video is what I obtain, straight out of the camera. The white balance is obviously wrong, there is some chromatic aberration on the edges of the image, and the soundtrack needs some cleanup, but that's a start:

<div style="text-align: center;">
<iframe allowfullscreen="" frameborder="0" height="315" src="http://www.youtube.com/embed/aan3prPY2W0" width="560"></iframe>
</div>

The [next post](/blog/2013/04/09/macro-video-correcting-chromatic/) will go into the technical details of fixing the white balance, chromatic aberration, and background noise in the soundtrack.
