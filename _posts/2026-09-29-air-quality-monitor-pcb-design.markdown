---
layout: post
title: "Grabbing LCD display: PCB design, layout, routing"
date: 2026-09-29 11:00:00+08:00
categories: embedded
image: TODO
---

This is the third part of my LCD display grabbing adventure, the first part is [here]({% post_url 2026-07-12-air-quality-monitor-lcd-grab %}), and the second part is here (TODO).

We got a basic LCD grabber working, and we selected chips and a general design for the system, it's now time to design a PCB. I tried a flow where Claude works on design doc for the PCB, creates a SKiDL script to describe the circuit, generate a netlist, and then me (human) does the PCB layout.

(add general schematics from last post here)

### AI-first PCB design

In a previous post (TODO LINK), I went for a more "classical" PCB design flow, starting with schematics (which I failed to generate with Claude), then moving on to PCB layout.

Here, I tried a flow that looks a bit more like software engineering.
I started with a [PCB design spec](link to pcb_spec.md) (footnote: partially outdated, as often with design docs), trying to input as much as possible of the design into Claude prompts. I also fed it datasheets and manuals of the STM32 chip, to help it with pin assignment, and necessary external circuits (reset, filtering caps...).

I then moved on to ask Claude for a SKiDL description[is this the right word] of the circuit (iterating back to the design doc as required).

SKiDL[add link] is basically a Python description of the netlist, for example, the the MCU and the status LED:

```
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

The comments are quite insightful about how Claude went about selecting pins, trying to keep compatibility with my prototyping rig where possible, making sure current rating works out, and making it possible to downgrade to STM32F0 where possible.

### Netlist to PCB layout

The SKiDL script can then be run to generate a netlist, that can be imported into KiCad.

I immediately moved on to PCB layout, and ended up doing most of it by hand: partly because I had never done this, partly because I didn't hear much good things about AI-based automated tools (at least at the time, 4-5 months ago).

Unlike a normal EE flow, I did not spend time drawing schematics, this is probably acceptable for a single-person project: I did review and design/pinout changes as I placed and routed components. Iterating from the design doc/SKiDL is actually fairly easy, prompt Claude to update both, regenerate the netlist, then just press File->Import->Netlist in KiCad: KiCad then smartly adjusts connections/components.

{% include img.html src="/images/aq-monitor/pcb-layout-annotated.png" alt="PCB layout: STM32F103, ESP32-C6 XIAO footprint, flex connectors to the LCD and main board, status LED, power/reset and SWD headers" %}

### Display connector lane and tap routing

Doing most of the routing by hand was somewhat okay, and not too repetitive for human self. However, connecting the 2 39-pin connectors, with the required vias to avoid violating DRC rules, and the required 19 taps to the STM32 became extremely tedious.

I asked Claude to help me with this, and it came up with this horrible horrible thing (https://github.com/drinkcat/aq-lcd-grab/blob/main/pcb/route_lcd_bus.py), basically doing some glorified string replacement directly into the PCB layout file. Iterating on this was also reasonably easy: Run script, press File -> Revert on KiCad, inspect, laugh at Claude silliness, ask it to fix stuff, iterate.

{% include img.html src="/images/aq-monitor/lcd-bus-routing.png" alt="Flex connector layout: LCD connector (top) to main board connector (bottom)" width="40%" %}
 
{% include img.html src="/images/aq-monitor/pcb-routed.png" alt="Overall PCB, fully routed" width="80%" %}

### Manufacturing and assembly




{% include img.html src="/images/aq-monitor/bodge-wire.jpg" alt="Assembled board, with a bodge wire" width="60%" %}

{% include img.html src="/images/aq-monitor/working-prototype.jpg" alt="Working prototype: the grabbed LCD contents mirrored in a browser" width="60%" %}
