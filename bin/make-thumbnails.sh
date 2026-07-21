#!/bin/bash
# Generate thumbnails for images that don't have one yet.
# Thumbnails are placed alongside originals with a -thumb suffix.
# Usage: bin/make-thumbnails.sh [images...]
# If no arguments given, processes all images/*.{jpg,jpeg,png}

THUMB_WIDTH=500
THUMB_QUALITY=92

images=("$@")
if [ ${#images[@]} -eq 0 ]; then
    mapfile -t images < <(find images/ -maxdepth 1 -iname "*.jpg" -o -iname "*.jpeg" -o -iname "*.png" | sort)
fi

for src in "${images[@]}"; do
    ext="${src##*.}"
    base="${src%.*}"
    thumb="${base}-thumb.${ext}"

    if [ -f "$thumb" ]; then
        echo "skip: $thumb already exists"
        continue
    fi

    # Slight pre-blur kills moire/aliasing from photographed LCD screens
    # before the downscale, then Lanczos resize keeps edges/text sharp.
    magick "$src" -blur 0x0.6 -filter Lanczos -resize "${THUMB_WIDTH}x>" -quality "$THUMB_QUALITY" "$thumb"
    echo "created: $thumb"
done
