---
layout: post
title: "Random blog banner, part 2"
date: 2013-03-01 19:00:00+08:00
categories: others blogspot
---

Alright, the [previous part](/blog/2013/02/26/random-blog-banner-part-1/) shows you the script that allows to randomly display a banner image. Now, this would quickly end up being a bit messy to manage, if you want to add a new banner, then update the page with the [list of all panoramas](../../p/panoramas.html).

I wrote a Ruby script to semi-automate the process. The source can be found on [github](https://github.com/drinkcat/randombanner).

We first start with a database of images, called [`images.db`](https://github.com/drinkcat/randombanner/blob/master/images.db). It is a [YAML](http://en.wikipedia.org/wiki/YAML) file, with this format:

```yaml
- url:   https://lh4.googleusercontent.com/.../s{}/xx.jpg
  width:  800
  repeat: true
  offset: -30
  text:   Krabi - Thailand
  date:   2012.12
```

I like to use YAML, because it is easy to parse in Ruby, without being as wordy as XML, but more structured than a simple text file.

The fields are:

- `url`: source image location. `s{}` in the URL will be replaced by the proper image width
- `width`: width to display
- `repeat`: if the panorama is a full 360° that can be repeated
- `offset`: vertical offset to center the image
- `text`: Text that will appear in the banner, and in the image description
- `date`: year and month when the image was taken

Then, the script [`banner.rb`](https://github.com/drinkcat/randombanner/blob/master/banner.rb) reads `images.db`. It generates the Javascript arrays, and insert them in [`jscode.template`](https://github.com/drinkcat/randombanner/blob/master/jscode.template), creating a new [`jscode.js`](https://github.com/drinkcat/randombanner/blob/master/jscode.js) file, that you can then copy-paste on the blog.

It will also read [`pagecode.html.template`](https://github.com/drinkcat/randombanner/blob/master/pagecode.html.template), and generate [`pagecode.html`](https://github.com/drinkcat/randombanner/blob/master/pagecode.html) for the list of images.

That's it! Looking at the code should be somehow self-documenting, if you want to modify it. I release the code in public domain, feel free to use it.
