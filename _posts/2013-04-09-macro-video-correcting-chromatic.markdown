---
layout: post
title: "Macro video: correcting chromatic aberration and white balance"
date: 2013-04-09 19:00:00+08:00
categories: photo blogspot
---

<!-- TODO: update macro-photography-cheap link once 2013-03-macro-photography-cheap is migrated -->
*This post is about taking macro videos, that is, videos of small objects, at relatively high magnification. In other posts, you can find a general introduction on taking [macro pictures using a close-up filter](http://drinkcat.blogspot.com/2013/03/macro-photography-cheap.html), and how to [correct chromatic aberration](/blog/2013/03/18/macro-photography-correcting-chromatic-aberration/).
The [previous post](/blog/2013/04/04/macro-video/) shows how to force the lens aperture on the DMC-GX1, this post gives technical details to correct chromatic aberration, white balance, and improve the soundtrack.*

As mentioned in the [previous post](/blog/2013/04/04/macro-video/), the Panasonic DMC-GX1 is severely limited when it comes to video mode: it does not allow manual setting of the aperture and white balance.

Once we managed to [trick the lens to force a slow aperture](/blog/2013/04/04/macro-video/), we get this not-so-nice video, with chromatic aberration (red/blue fringes in the corner of the images), incorrect white balance (the music box looks too white), and noisy soundtrack:

<div style="text-align: center;">
<iframe allowfullscreen="" frameborder="0" height="315" src="http://www.youtube.com/embed/aan3prPY2W0" width="560"></iframe>
</div>

With a few operations, we'll show how to turn it into this nicer looking one:

<div style="text-align: center;">
<iframe allowfullscreen="" frameborder="0" height="315" src="http://www.youtube.com/embed/5IlDgwzRR94" width="560"></iframe>
</div>

For this purpose, I'm going to use the following (free and open source) tools:

- [FFmpeg](http://www.ffmpeg.org/) and [mencoder](http://www.mplayerhq.hu/) for video frame processing.
- [hugin](http://hugin.sourceforge.net/)/[panotools](http://wiki.panotools.org/Main_Page) for the [fulla](http://wiki.panotools.org/Fulla) tool, to correct chromatic aberration.
- [ImageMagick](http://www.imagemagick.org/script/index.php) to fix the white balance.
- [Audacity](http://audacity.sourceforge.net/) for the audio processing.
- [kdenlive](http://www.kdenlive.org/) for general video editing (but any other tool would do).
- Everything assumes you're running Linux, but it should be doable on Windows/Mac OS X with a bit of effort.

### Extract frames from the video

The DMC-GX1 creates a MP4 video file. However, the tools at our disposal for chromatic aberration and white balance correction work on images. Therefore, we need to extract each frame from the video, which is easily done with FFmpeg:

```
mkdir mjpeg
ffmpeg -qscale 1 -i video.MP4 -f image2 -qscale 1 mjpeg/image-%05d.jpg
```

This outputs 1 JPEG file for each frame of the video. The `qscale` parameters should create JPEG files of a slightly better quality than the default.

{% include img.html src="/images/macro-video-correcting-chromatic/image-00100.jpg" alt="Sample frame from the video (near the start, the wheel is not moving yet). Some chromatic aberration can be seen (especially on the teeth of the wheel at the top right), and the white balance is clearly off." %}

### Correct chromatic aberration

<!-- TODO: update macro-photography-cheap link once 2013-03-macro-photography-cheap is migrated -->
Now, let's fix the chromatic aberration (red and blue fringes in the corners of the images), that is caused by our [cheap close-up filter](http://drinkcat.blogspot.com/2013/03/macro-photography-cheap.html):

```
cd mjpeg
ls *.jpg | xargs -I{} fulla -r -0.0046300:0.0177484:-0.0198170:0.9971389 -b 0.0007956:-0.0029477:0.0041624:1.0069115 -e 90 {}
```

For more details on how to find the parameters, have a look at this post on [correcting chromatic aberration of still images](http://drinkcat.blogspot.com/2013/03/macro-photography-correcting-chromatic-aberration.html).

{% include img.html src="/images/macro-video-correcting-chromatic/image-00100_corr.jpg" alt="Sample frame, corrected for chromatic aberration: the teeth of the wheel look sharper, and do not exhibit blue and red fringes." %}

### Correct white balance

White balance correction deserves an article on its own. I will probably write up something in the next few weeks. To make it short, we first take 2 pictures of the scene, one with a fairly correct balance ("Flash"), the other one with "Auto".

The auto white balance is very bad in this situation, as the scene is mostly yellow. This makes the camera believe that the scene is yellow because of the lighting, so it corrects it to a grayish color.

Looking at the EXIF of the good image (using exiftool, or any decent image editor), we see the following:

```
White Balance              : Flash
Red Balance                : 3.034873
Blue Balance               : 1.451461
```

While the EXIF of the wrong image looks like this:

```
White Balance              : Auto
Red Balance                : 2.34967
Blue Balance               : 1.896324
```

These numbers tell you that the Auto balance puts less red (2.34967) than the flash balance (3.034873), and more blue (1.896324 vs 1.451461). Therefore, we need to boost the red channel (multiply by a factor 3.034873/2.34967 = 1.246), while reducing the blue channel (1.451461/1.896324 = 0.681).

We can compute the corrected images using Imagemagick's convert `color-matrix` option. The following performs a batch operation on all the images:

```
mkdir ../mjpegcorr/
ls *_corr.jpg | xargs -I{} convert {} -color-matrix \
     " 1.246 0.0 0.0 0.0, 0.0, 0.0 \
       0.0 1 0.0 0.0, 0.0, 0.0 \
       0.0 0.0 0.681 0.0, 0.0, 0.0 \
       0.0 0.0 0.0 0.0, 0.0, 0.0 \
       0.0 0.0 0.0 0.0, 0.0, 0.0 \
       0.0 0.0 0.0 0.0, 0.0, 0.0" \
   ../mjpegcorr/{}
```

{% include img.html src="/images/macro-video-correcting-chromatic/image-00100_corr_wb.jpg" alt="Chromatic aberration and white balance corrected image." %}

I think the result's balance is still a little bit off, but it is already much more realistic than the original. A following article will focus on this, and see how and if we can correct it in a better way.

### Rebuild the video from the frames

Now that we have a bunch of corrected JPEG files, we can recreate a MJPEG video file:

```
cd ../mjpegcorr
mencoder mf://*_corr.jpg -mf fps=25:type=jpg -ovc copy -oac copy -o ../corr.avi
```

[MJPEG](http://en.wikipedia.org/wiki/Motion_JPEG) is a simple video file format, where each frame is a compressed JPEG file. We use the `-ovc copy` option, so that the frames are not recompressed.

### Fix the audio

The audio contains quite a lot of background noise. I first extracted the audio from the original MP4 file using kdenlive, then processed it using audacity, using the "Noise Removal" effect, followed by amplification. The noise removal also removes some of the signal, but the result sounds much better.

A better way would be to increase the signal, e.g. by fixing the music box on a resonance box, or by using a dedicated microphone, closer to the source (but... the DMC-GX1 does not have a mic input, so that doesn't really solve the problem...).

### Merge the audio and video together

Finally, I use kdenlive to merge the audio and video, cut the video (remove the shaky beginning, and some of the end), recompress it to x264, and you get this nice video!

<div style="text-align: center;">
<iframe allowfullscreen="" frameborder="0" height="315" src="http://www.youtube.com/embed/5IlDgwzRR94" width="560"></iframe>
</div>
