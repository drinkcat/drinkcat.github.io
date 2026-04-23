---
layout: post
title: "Tropical storm composite shot"
date: 2013-04-21 17:00:00+08:00
categories: photo blogspot
---

One of the nice (and sometimes a little scary) things about living under the tropics is those very intense thunderstorms, especially at night.

{% include img.html src="/images/tropical-storm-composite-shot/compose-fromdiff.jpg" alt="Composite of an evening thunderstorm." %}

Last weekend a big one struck, so I mounted my camera (Panasonic DMC-GX1) on the window ledge, using my Gorillapod, and set the kit lens 14-42mm at its widest angle. Luckily, the rain was quite localized, so it wasn't raining much outside my window: no worries about getting my camera wet!

I use the camera in burst mode, more precisely the "H" mode, that allows maximum speed while keeping the maximum resolution. I keep the trigger pressed using my [home-made remote trigger](/blog/2012/12/16/simple-remote-trigger-for-panasonic/). All these shots are taken at f/5.0, with 2 seconds exposure, at ISO 160, slightly underexposed when there is no lightning, but sometimes overexposed for the brightest ones. Only mistake: I should have fixed the white balance, some of the shots clearly use different balances.

The interval between shots is short, around 400ms, meaning that we expose almost all the time, and therefore are able to capture most lightnings. Actually, some lightnings last for a fairly long time, and can be seen over 2 consecutive shots.

In total, I have close to 1000 shots, acquired over 45 minutes. Here are some of the best ones:

{% include img.html src="/images/tropical-storm-composite-shot/P1180227.JPG" alt="2 lightnings next to each other" %}

{% include img.html src="/images/tropical-storm-composite-shot/P1180375.JPG" alt="Intra-cloud lightnings, those are attenuated, probably because the light has to go though clouds and rain." %}

{% include img.html src="/images/tropical-storm-composite-shot/P1180419.JPG" alt="A bit overexposed, but notice how the sky becomes blue, at night." %}

The 13 nicest images of the series can be seen in this [Picasa album](https://picasaweb.google.com/107363214762755220776/SingaporeStormApril132013?authuser=0&authkey=Gv1sRgCKfM9Yfai4OKOw&feat=directlink).

I am going to do 2 things with those images: a composite image of all the lightnings, and a video, that I will show in the [next post](/blog/2013/04/24/thunderstorm-video/).

### Composite

I tried a few different ways to get a nice composite, here is what I think works best:

1. Select the nicest images, that have a clear lightning in it.
2. For each of these, take a difference image to the previous one in the series. That is, we want to remove all the background light (man-made light and other background lighting), and only take out the extra light created by the lightning. An image explains it best:

   {% include img.html src="/images/tropical-storm-composite-shot/montagediff-227.jpg" alt="From top to bottom: 1. Number 226 in sequence, no lightning; 2. Number 227 in sequence, strong lightning; 3. Difference between 227 and 226: all man-made lights have disappeared." %}

   Obtaining these difference images is straightforward with [ImageMagick](http://www.imagemagick.org/):

   ```
   convert ../P1180226.JPG P1180227.JPG -evaluate-sequence subtract diff/P1180227.JPG
   ```

3. The composition itself is done is [Gimp](http://www.gimp.org/): First, open one of the original images in Gimp, that does not contain a lightning. Then, drag and drop all the differences images in the *Layers* window.
4. Hide all these new layers (i.e. until you are back to the original image).
5. Move one of the difference layer above your original image (start with the brightest lightnings), and show it.
6. Choose *Screen* as [layer mode](http://docs.gimp.org/en/gimp-concepts-layer-modes.html). *Lighten* may also work, but produces results that are not as nice in my opinion.
7. Go to *Colors*, then *Levels*, and increase the input black level until you only see mostly the lightning, without too much increase in the background light in the sky. You can also change the input white level if you want the lightning to appear brighter.
8. Repeat from step 5.

This shows you the progression of the composition, starting from the original image, step 0:

{% include img.html src="/images/tropical-storm-composite-shot/montage-compose-fromdiff.jpg" alt="Montage of the progression of the composite image. One image is added at each step (2 images from 3 to 5). For step 1, the black level is not increased significantly, so as to get the blue sky on the right." %}

The image at step 12 is the one shown at the beginning of this post, after slight color adjustments.
