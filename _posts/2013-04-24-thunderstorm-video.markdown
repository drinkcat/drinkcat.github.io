---
layout: post
title: "Thunderstorm video"
date: 2013-04-24 19:00:00+08:00
categories: photo blogspot
---

The [previous post](/blog/2013/04/21/tropical-storm-composite-shot/) shows you pictures of a thunderstorm, and how to create a composite from the images. This one shows you how to make a video from the images.

Two week-ends ago, a fairly strong thunderstorm struck, with clear views from my window. I mounted my camera on a tripod, and I took close to 1000 shots in about 45 minutes, each with 2 seconds exposure.

The previous post shows you some [sample pictures, as well as a composite of many lightnings](/blog/2013/04/21/tropical-storm-composite-shot/).

{% include img.html src="/images/thunderstorm-video/compose-fromdiff.jpg" alt="Composite shot, see previous article." %}

This post will show you how to make a video from these images. Just playing those images at normal video speed (25 frames per second) will not work: because of the 2 seconds exposure I use, lightnings only appear on 1 images, at most 2. Played at 25 fps, each lightning would only appear for 1/25s: barely noticeable.

The idea is therefore to create some [burn-in effect](http://en.wikipedia.org/wiki/Afterimage), where the bright lightning stay for a number of frames, slowing fading away. The resulting video is below (go to [Vimeo](http://vimeo.com/64485466) for higher resolution):

<div style="text-align: center;">
<iframe allowfullscreen="" frameborder="0" height="281" src="http://player.vimeo.com/video/64485466" width="500"></iframe>
</div>

### Technique

The first step to create a video is to resize the images to HD format, that is 1080p (1920x1080):

```
mkdir sm1080
ls *.JPG | xargs -I{} convert -crop 4380x2464+143+67 -resize 1920x1080 -quality 95 {} sm1080/{}
```

I also do a bit of cropping, as my original framing shows some of the wall next to the window.

We then add the required "burn-in" effect, so that bright light will appear quickly, and slowly fade away. I tried several methods to create this effect, but this very simple method seems to work best:

1. Get the maximum between the last output frame and the current frame.
2. Blend this maximum image with the current frame (93% maximum, 7% current): this is your next output frame.
3. Iterate on the next input frame.

The idea is that a bright lightning will appear immediately, through the maximum operator: if the frame is bright, at step 2, the current frame is the same as the maximum frame, therefore you get 100% of the current frame. For following images, the current frame is less bright, so the brightness decays at a 93% rate. That is, the lightning will disappear almost completely within 29 frames (slightly more than a second). See Wikipedia on [exponential decay](http://en.wikipedia.org/wiki/Exponential_decay) for more details.

I tried different parameters, 90% made lightnings appear for a too short duration, and 95% led to significant artifacts: 93% seems to be a sweet spot.

This simple Ruby script, that calls ImageMagick, does the job for you:

```ruby
#!/usr/bin/ruby

list = Dir.new(".").to_a.select{|x| x.match(/.*\.JPG/)}.sort

system("mkdir output")
system("rm output/*")
system("cp #{list[0]} output/#{list[0]}")
list.each_cons(2){|k1, k2|
	system("convert output/#{k1} #{k2} -evaluate-sequence max output/max-#{k2}.tiff")
	system("composite output/max-#{k2}.tiff #{k2} -blend 93% output/#{k2}")
}
```

I output the maximum images as TIFF, as to avoid additional JPEG compression artifacts.

It is easy to check out the resulting video with mplayer:

```
mplayer mf://sm1080/output/*.JPG -mf fps=25:type=jpg
```

Once you are happy with the results, you can then encode the output (I chose highest quality x264 encoding):

```
mencoder mf://sm1080/output/*.JPG -mf fps=25:type=jpg -ovc x264 -x264encopts preset=veryslow:tune=film:crf=15:frameref=15:fast_pskip=0:threads=auto -o video.avi
```

The video can be seen in 720p on [Vimeo](http://vimeo.com/64485466), and in 1080p on [Youtube](http://www.youtube.com/watch?v=pQOGZcWx1EU) (unfortunately, Youtube does not let me choose a good thumbnail image, so I switched to Vimeo for this reason).

---

*Imported from "[Random ideas of a drinking cat](https://drinkcat.blogspot.com/)" blog.*
