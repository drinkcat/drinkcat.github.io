---
layout: post
title: "Grabbing LCD display: Overall design, chip selection and performance"
date: 2026-07-22 11:00:00+08:00
categories: embedded
excerpt: >-
  Now that we got a basic LCD grabber working, the next step is to move forward towards a design that taps on the LCD display lines while still keeping the display functional. We'll end up manufacturing a PCB for this purpose, but that'll be for the next article.
---

This is the second part of my LCD display grabbing adventure, the first part is [here]({% post_url 2026-07-12-air-quality-monitor-lcd-grab %}). This one is perhaps a little drier, but explains design constraints.

Now that we got a basic LCD grabber working, the next step is to move forward towards a design that taps on the LCD display lines while still keeping the display functional. We'll end up manufacturing a PCB for this purpose, but that'll be for the next article.

### Design constraints

I have a few constraints for this project that will end up making the software implementation tricky.

I want to keep the **original device fully functional**: The previous version disconnected the original LCD display, we want something that works as a **passthrough**, keeping the original device functional.

<svg viewBox="0 45 570 110" xmlns="http://www.w3.org/2000/svg" font-family="-apple-system, BlinkMacSystemFont, 'Segoe UI', Helvetica, Arial, sans-serif" style="max-width: 100%; height: auto; display: block; margin: 0 auto;">
  <defs>
    <marker id="arrow1" viewBox="0 0 10 10" refX="9" refY="5" markerWidth="6" markerHeight="6" orient="auto-start-reverse">
      <path d="M 0 0 L 10 5 L 0 10 z" fill="#333"/>
    </marker>
  </defs>

  <rect x="5" y="52" width="150" height="96" rx="6" fill="#f5f5f5" stroke="#333" stroke-width="1.5"/>
  <text x="80" y="98" text-anchor="middle" font-size="15" fill="#222">main board</text>
  <text x="80" y="120" text-anchor="middle" font-size="12" fill="#555">(existing AQ device)</text>

  <line x1="155" y1="100" x2="235" y2="100" stroke="#333" stroke-width="3" marker-end="url(#arrow1)"/>
  <text x="195" y="88" text-anchor="middle" font-size="11" fill="#555">flex 39p</text>

  <rect x="235" y="52" width="150" height="96" rx="6" fill="#eaf2ff" stroke="#2a5db0" stroke-width="1.5"/>
  <text x="310" y="98" text-anchor="middle" font-size="15" font-weight="600" fill="#1a3d75">Capture Board</text>
  <text x="310" y="120" text-anchor="middle" font-size="12" fill="#1a3d75">(WiFi-connected)</text>

  <line x1="385" y1="100" x2="475" y2="100" stroke="#333" stroke-width="3" marker-end="url(#arrow1)"/>
  <text x="430" y="88" text-anchor="middle" font-size="11" fill="#555">flex 39p</text>

  <rect x="475" y="52" width="90" height="96" rx="6" fill="#f5f5f5" stroke="#333" stroke-width="1.5"/>
  <text x="520" y="105" text-anchor="middle" font-size="15" fill="#222">LCD</text>
</svg>

The whole capture board needs to be **small enough** to fit inside of the existing air quality sensor enclosure. This eliminates the somewhat trivial approach of adding a second 2mm FPC to 2.54mm to the Raspberry Pi Pico 2 W setup of the previous articles: The Pico 2 is too large, and the whole setup would be a total spaghetti of wires.

This pushes me towards a **custom PCB** to shrink the design: Routing the 39 LCD pins between 2 flex connectors to enable passthrough, while tapping some of them to a capture MCU is reasonably easy.

However, due to local regulations, importing WiFi-enabled devices is a headache. So I'll go for a split design: the custom PCB routes the LCD signals to an on-board MCU, with a couple of pins to transmit the data over UART to an off-the-shelf WiFi-enabled board. Just like the [zapper PCB]({% post_url 2026-05-23-zapper-pcb %}), I picked the [Seeed Studio XIAO ESP32-C6](https://wiki.seeedstudio.com/xiao_esp32c6_getting_started/) board: small footprint, and I can just add simple 2.54mm headers on my custom PCB to slot in the Xiao board.

<svg viewBox="0 45 570 190" xmlns="http://www.w3.org/2000/svg" font-family="-apple-system, BlinkMacSystemFont, 'Segoe UI', Helvetica, Arial, sans-serif" style="max-width: 100%; height: auto; display: block; margin: 0 auto;">
  <defs>
    <marker id="arrow2" viewBox="0 0 10 10" refX="9" refY="5" markerWidth="6" markerHeight="6" orient="auto-start-reverse">
      <path d="M 0 0 L 10 5 L 0 10 z" fill="#333"/>
    </marker>
    <marker id="arrow2b" viewBox="0 0 10 10" refX="9" refY="5" markerWidth="6" markerHeight="6" orient="auto-start-reverse">
      <path d="M 0 0 L 10 5 L 0 10 z" fill="#2a5db0"/>
    </marker>
  </defs>

  <rect x="5" y="52" width="150" height="96" rx="6" fill="#f5f5f5" stroke="#333" stroke-width="1.5"/>
  <text x="80" y="98" text-anchor="middle" font-size="15" fill="#222">main board</text>
  <text x="80" y="120" text-anchor="middle" font-size="12" fill="#555">(existing AQ device)</text>

  <line x1="155" y1="100" x2="235" y2="100" stroke="#333" stroke-width="3" marker-end="url(#arrow2)"/>
  <text x="195" y="88" text-anchor="middle" font-size="11" fill="#555">flex 39p</text>

  <rect x="235" y="52" width="150" height="96" rx="6" fill="#eaf2ff" stroke="#2a5db0" stroke-width="1.5"/>
  <text x="310" y="98" text-anchor="middle" font-size="15" font-weight="600" fill="#1a3d75">Capture Board</text>
  <text x="310" y="120" text-anchor="middle" font-size="12" fill="#1a3d75">(MCU w/o WiFi)</text>

  <line x1="385" y1="100" x2="475" y2="100" stroke="#333" stroke-width="3" marker-end="url(#arrow2)"/>
  <text x="430" y="88" text-anchor="middle" font-size="11" fill="#555">flex 39p</text>

  <rect x="475" y="52" width="90" height="96" rx="6" fill="#f5f5f5" stroke="#333" stroke-width="1.5"/>
  <text x="520" y="105" text-anchor="middle" font-size="15" fill="#222">LCD</text>

  <line x1="310" y1="148" x2="310" y2="178" stroke="#2a5db0" stroke-width="2" stroke-dasharray="5 4" marker-end="url(#arrow2b)"/>
  <text x="327" y="167" font-size="11" fill="#2a5db0">UART</text>

  <rect x="235" y="178" width="150" height="50" rx="6" fill="#eafbea" stroke="#2a8c3a" stroke-width="1.5"/>
  <text x="310" y="200" text-anchor="middle" font-size="14" font-weight="600" fill="#1c5e28">XIAO ESP32-C6</text>
  <text x="310" y="218" text-anchor="middle" font-size="12" fill="#1c5e28">(WiFi)</text>
</svg>

### Chip selection

I'll manufacture the PCB with [JLCPCB](https://jlcpcb.com/) again, as the value for money is great and I'm familiar with the flow now. I first wanted to keep an RP2350-based design, or maybe downgrade to the earlier RP2040 to save a few cents. To avoid SPI-NOR cost, I would have gone for a flashless design, so the ESP32 would program the RP internal RAM on boot (the few seconds delay would be acceptable).

However, those parts are a bit more expensive to manufacture as they require 2 "extended" parts in the JLCPCB catalogue: The MCU itself, and the recommended oscillator (extended parts come at a fixed loading fee per part).

I looked at JLCPCB "basic"[^1] part catalogue further, and they had 2 STM32 parts available, an F0 and F1. The F0 was probably too tight (RAM and compute power), and wasn't drastically cheaper, so I picked the STM32F103C8T6.

| | RP2350 | RP2040 | STM32F103C8T6 |
|---|---|---|---|
| CPU | 2x Cortex-M33 @ 150MHz | 2x Cortex-M0+ @ 133MHz | 1x Cortex-M3 @ 64MHz (internal oscillator) |
| Flash | none | none | 64KB |
| RAM | 520KB | 264KB | 20KB |
| PIO | Yes | Yes | No |
| JLCPCB catalogue | Extended | Extended | "Basic"[^1] |

This introduces a bunch of new constraints though. The main issue here is not really the processor speed, but the very small RAM content and lack of flexible PIO --- getting the capture to work will require extensive work. But, hey, I guess that's what makes it more fun.

### Porting the capture firmware to STM32F1

This was probably the most frustrating part of this project, and where Claude struggled the most.

The lack of flexible PIO is painful. But by routing `WR` to the correct pin(s), we can use timers to capture an edge of the `WR` pin to control DMA transfer and read the GPIO pin status. Again, Claude (and myself) did a lot of trial and error to select the right edge and filtering, especially given the somewhat different `DC` pin transition time.

The next issue is that we are not able to put all the required 18 data pins (16-bit bus + `DC` + `CS`) on one single PA or PB port, as they are 16-bit each. Then, DMA transfers can only read from a single 16-bit register (or multiple if contiguous: not the case here). So we need to set up 2 DMA transfers to 2 different ring buffers to pull both PA and PB values.

Within those PA/PB ports, we also cannot guarantee the 16-bit data bus is even in a sensible order (some pins are reserved/fixed function, and PCB routing later will be easier if we reorder pins). Given that STM32 compute power is limited, I decided to pass unordered LCD data over UART. The ESP32 is powerful enough to reorder bits as required for rendering/processing.

#### Performance

After a bunch of back and forth, some basic capture worked fine, but quite a few of the LCD drawing operations ended up getting lost. With hindsight, I think this is where I should have stepped back, and think a bit more from first principles instead of just beating at things with Claude.

I first thought CPU performance was a concern. The Rust code generated by Claude was sometimes sub-optimal, with a bunch of unnecessary memory copies to fit into nice software abstractions like queues. This is totally acceptable for non-embedded/non-performance critical applications, but not here. I found ways to optimize the code though, sometimes bypassing `embassy-stm32`, but this feels like writing C in Rust. It would be a nice follow-up project to get `embassy-stm32` to work better in my case.
```rust
let pa_ndtr = pac::DMA1.ch(4).ndtr().read().ndt() as usize;
...
let pa = unsafe { core::ptr::read_volatile(pa_base.add(pa_i)) };
```

I also had doubts about whether the STM32 memory bus could withstand 2 relatively high speed DMA + a CPU core running at the same time, but that seems fine in the end.

It turns out most of the losses were due to buffer overflows in the UART transmission. On average, the data transferred to the LCD is relatively small, but there are fast bursts that will saturate a 921600 bps UART line if we're not careful.

For example, the display init sequence starts by writing 153600 words of zeros over the whole display, at about 3 MB/s sustained speed.
```text
0x13 NORMAL_MODE_ON
0x29 DISPLAY_ON
0x2a SET_COLUMN_ADDRESS [4] 0000 0000 0001 003f (0..319)
0x2b SET_ROW_ADDRESS [4] 0000 0000 0001 00df (0..479)
0x2c MEMORY_WRITE [153600] 0000 0000 0000 0000 0000 0000 …
```

#### Compression protocol

Clearly, some compression is necessary to address the UART bottleneck, and I ended up designing a [protocol](https://github.com/drinkcat/aq-lcd-grab/blob/main/docs/wire_protocol.md) with Claude's help. The Raspberry Pico also required some basic compression to work, and I started with some basic run-length encoding:

- **tag = 0x02 — run-length event (6-byte body)**

  *N consecutive WR edges produced identical (pa, pb). N ≥ 2.*

  ```
  [0x02] [n_lo] [n_hi] [pa_lo] [pa_hi] [pb_lo] [pb_hi]
         └── u16 LE ─┘ └── u16 LE ───┘ └── u16 LE ───┘
  n = number of WR edges represented (≥ 2, ≤ 65535).
  ```

  *(A run longer than 65535 edges is split into multiple
     tag=0x02 frames back-to-back.)*

This works well for large uniform paints, but less so for individual digit updates that alternate between different colors. The RP could get away with less optimal encoding as it could easily fit 32kB DMA buffer and a 4kB UART buffer. The STM32 could only fit around half of that: 16kB DMA and 2kB UART (leaving only 2kB available for the rest). Those would easily overflow when multiple digits are updated in quick succession.

The key here was to recognize that digit updates are only composed of 2 different colors (e.g. black on green background, white on black background), so we can encode them efficiently:

- **tag = 0x03 — alternating runs (variable body)**

  *A sequence of same-sample runs that strictly alternate between two distinct sample values. Optimized for B&W pixel rendering on the display where the bus toggles between 0xFFFF and 0x0000 every few-to-tens of WR edges — naïve RLE wastes ~7 bytes of header per toggle; this frame collapses each toggle to 1 byte at the cost of one shared 9-byte header per sequence.*

  ```
  [0x03] [pa_lo pa_hi pb_lo pb_hi]  [pa_lo pa_hi pb_lo pb_hi]  [run_lens...] [0x00]
       └──── val_a u32 LE ─────┘  └──── val_b u32 LE ─────┘  └── u8 each ─┘
  ```

  *(`0x00` terminates the run-length list --- 0 is never a valid run length, so it's unambiguous.)*

<figure style="text-align: center">
  <iframe width="315" height="560" src="https://www.youtube.com/embed/CnENXiT2JiE" frameborder="0" allowfullscreen></iframe>
  <figcaption>Notice the large uniform paints on device reset, and how individual digits are only comprised of 2 colors.</figcaption>
</figure>

---

Despite the heavy constraints, and painful trial and errors, the STM32 implementation works well enough (full code [here](https://github.com/drinkcat/aq-lcd-grab/tree/main/firmware-stm32)), and I also implemented the digit decoding parts in ESP32, but we'll cover this briefly later.

We'll move on to the Claude-assisted PCB design next.

[^1]: Technically, these are "promotional extended" parts, but they are billed just like "basic" parts.
