---
layout: post
title: "Tracking your DHL package in conky"
date: 2013-09-30 19:00:00+08:00
categories: linux others blogspot
---

*(or converting the DHL tracking page from HTML to plain text, using XSLT)*

So, let me explain the problem: You have that package shipped by DHL, and its tracking number. And you're so eager to receive it, that you end up checking the package tracking page every 5 minutes. Your productivity falls to zero.

But, worry no more! Let's put the package status inside your [conky](http://conky.sourceforge.net/), so that you can just have a quick look on the side of your screen, and continue working.

Just in case you don't know (but really, you should), conky is the information bar on the right of the screenshot below:

{% include img.html src="/images/tracking-your-dhl-package-in-conky/screenshot.png" alt="conky is the information bar on the right on the display. CPU/RAM usage, disk free space, network status, temperature, a calendar, my DHL tracking thing, then weather in a few places. In case you wonder, the background is somewhere in Hue, Vietnam. And don't worry about the gimp error, really." %}

The idea is to write the code that can generate this:

{% include img.html src="/images/tracking-your-dhl-package-in-conky/screenshot-zoom.png" alt="Yes, they spelled my name wrong..." %}

From something as ugly looking as this:

{% include img.html src="/images/tracking-your-dhl-package-in-conky/screenshot-dhl.png" alt="DHL tracking page" %}

### I don't care how it works, I just want to get it running

Ok! After all the point of this was to increase your productivity, right? You can fetch the script from [my github](https://github.com/drinkcat/tracking-conky).

Then call it with:

```
./dhl <AWB>
```

Where `<AWB>` is the Waybill number (tracking number). It produces a text-only tracking information for your package.

You can integrate it in your conky with something like:

```
${font Monospace:size=6}${execi 60 ~/.conky/dhl <AWB> | head -n 3 | fold -w 16}$font
```

Replace `~/.conky/dhl` with the path to where you copied the script. Change the `head` parameter if you want more lines, and `fold` inserts new lines every 16 characters (change that depending on your conky width).

Now, if you want to know how it works, so you can fix it if it breaks, or update the code for other shipping companies, continue reading.

### Inspecting the HTML source

The tracking URL looks like this (where `<AWB>` is your tracking number):

```
http://www.dhl-usa.com/content/us/en/express/tracking.shtml?brand=DHL&AWB=<AWB>
```

Looking at the HTML source, we notice that the interesting stuff is enclosed in a table:

```html
<table border="0" summary="Summary of table content">
```

Then, you have a succession of `thead`/`tbody` tags. The first `thead` contains general information about the package, that we are not interested in. It starts like this (notice it has class `tophead`):

```html
<thead class="tophead">
```

The next `thead` shows the date valid for the following entries. We are only interested in the first column here (the one that contains the date).

```html
<thead>
    <tr>
        <td colspan="5" class="emptyRow"></td>
    </tr>
    <tr>
        <th scope="col" colspan="2" axis="length"
            style="width: 40% ;text-align:left">Thursday, September 19, 2013         </th>
        <th scope="col" axis="length"
            style="width: 30% ;text-align:left ">Location</th>
        <th scope="col" axis="length"
            style="width: 9%;text-align:left">Time</th>
        <th scope="col" axis="length" class="lastChild"
            style="width: 25% ;text-align:left">&nbsp;</th>
    </tr>
</thead>
```

Finally, the bulk of the events are enclosed in `tbody`. The first column is a incremented number, the second one is a description of what happened (passed customs, arrived at destination, etc.), the third one tell you the location (but this is often repeated in the description), and the fourth one is the time.

```html
<tbody>
    <tr>
        <td class="" style="width: 5% ;text-align:left">18</td>
        <td class="" style="text-align:left">With delivery courier</td>
        <td class="" style="text-align:left">SINGAPORE - SINGAPORE</td>
        <td class="">7:27 PM</td>
        <td class="lastChild "><!--start contentteaser -->
        <div class="dhl">
        <div><div class="clearAll">&nbsp;</div></div>
        </div><!--end contentteaser --></td>
    </tr>
</tbody>
```

Ok, now we have an idea of the structure, let's parse that!

### Parse HTML with XSLT

Ok, so let's say you have the DHL tracking page downloaded to `/tmp/dhl.tmp`, and an XSLT file in `dhl.xslt`, you can parse the page with:

```
xsltproc --html dhl.xslt /tmp/dhl.tmp
```

The XSLT file looks like this:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:output method="text" encoding="utf-8" />
    <xsl:template match="/">
        <xsl:for-each select="//table[@summary='Summary of table content']/*[self::thead|self::tbody][not(@class)]">
            <xsl:choose>
                <xsl:when test="name(.) = 'thead'">
                    <xsl:value-of select="tr/th[1]"/>
                    <xsl:text>
</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:if test="floor(tr/td[1]) = tr/td[1]">
                        <xsl:value-of select="normalize-space(tr/td[4])"/>
                        <xsl:text>: </xsl:text>
                        <xsl:value-of select="normalize-space(tr/td[2])"/>
                        <xsl:text>
</xsl:text>
                    </xsl:if>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:for-each>
    </xsl:template>
</xsl:stylesheet>
```

Let's take it step by step. It starts like this:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:output method="text" encoding="utf-8" />
    <xsl:template match="/">
```

Nothing special here, apart from the `text` output mode, so that `xsltproc` outputs a text file (and not another XML file...).

Now begins the fun. We look for a table with `summary` attribute `'Summary of table content'`. Inside that table, we look for `thead` and `tbody` elements, that do not have a class attribute set, so we can exclude the `'tophead'` row, that we are not interested in.

```xml
<xsl:for-each select="//table[@summary='Summary of table content']/*[self::thead|self::tbody][not(@class)]">
```

Now, `thead` (containing only the date of the following events) and `tbody` (containing events) need to be parsed differently. This is done with `xsl:choose`:

```xml
<xsl:choose>
    <xsl:when test="name(.) = 'thead'">
    ...
    </xsl:when>
    <xsl:otherwise>
    ...
    </xsl:otherwise>
</xsl:choose>
```

For `thead`, we just want to show the date, that is the first `th` inside a `tr` (`tr/th[1]`). Then we print a new line with `xsl:text`.

```xml
<xsl:when test="name(.) = 'thead'">
    <xsl:value-of select="tr/th[1]">
    <xsl:text>
</xsl:text>
</xsl:value-of></xsl:when>
```

For `tbody`, it is slightly more complicated. First, we check that the first column is indeed a number (this removes the last row in the table, which is another type of summary): this is done with a "trick" (`floor(tr/td[1]) = tr/td[1]`). Then we print the time (4th column), followed by a colon, and the event description (2nd column).

```xml
<xsl:otherwise>
    <xsl:if test="floor(tr/td[1]) = tr/td[1]">
        <xsl:value-of select="normalize-space(tr/td[4])"/>
        <xsl:text>: </xsl:text>
        <xsl:value-of select="normalize-space(tr/td[2])"/>
        <xsl:text>
</xsl:text>
    </xsl:if>
</xsl:otherwise>
```

That's it! Then you can put everything in a shell script, see [the complete code on github](https://github.com/drinkcat/tracking-conky/blob/master/dhl) for details.

### It's so cool, I want more!

I get it. I, too, have become a fan of parsing XML/HTML from scripts. See [this post](/blog/2013/06/09/parse-xml-from-shell-scripts/) for another example.
