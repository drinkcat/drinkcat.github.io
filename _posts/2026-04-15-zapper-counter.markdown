---
layout: post
title: "From zap! to dashboard"
date: 2026-04-15 14:09:32+01:00
categories: embedded
description: "I installed a bug zapper in my house. I know it zaps, but something is obviously missing: A dashboard! How many mosquitoes do I catch? I need statistics."
excerpt: "So I installed a bug zapper in my house, in an attempt to control the mosquito population. It's basically a blue light surrounded by a metal grid, charged at high voltage, that discharges when something gets in between. I know it zaps, the noise and flash are obvious, but something is obviously missing: A dashboard! How many mosquitoes do I catch? I need statistics."
---

So I installed a bug zapper in my house, in an attempt to control the mosquito population[^1]. It's basically a blue light surrounded by a metal grid, charged at high voltage, that discharges when something gets in between. I know it zaps, the noise and flash are obvious, but something is obviously missing: A dashboard! How many mosquitoes do I catch? I need statistics.

{% include img.html src="/images/zap/zap-tower.jpg" alt="Zap tower" width="20%" %}

This was also a good opportunity to see how well vibe-EEing works (clue: I mostly have no idea what I'm doing -- and neither does Claude, but in all fairness the end result is probably not so different from what I could have done with web searches picking bits and pieces of circuits).

### Discarded alternatives

I considered a number of alternatives, from web searches to friends' suggestions. It would probably be easiest to detect the discharge by tapping the circuit inside the tower directly, but I want something that doesn't require disassembly.

Looking at external methods to record the events:

  - Recording the zap **sound** with a microphone: very doable, but I didn't want something that always listens, and signal processing didn't seem fun.
  - Recording the light **flash**: might be doable, unsure how it'd do under different lighting, and we'd need multiple sensors around the tower. But maybe doable.
  - **Weighing** caught mosquitoes: impossible, each of them weighs in the order of a few micrograms.

### Vibe-EEing experiments

That leaves something that detects the **fast electrical discharge**. The grid actually radiates quite a bit of energy during the zap.

I did not have spare mosquitoes at hand, so the "testing" method looks like this:
 - Turn on the tower, wait a bit for the grid to charge, turn it off.
 - Insert a screwdriver between the wires of the grid, zap.

I tried a few things with my portable oscilloscope, a simple antenna monopole caught a strong spike, but also a _LOT_ of noise. I thought a wire loop would work better, and, after a bit of back and forth with my best friend Claude:

> **Me:** I... found an AM antenna loop, is this a good idea
>
> **Claude:** [long reply about antenna frequency tuning] <br/>
> Just try it as-is on the scope first — I'd expect a very clean detectable burst with good SNR, probably better than a homebrew air-core loop. It's a good find.

<div class="img-row">
{% include img.html src="/images/zap/zap-tower-am.jpg" alt="AM antenna loop" width="50%" %}
{% include img.html src="/images/zap/zap-tower-am-scope.jpg" alt="Zap signal oscilloscope, note the 633V peak-to-peak" width="100%" %}
</div>

And, it worked! Perhaps a bit too well, but hey Claude found it hilarious, seemed really confident in the quality of my tiny handheld oscilloscope[^2], and made safety claims I'd rather not rely on:

> **Me:** Err 600 V peak to peak. About 2us long but ringing down to zero in 10us. Is this safe?
>
> **Claude:** 600Vpp is surprising but not shocking (no pun intended) given the ferrite core amplification — the ferrite concentrates flux and the tuned resonance further boosts the voltage. *[note: no ferrite core in this antenna]* <br/>
> Let's break down the safety question: <br/>
> **The oscilloscope is fine** — you're well within typical probe ratings (most handle 300–600V CAT I), so no concern there. <br/>
> **Don't touch the leads while the zapper is on** — 600V open-circuit is enough to feel very unpleasant even if the energy (microjoules, given the 10µs duration) is tiny. It won't kill you but it will surprise you.

Obviously, I then moved on to a smaller loop (using some makeshift coax cable to reduce noise), and the signal is more reasonable, and very consistent. I left the oscilloscope overnight in persistence mode, and a few mosquitoes got caught.

<div class="img-row">
{% include img.html src="/images/zap/zap-tower-loop.jpg" alt="Homebrew loop antenna, using a broken USB-C cable as coax..." width="50%" %}
{% include img.html src="/images/zap/zap-tower-loop-scope.jpg" alt="A few (real) zaps recorded overnight -- very consistent!" width="100%" %}
</div>

I also verified that switching _on_ or _off_ the tower manually doesn't cause anything measurable -- the grid takes about a second to charge fully, and naturally discharges in a few seconds.

### Clamping circuit

So the next step is to design a circuit to connect that signal to a microcontroller -- we need to clamp the voltage between 0 and 3.3V or so.

This is where Claude showed serious limitations, and asking relevant questions was essential. A long bit of confusion later --- maybe this is a bit too high frequency for a cheap op-amp? No, I can't use a "normal" diode with 0.6-0.7 V forward voltage as the pin is only presumably rated for -0.3V to 3.6V. No, the Schottkys do not need to withstand large reverse voltage as the other diode conducts --- we ended up with this:

```
Coil → [1kΩ] → D1 1N5817 to 3.3V
             → D2 1N5817 to GND
             → GPIO
```

Which I _think_ is sensible, the resistor limits the current going through the diodes, and the diodes will conduct when the voltage is higher than 3.6V (or lower than -0.3V). The extra 0.3V on each side should be fine for a GPIO pin, the datasheet for the ESP32C6 I'm going to use isn't terribly clear though.

I then tried to get Claude to generate schematics -- hilarity ensued. I now know I should have asked for a netlist (maybe using SKidl?)?

{% include img.html src="/images/zap/zap-vibe-eeing-is-the-future.jpg" alt="Vibe-EEing is the future" width="75%" %}

So I drew something by hand instead. I also added a second resistor between the diodes and the GPIO pin for extra protection[^3]. And a capacitor on 3.3V -- I think this should help still clamp the signal if the board is switched off (probably not needed). And a pull-down on the GPIO pin to prevent the signal from floating to weird levels if the antenna is not connected (I should learn about diode leakage current...).

{% include img.html src="/images/zap/zap-schematics.png" alt="Hand-drawn schematics (drinkcat: 1; Claude: 0)" width="100%" %}

### ESP32 integration

I picked an ESP32-C6 devkit board, and used this as an opportunity to see what all the embedded Rust rage is about. I first made a [`esp-idf-svc`](https://github.com/esp-rs/esp-idf-svc) based firmware, that builds on top of Espressif C/FreeRTOS `esp-idf` libraries, then rewrote in the Espressif-supported, native-Rust [`esp-hal`](https://github.com/esp-rs/esp-hal) framework. Claude is much better at vibe-coding than vibe-EEing, and makes it really fast to put things together.

The [code](https://github.com/drinkcat/zappy-esp/blob/main/src/bin/main.rs)[^4] is reasonably simple, and boils down to:
 - Connect to Wifi (hardcoded SSID in firmware for now)
 - Wait for rising edge on the input pin (pulse is long enough to be easily caught, a little bit of debouncing is necessary though)
   - Blink LED as immediate feedback.
   - Send a message to a cloud service via Wifi/HTTP.

I must say I found the `embassy` abstraction of interrupt handler into an async event really cool, then we can just publish to a pubsub queue that the blink and message handler tasks can pull from.
```rust
    loop {
        zap_pin.wait_for_rising_edge().await;
        info!("Zap!");
        zap_pub.publish_immediate(());
        Timer::after(Duration::from_millis(100)).await; // debounce
    }
```

<figure style="text-align: center">
  <iframe width="315" height="560" src="https://www.youtube.com/embed/IUfh4uln3fk" frameborder="0" allowfullscreen></iframe>
  <figcaption><em>At least it can detect screwdrivers...</em></figcaption>
</figure>

### Dashboard!

I looked a bit at Blynk and [Thingsboard](https://thingsboard.io/)[^5], and the latter seemed a little easier to use, with possibility of self-hosting if I wanted to, and longer data retention (if I understand correctly).

The only small gotcha was to make sure the ESP32 sends an empty message every few minutes so that the dashboard does not think the device is offline, as we only very rarely send events.

Making a dashboard out of the zap events was really simple with the UI.

{% include img.html src="/images/zap/zap-dashboard.png" alt="Zap dashboard" width="100%" %}

[^1]: For some reason Google AI overview says those are useless and harmful to good insects. Possibly in US-context, and when placed outdoors? Anyhow, I live in dengue country, so I would rather not get bitten by mosquitoes, and I'm sorry for the other insects this catches.

[^2]: I bought the Alientek DM-40C for these experiments, it's great to use! But... I'm not sure how much I trust any of the voltage ratings...

[^3]: _Presumably_ the microcontroller should also have some sort of internal ESD protection -- so if the signal is not too strong we might be able to rely on this -- probably not the best idea.

[^4]: `esp-idf-svc` code on [this branch](https://github.com/drinkcat/zappy-esp/blob/esp-idf/src/main.rs).

[^5]: I considered Home Assistant but I do not have a server at home, so I wanted something cloud based.