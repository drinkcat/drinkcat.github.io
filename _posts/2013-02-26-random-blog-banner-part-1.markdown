---
layout: post
title: "Random blog banner - Part 1"
date: 2013-02-26 19:00:00+08:00
categories: others blogspot
---

You probably have noticed: the banner image of this blog changes every time you refresh the page. Those are stitched panorama from many pictures, sometimes spanning a full 360 degrees, sometimes less (I will write something about stitching these panoramas at some point...).

The code to randomize the banner image is easily available online, I took it from [this page](http://forum.cloudmedianews.com/forum-cms/random-header-and-background-images-for-blogspot-t586.html), but it is found in so many places that I'm not sure who wrote it first.

My updated version follows:

```javascript
var banner= new Array()
var shift= new Array()
var information= new Array()
banner[0]="https://...s1600/101P.jpg"
shift[0]="no-repeat center -50px"
information[0]="Jiufen - Taiwan"
//...
var random=Math.floor(banner.length*Math.random());
var query = window.location.search.substring(1);
var match = query.match(/.*headerimage=([0-9]+).*/);
if (match) {
document.write("Manual image index:");
document.write(match[1]);
random = match[1];
}
document.write("<style>");
document.write(".body-fauxcolumn-outer .cap-top {");
document.write(' background: #FFFFFF url("' + banner[random] + '") ' + shift[random] + ';');
document.write(" }");
document.write("</style>");
document.getElementById('headerinformation').innerHTML = information[random];
```

Let's take it step by step. I define 3 arrays: `banner`, `shift`, and `information`, each index representing one image:

- `banner` contains the URL of the image. I extract the public link from the Picasa Web Albums. This URL can be obtained by a right-click on the image, and then "View Image". The URL looks like this: `https://lh6.googleusercontent.com/.../s1600/X.jpg`. The number after the letter `s` is particularly interesting: it defines the width of the image. If you change it, Google will very nicely resize it to the size you want. Using `s0` gives you the original resolution.
- `shift` contains some extra CSS style information. For full 360° panoramas, the image can be repeated endlessly without seams, so the parameter `repeat` is used, otherwise, the panorama is shown only once. Also, you can set an offset, so that the image gets centered vertically, so you don't get only sky or only land/sea in the banner.
- `information` contains some short description about the image.

Then, I pick a random number in the list, unless the URL is something like `http://drinkcat.blogspot.com/?headerimage=0`: In that case I force displaying image 0. That's useful for debugging.

Finally, the code generates a little piece of CSS style, that will set the banner image. Also, it modifies the HTML element with id `headerinformation`, so that it shows the image information. In my case, this element is at the bottom of the banner, aligned to the right, and tells you where the image was taken.

All right. All for now, [next post](/blog/2013/03/01/random-blog-banner-part-2/) will show how to generate the script, as well as [the panoramas page](/panoramas/), automatically.

---

*Imported from "[Random ideas of a drinking cat](https://drinkcat.blogspot.com/)" blog.*
