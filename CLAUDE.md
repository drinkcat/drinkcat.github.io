# drinkcat.github.io

Jekyll blog. Posts live in `_posts/`, named `YYYY-MM-DD-slug.markdown`.

## Adding images to a post

1. Put the full-size image in `images/POST-SLUG/name.jpg` (create the
   directory if needed; `POST-SLUG` doesn't need to match the post's
   filename slug exactly, just be descriptive and unique).
2. Generate a thumbnail: `bin/make-thumbnails.sh images/POST-SLUG/name.jpg`
   (or with no args, processes all images missing a thumbnail). This
   creates `name-thumb.jpg` alongside the original -- the include below
   expects both to exist.
3. Embed with the `img.html` include, which renders a clickable
   thumbnail (opens the full-size image in a lightbox) with an
   optional caption:
   ```
   {% include img.html src="/images/POST-SLUG/name.jpg" alt="Caption text" width="80%" %}
   ```
   - `alt` doubles as the visible `<figcaption>` -- omit it for no caption.
   - `width` defaults to `100%`; use a smaller value (e.g. `80%`) for tall/narrow photos.
   - Multiple images side by side: wrap includes in `<div class="img-row">...</div>`.
4. For inline formula/small images that don't need a lightbox, use a
   plain `<img>` tag instead:
   ```
   <img src="/images/POST-SLUG/name.png" alt="description" style="display: block; margin: auto;" />
   ```

## Internal links between posts

Use Jekyll's `{% post_url %}` tag, not raw relative paths:
```
[part 2]({% post_url 2026-05-23-zapper-pcb %})
```

## Footnotes

Kramdown footnotes: inline marker `[^1]`, definition anywhere (usually
grouped at the end of the post) as `[^1]: text`.

## Migrating old Blogger posts

See [_posts/2026-06-05-mass-import-blogger-to-markdown.markdown](_posts/2026-06-05-mass-import-blogger-to-markdown.markdown)
for the full conversion workflow (HTML→markdown rules, image
extraction from Blogger's caption/separator patterns, TODOs for
cross-post links).
