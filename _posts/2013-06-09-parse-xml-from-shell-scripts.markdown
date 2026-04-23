---
layout: post
title: "Parse XML from shell scripts"
date: 2013-06-09 19:00:00+08:00
categories: linux blogspot
---

As part of the [crouton](https://github.com/dnschneid/crouton)/[chroagh](https://github.com/drinkcat/chroagh) project, I wanted to be able parse D-bus configuration file, to figure out what user account the system D-bus runs under.

The configuration file looks like this:

```xml
# /etc/dbus-1/system.conf
<!DOCTYPE busconfig PUBLIC "-//freedesktop//DTD D-Bus Bus Configuration 1.0//EN"
 "http://www.freedesktop.org/standards/dbus/1.0/busconfig.dtd">
<busconfig>

  <!-- Our well-known bus type, do not change this -->
  <type>system</type>

  <!-- Run as special user -->
  <user>dbus</user>

  <!-- Fork into daemon mode -->
  <fork/>

  .....

</busconfig>
```

Yes, that's XML. And normally, XML and shell scripts are not exactly good friends...

What we want to be able to do here is to fetch the content of the `user` tag.

### Solution 1 - sed

Well, that's without doubt the easiest:

```
sed -n 's|.*<user>\(.*\)</user>|\1|p' /etc/dbus-1/system.conf
```

Problem is that the is no guarantee that there would be no other user tag[^1], in other parts of the file, and, who knows, the tag might be commented out. We also do not handle "misplaced" newlines in the file...

[^1]: Actually, there is no other user tag, according to the dtd file, but not necessarily in a general case... And well, this post is boring if I stop here, right?

### Solution 2a - xmllint

A more proper and generic solution, making use of the xmllint parser:

```
echo "cat /busconfig/user/text()" | xmllint --shell /etc/dbus-1/system.conf
```

Problem is, xmllint is not really script-friendly, and outputs some garbage along with the desired output.

```
$ echo "cat /busconfig/user/text()" | xmllint --shell /etc/dbus-1/system.conf
/ > cat /busconfig/user/text()
 -------
dbus
/ > $
```

In my case, I know how a valid username looks like, so I just pipe the output through `grep '^[a-z][-a-z0-9_]*$'`, and that's the solution that is used in my code. Also, xmllint is installed by default on Chrome OS, so that's good enough for me.

### Solution 2b - xmllint and write

An improved version would be:

```
echo "cd //busconfig/user/text()
     write tmp" | xmllint --shell /etc/dbus-1/system.conf
cat tmp
```

This is cleaner, as the full text of the tag is written to a file, but this requires an intermediate file, so, maybe not that nice...

### Solution 2c - xmllint, write, and fd/3

Maybe an even nicer one, assuming `/dev/fd` exists (as it does on recent Linux distributions):

```
exec 3>/dev/null
echo "cd //busconfig/user/text()
     write /dev/fd/3" | xmllint --shell /etc/dbus-1/system.conf 3>&1 1>/dev/null 2>/dev/null
exec 3<&-
```

No temporary file!

### Solution 3 - xsltproc

Finally, the most powerful version, using XSLT:

```
xsltproc - /etc/dbus-1/system.conf <<END
<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="text" encoding="utf-8" />
<xsl:template match="/"><xsl:value-of select="/busconfig/user"/></xsl:template>
</xsl:stylesheet>
END
```

That's as good as it gets, and you could easily do much more complicated things with XSLT. But, that's a bit overkill for our purpose (and `xsltproc` is not installed in Chrome OS).

---

*Imported from "[Random ideas of a drinking cat](https://drinkcat.blogspot.com/)" blog.*
