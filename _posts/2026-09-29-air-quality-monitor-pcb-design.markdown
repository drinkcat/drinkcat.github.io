---
layout: post
title: "Grabbing LCD display: PCB design, layout, routing"
date: 2026-09-29 11:00:00+08:00
categories: embedded
image: /images/aq-monitor/blue-wire-preview.jpg
excerpt: >-
  We got a basic LCD grabber working, and we selected chips and a general design for the system, it's now time to design a PCB. I tried a flow where Claude works on design doc for the PCB, creates a SKiDL script to describe the circuit, generate a netlist, and then me (human) does the PCB layout.
---

This is the third part of my LCD display grabbing adventure, the first part is [here]({% post_url 2026-07-12-air-quality-monitor-lcd-grab %}), and the second part is [here]({% post_url 2026-08-19-air-quality-monitor-esp32-stm32 %}).

We got a basic LCD grabber working, and we selected chips and a general design for the system, it's now time to design a PCB. I tried a flow where Claude works on design doc for the PCB, creates a SKiDL script to describe the circuit, generates a netlist, and then this human does the PCB layout.

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

## AI-first PCB design

In a [previous post]({% post_url 2026-05-23-zapper-pcb %}), I went for a more "classical" PCB design flow, starting with schematics (which I failed to generate with Claude), then moving on to PCB layout.

Here, I tried a flow that looks a bit more like software engineering:
I started with a [PCB design spec](https://github.com/drinkcat/aq-lcd-grab/blob/main/docs/pcb_spec.md)[^1], trying to finalize as much as possible of the design with Claude prompts, letting it record questions at the end of the document, and slowly going through them together. I also fed it datasheets and manuals of the STM32 chip, to help it with pin assignment, and necessary external circuits (reset, filtering caps...).

I then moved on to ask Claude for a SKiDL script for the circuit, iterating back to the design doc as required. [SKiDL](https://github.com/devbisme/skidl) is basically a Python description of the netlist. For example, this describes the MCU and status LED:

<div class="small-code" markdown="1">

```python
# =============================================================================
# STM32F103C8T6 (LQFP-48) — capture MCU
# =============================================================================
# Pin map verified against DS5319 Table 5 (see docs/pcb_spec.md "Pin
# map (proposal)"). LQFP-48 footprint from KiCad stock library; no
# exposed pad. The F103 LQFP-48 pinout matches F030 LQFP-48 for every
# pin we use, except: pin 1 is VBAT (not VDD), pin 35/36 is an extra
# VSS/VDD pair (replaces F030's PF6/PF7), and PB2 is the BOOT1 latch
# (must be held low at reset; cannot drive the LED).
U1 = Part("MCU_ST_STM32F1", "STM32F103C8Tx",
          footprint="Package_QFP:LQFP-48_7x7mm_P0.5mm",
          value="STM32F103C8T6",
          ref="U1",
          tag="U1_STM32")

# =============================================================================
# Status LED on STM32 PC13 (pin 2)
# =============================================================================
# Matches the Blue/Black Pill dev-board pinout: same pin, same
# 3V3→1 kΩ→anode / cathode→GPIO active-low topology. So a firmware blink on
# PC13 lights both the dev-board LED and ours, no per-target #ifdef.
#
# PC13 is in the F103 backup domain (low-drive, 3 mA max sink/source,
# 2 MHz toggle, not 5V tolerant per DS5319). All within spec for a
# <2 mA LED at sub-Hz rates.
LED_STATUS = Net("LED_STATUS")
R_LED = R("1k", "R3", "R_LED_STATUS")
D_LED = Part("Device", "LED",
             value="KT-0603R",
             footprint="LED_SMD:LED_0603_1608Metric",
             ref="D1",
             tag="D1_LED_STATUS")
R_LED[1] += P3V3
R_LED[2] += D_LED[2]      # anode (pin 2)
D_LED[1] += LED_STATUS    # cathode (pin 1) -> PC13
U1[2] += LED_STATUS
```

</div>

The comments are quite insightful about how Claude went about selecting pins, trying to keep compatibility with my prototyping rig where possible, making sure current rating works out, and making it possible to downgrade to STM32F0 where possible.

### Netlist to PCB layout

The SKiDL script can then be run to generate a netlist, that can be imported into KiCad.

I immediately moved on to PCB layout, and ended up doing most of it by hand: partly because I had never done this, partly because I didn't hear much good things about AI-based automated tools (at least at the time, 4-5 months ago).

Unlike a normal EE flow, I did not spend time drawing schematics, this is probably acceptable for a single-person project: I did review and design/pinout changes as I placed and routed components. Iterating from the design doc/SKiDL is actually fairly easy, prompt Claude to update both, regenerate the netlist, then just press File->Import->Netlist in KiCad: KiCad then smartly adjusts connections and components, and in the worst case, DRC checks will fail.

{% include img.html src="/images/aq-monitor/pcb-layout-annotated.png" svg="aq-monitor/pcb-layout-annotated.svg" width="70%" alt="PCB layout: STM32F103, ESP32-C6 XIAO footprint, flex connectors to the LCD and main board, status LED, power/reset and SWD headers" %}

### Display connector lane and tap routing

Doing most of the routing by hand was somewhat okay, and not too repetitive for this human. However, connecting the 2 39-pin connectors (J1 and J2 above), with the required vias to avoid violating DRC rules, and the required 19 taps to the STM32 became extremely tedious.

Basically, routing the 39 pins between the connectors is a repeated pattern. We route the top layer pins (the connectors footprints) to 2 other layers (we have 4 in total). The tricky bit is that we need to add "kinks" to the traces around the vias to avoid violating design rules.

We also need to add additional vias to tap the lines and connect them to the STM32. Similarly, the vias require kinks in the lines in the 2 other layers.

I asked Claude to help, and it came up with this [horrible horrible thing](https://github.com/drinkcat/aq-lcd-grab/blob/main/pcb/route_lcd_bus.py), basically doing some glorified string replacement directly into the PCB layout file. Iterating on this was also reasonably easy: Run script, press File -> Revert on KiCad, inspect, laugh at Claude silliness, ask it to fix stuff, iterate.

The exact placement of the vias was decided by me. I swapped pins on the STM32 as required to make routing easier, as we have facilities in the software to swap bits of the captured data anyway.

<div class="img-row">
{% include img.html src="/images/aq-monitor/flex-connector-fanout.png" alt="Close-up of J1 (main board flex connector): vias to fan out the two staggered pad rows" width="100%" %}
{% include img.html src="/images/aq-monitor/lcd-bus-taps.png" alt="Close-up of the taps: bus lines routed to the STM32 on the top layer (red)" width="100%" %}
</div>

The final, routed, PCB, looks like this. This is a 4-layer PCB (2 layer is definitely not possible).
 
{% include img.html src="/images/aq-monitor/pcb-routed.png" alt="Overall PCB, fully routed -- ground layer not displayed" width="80%" %}

## Manufacturing and assembly

Confidently enough, I then pressed the button on JLCPCB website, payed a whopping 21.16 USD for 5 boards (with shipping), and waited a week.

Then I got the boards, assembled everything, and started scratching my head... Something looked very wrong in the capture, with such strange behaviour that I went down a rabbit hole trying to figure out if the STM32 I used for prototyping was a genuine part (and the presumably genuine JLCPCB part was actually underperforming).

Turns out, I had swapped 2 pins (CS and WR)... WR is the worst pin to swap, as it is the trigger for the data capture. Luckily though, CS is not actually used, so I just cut that trace, and jumped a wire. Claude wrote a small [errata](https://github.com/drinkcat/aq-lcd-grab/blob/main/docs/pcb_spec.md#v1-errata), and well, I guess it's my fault, or, anyway, I'm ultimately responsible!

{% include img.html src="/images/aq-monitor/blue-wire.jpg" alt="Claude can't rework your board! At least this was fun!" width="60%" %}

And finally, a fully working version. The ESP32-C6 is running a web server you can see on the laptop, displaying a mirror of what is shown on the display, with the values decoded.

{% include img.html src="/images/aq-monitor/working-prototype.jpg" alt="Working prototype: the grabbed LCD contents mirrored in a browser" width="60%" %}

The next post will look at the details of the implementation, and home assistant integration.

[^1]: Partially outdated, as often with design docs.
