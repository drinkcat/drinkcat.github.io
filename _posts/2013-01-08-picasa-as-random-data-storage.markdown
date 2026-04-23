---
layout: post
title: "Picasa as (random) data storage"
date: 2013-01-08 10:22:00+08:00
categories: others linux blogspot
---

People have been using Gmail storage space for backup purpose, [GmailFS](http://sr71.net/projects/gmailfs/) is one of such projects. What I propose here is to use Google Picasa: Storage space is unlimited, as long as your image dimensions do not exceed 2048x2048 pixels. By encoding data to appear as an image, you could potentially get unlimited storage space.

> **Disclaimer**: This may be a gray area in terms of Google Terms of Service. I can't find anything that explicitly forbids doing what I describe here, but I would not be surprised if your data suddenly disappears, or if your account gets [suspended](http://www.dslreports.com/forum/remark,16983539) (it happened for people using GmailFS), especially if you start uploading terabytes of data...
> Anyway, I would not use it for serious data backup, and I don't use it myself. I just thought the idea was fun.

Alright. The first step is to install the 2 tools we will need:

- Google command line (`googlecl` package on Archlinux)
- Imagemagick (`imagemagick` package)

Then, pick a file you want to upload. I created a simple text file. The text is only a few hundreds bytes long, so I encoded a JPEG image in base64, and appended it to the text:

```
Hello World!

File below (a jpeg image), can be decoded with:
tail -n +10 myfile | base64 -d > img.jpg

Random ideas of a drinking cat
-- http://drinkcat.blogspot.com/

BEGIN
/9j/4AAQSkZJRgABAQAAAQABAAD/4QlERXhpZgAASUkqAAgAAAACADEBAgAHAAAAJgAAAGmHBAAB
[many lines of base64 encoded content]
```

The total file length is 73305 bytes. This can easily fit in a 256x256 color image: each pixel takes 3 bytes, one for each of the 3 base colors (red, green, blue). 256 * 256 * 3 = 196608 bytes. But remember, you can store much more than that in one image: the maximum "free" image size is 2048x2048, that's 12 MiB. You just need to split your data in chunks of 12 MiB, and you can store as much data as you want.

We are going to create a PNG file out of the data. We use PNG instead of JPEG, because PNG uses a lossless compression algorithm, while JPEG compression is lossy: it tries to minimize differences in image, according to what the human eye perceives. Good for pictures of real things, but not good when you're trying to recover the exact information you put in the image.

We can use Imagemagick to create the PNG file:

```
convert -size 256x256 -depth 8 RGB:myfile myfile.png
```

However, this outputs:

```
convert: unexpected end-of-file `myfile': No such file or directory @ error/rgb.c/ReadRGBImage/231.
```

This is because the text file is not long enough to cover the whole 256x256 image.

The workaround is simple: let's create a blank file of the correct size, and overwrite the beginning of the file with our text file:

```
dd if=/dev/zero of=pad bs=1 count=196608
dd if=myfile of=pad conv=notrunc
```

The `notrunc` option makes sure the output file is not truncated before the content of `myfile` is written.

Now we can convert the file:

```
convert -size 256x256 -depth 8 RGB:pad myfile.png
```

And we get this (not-so-)beautiful image.

{% include img.html src="/images/picasa-as-random-data-storage/myfile.png" alt="The encoded image." %}

You can see some black line patterns in the image. This corresponds to the new lines in the base64 encoded image: every 77 characters, there is a line feed. The line feed has ASCII value 10, i.e. quite dark, while the rest are printable characters with values above 60 (darkish gray).

Now we can easily recover the content. Download the image as `myfile2.png`, then run:

```
convert myfile2.png RGB:pad2
dd if=pad2 of=myfile2 bs=1 count=73305
```

And you can check that `myfile` and `myfile2` are identical.

We could of course automate the process, but that's not the point here (feel free to do it if you want to take the risk of using Picasa as a sharing facility).

A nicer, and more "subversive" way is [steganography](http://en.wikipedia.org/wiki/Steganography). Steganography embeds data in an otherwise normal looking image. But that will be for another post.

---

*Imported from "[Random ideas of a drinking cat](https://drinkcat.blogspot.com/)" blog.*
