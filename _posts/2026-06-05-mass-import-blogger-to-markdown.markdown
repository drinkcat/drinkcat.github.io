---
layout: post
title: "Mass-import blogger to markdown"
date: 2026-06-05 14:00:00+08:00
categories: others
---

I have an older blog on Blogspot from many years ago -- it's still up, but there's no telling for how long. Looking at the content, I thought some of it was still somewhat interesting (at least to me, as a trip down memory lane), so it'd be worthwhile converting the articles to markdown and including them here.

This idea came when listening to the excellent [Vergecast](https://www.theverge.com/the-vergecast), and in particular this episode, where they mention using AI to fix up formatting in their archive:

> I'll give you an example just from our own little database. The Verge is a database. Like, what is a website? It's a big database full of stories. There's a bunch of old features in our database that are broken because of successive redesigns or web standards, changes, or whatever it is. After 15 years, there are stories on our site that are broken. I'm like, we should just let Claude fix them.
>
> This is a classic example of we would never pay a human being to go through the archive because we'll never get enough traffic to pay back the work. And it's like, I can, this is what it's for. It's amazing. Its ability to strip bad HTML and replace it with good HTML is [...] a fundamental capability of Claude Code. And I don't have this [...] labor anxiety about it because in no world was that ever a good idea to set a human upon doing. Like it just economically made no sense. Engineers want to make new things that people care about. They don't want to make old things. Or fix old things.
>
> — [Vergecast: Fear and Loathing at OpenAI](https://www.theverge.com/podcast/909621/openai-sam-altman-drama-vergecast) ([transcript](https://www.tapesearch.com/episode/fear-and-loathing-at-openai/LMY9ghUmbGawMJZ5DNeoJ4))[^1]

[^1]: Fun fact: I used Claude in browser to find the episode, impossible to find otherwise...

This totally resonated with me. It would not be worth doing the work myself, but if I can just throw tokens at it? Sure.

So I did, just that. "I" downloaded the whole of my old blog using recursive `wget`. Then asked Claude to convert one page to markdown, and write down instructions. I then looked at the output, asked it to iterate on the instructions. Once I got a bit more confident, I asked it to convert batches of 5-10 pages at a time.

Took me maybe 2 to 3 hours, and, not that many tokens -- you don't need a very smart model for this.

Here's the final guide, for reference:

<div class="small-code" markdown="1">

````markdown
{% raw %}# Blogger Import Guide

Migrating posts from https://drinkcat.blogspot.com/ to this Jekyll blog,
one at a time.

## Source material

- Mirrored HTML: `blogger-mirror/drinkcat.blogspot.com/YYYY/MM/slug.html`
- All posts listed at: `blogger-mirror/drinkcat.blogspot.com/index.html`

## Per-post conversion steps

### 1. Determine post metadata

From the mirrored HTML, extract:
- **Title**: `<h3 class='post-title entry-title'>`
- **Date**: `<abbr class='published' itemprop='datePublished' title='YYYY-MM-DDTHH:MM:SS+TZ'>`
- **Labels/categories**: `<a href='.../search/label/...' rel='tag'>` in
  post footer

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

Always include `blogspot` as a tag/category to identify imported
posts.

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

**Images with captions** (Blogger `tr-caption-container` table
pattern):
```html
<table class="tr-caption-container">
  <tr><td><a href="...s1600/name.jpg"><img src="...s640/name.jpg"/></a></td></tr>
  <tr><td class="tr-caption">Caption text</td></tr>
</table>
```
→ download the `s1600` full-size image, store in
`images/POST-SLUG/name.jpg`, generate thumb with
`bin/make-thumbnails.sh`, then:
```
{% include img.html src="/images/POST-SLUG/name.jpg" alt="Caption text" %}
```

**Images without captions** (`div.separator` pattern):
```html
<div class="separator"><a href="...s1600/name.jpg"><img src="...sNNN/name.jpg"/></a></div>
```
→ same download/embed as above, use filename as alt text or leave
blank.

**Inline formula/small images** (no link, no zoom needed): use a plain
`<img>` tag instead of the include:
```html
<img src="/images/POST-SLUG/name.png" alt="description" style="display: block; margin: auto;" />
```

**Data tables**: keep as raw HTML — kramdown passes it through fine.

**Code blocks** (`<div class="code">` with monospace spans, or `<pre>`):
→ triple-backtick fences, add language hint if obvious.

**Dead embeds** (Flash, Picasa slideshows): remove entirely.

**Internal Blogger links**: leave as-is for now; update as those posts
are migrated.

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

For any link pointing to another `drinkcat.blogspot.com` post not yet
migrated, add an HTML comment above the line:

```
<!-- TODO: update link below once YYYY-MM-slug is migrated -->
```

### 6. Preview

```bash
bin/serve.sh
```

Check the post renders correctly and images load.
{% endraw %}
````

</div>

