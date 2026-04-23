# Blogger Import Guide

Migrating posts from https://drinkcat.blogspot.com/ to this Jekyll blog, one at a time.

## Source material

- Mirrored HTML: `blogger-mirror/drinkcat.blogspot.com/YYYY/MM/slug.html`
- All posts listed at: `blogger-mirror/drinkcat.blogspot.com/index.html`

## Per-post conversion steps

### 1. Determine post metadata

From the mirrored HTML, extract:
- **Title**: `<h3 class='post-title entry-title'>`
- **Date**: `<abbr class='published' itemprop='datePublished' title='YYYY-MM-DDTHH:MM:SS+TZ'>`
- **Labels/categories**: `<a href='.../search/label/...' rel='tag'>` in post footer

### 2. Create the Jekyll post file

Filename: `_posts/YYYY-MM-DD-slug.markdown` (use same slug as Blogger URL)

Frontmatter:
```yaml
---
layout: post
title: "Post Title"
date: YYYY-MM-DD HH:MM:SS+TZ
categories: category blogspot
---
```

Always include `blogspot` as a tag/category to identify imported posts.

### 3. Convert HTML content to Markdown

The post body is in `<div class='post-body entry-content'>`.

Conversion rules:
- `<h3>`, `<h4>` → `###`, `####` headers (strip inline styles)
- `<b>` → `**bold**`
- `<i>` → `*italic*`
- `<a href="URL">text</a>` → `[text](URL)`
- `<br />` → remove or use blank line between paragraphs
- `<ul>/<li>` → `- list item`
- Plain paragraphs: strip surrounding `<div>` and `<p>` tags
- `<!--more-->` → remove (use `excerpt:` in frontmatter if needed)

**Images with captions** (Blogger `tr-caption-container` table pattern):
```html
<table class="tr-caption-container">
  <tr><td><a href="...s1600/name.jpg"><img src="...s640/name.jpg"/></a></td></tr>
  <tr><td class="tr-caption">Caption text</td></tr>
</table>
```
→ download the `s1600` full-size image, store in `images/POST-SLUG/name.jpg`, generate thumb with `bin/make-thumbnails.sh`, then:
```
{% include img.html src="/images/POST-SLUG/name.jpg" alt="Caption text" %}
```

**Images without captions** (`div.separator` pattern):
```html
<div class="separator"><a href="...s1600/name.jpg"><img src="...sNNN/name.jpg"/></a></div>
```
→ same download/embed as above, use filename as alt text or leave blank.

**Inline formula/small images** (no link, no zoom needed): use a plain `<img>` tag instead of the include:
```html
<img src="/images/POST-SLUG/name.png" alt="description" style="display: block; margin: auto;" />
```

**Data tables**: keep as raw HTML — kramdown passes it through fine.

**Code blocks** (`<div class="code">` with monospace spans, or `<pre>`):
→ triple-backtick fences, add language hint if obvious.

**Dead embeds** (Flash, Picasa slideshows): remove entirely.

**Internal Blogger links**: leave as-is for now; update as those posts are migrated.

### 4. Download and process images

```bash
# Create image directory for this post
mkdir -p images/POST-SLUG

# Download full-size images (replace URL with s1600 variant)
wget -P images/POST-SLUG/ "https://blogger.googleusercontent.com/.../s1600/name.jpg"

# Generate thumbnails
bin/make-thumbnails.sh images/POST-SLUG/*.jpg images/POST-SLUG/*.png
```

### 5. Add TODOs for internal Blogger links

For any link pointing to another `drinkcat.blogspot.com` post not yet migrated, add an HTML comment above the line:

```
<!-- TODO: update link below once YYYY-MM-slug is migrated -->
```

### 6. Preview

```bash
bin/serve.sh
```

Check the post renders correctly and images load.

## Posts to migrate

Listed roughly newest-first. Check off as done.

- [ ] 2013-12-01 `opencl-on-samsung-chromebook-arm` (linux)
- [ ] 2013-11-xx `opencl-on-samsung-chromebook-arm-under` (linux) — referenced by above
- [ ] 2013-10-xx `running-chromium-os-in-qemu` (linux)
- [ ] 2013-09-xx `tracking-your-dhl-package-in-conky` (linux)
- [ ] 2013-07-xx `olympus-om-50mm-f18-on-mft` (photo)
- [ ] 2013-07-xx `bokeh-fixing-opening-and-cleaning` (photo)
- [ ] 2013-06-xx `parse-xml-from-shell-scripts` (linux)
- [ ] 2013-06-xx `good-morning-haze` (photo)
- [ ] 2013-06-xx `chromebook` (linux)
- [ ] 2013-05-xx `archlinux-swapping-hard-drives-between` (linux)
- [ ] 2013-04-xx `tropical-storm-composite-shot` (photo)
- [ ] 2013-04-xx `thunderstorm-video` (photo)
- [ ] 2013-04-xx `macro-video` (photo)
- [ ] 2013-04-xx `macro-video-correcting-chromatic` (photo)
- [ ] 2013-03-xx `random-blog-banner-part-2` (others)
- [ ] 2013-03-xx `macro-photography-correcting-chromatic-aberration` (photo)
- [ ] 2013-03-xx `macro-photography-cheap` (photo)
- [ ] 2013-02-xx `triggering-panasonic-lumix-from-arduino` (photo)
- [ ] 2013-02-xx `tilt-adapter-for-micro-four-third-part3` (photo)
- [ ] 2013-02-xx `tilt-adapter-for-micro-four-third-part2` (photo)
- [ ] 2013-02-xx `tilt-adapter-for-micro-four-third-part1` (photo)
- [ ] 2013-02-xx `random-blog-banner-part-1` (others)
- [ ] 2013-01-xx `pinwide-lens-or-real-life-instragram` (photo)
- [ ] 2013-01-xx `picasa-as-random-data-storage` (others linux)
- [ ] 2013-01-xx `nikon-charger-vs-nikon-charger` (others)
- [ ] 2012-12-xx `simple-remote-trigger-for-panasonic` (photo)
- [ ] 2012-12-xx `playing-galileo` (photo)
- [ ] 2012-12-xx `migrating-from-gentoo-to-archlinux-part_4` (linux)
- [ ] 2012-12-xx `migrating-from-gentoo-to-archlinux-part_7` (linux)
- [ ] 2012-12-xx `migrating-from-gentoo-to-archlinux-part` (linux)
- [ ] 2012-12-xx `dell-charger-vs-dell-charger` (others)
