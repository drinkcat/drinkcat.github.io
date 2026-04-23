---
layout: post
title: "Triggering Panasonic Lumix from Arduino"
date: 2013-02-20 19:00:00+08:00
categories: photo blogspot
---

As mentioned in the post about [how to build a remote trigger](/blog/2012/12/16/simple-remote-trigger-for-panasonic/), we can also use an electronic device, such as an Arduino, to trigger the camera.

There are several way of accomplishing this, a simple MOSFET may do the trick, but I decided to use a optocoupler. The optocoupler electrically separates the Arduino and camera circuitry: This decreases risks of damaging the camera because of incorrect wirings on the Arduino side.

### Camera detection circuit

If you remember from [this post](/blog/2012/12/16/simple-remote-trigger-for-panasonic/), the triggering mechanism for Panasonic cameras is a little unusual: The resistance across 2 pins defines the trigger status: around 41.1kOhm at rest, 5.1kOhm to pre-focus the camera, 2.2kOhm to trigger a shot.

Understanding a bit more of the detection circuit inside the camera is useful here. This can be done by measuring the voltage across the 2 pins in different combinations:

<table border="1" cellpadding="5px" style="border-collapse: collapse; margin: auto;">
<tbody>
<tr>
<td align="center"><b>State</b></td>
<td align="center"><b>Measured resistance</b></td>
<td align="center"><b>Voltage measured</b></td>
</tr>
<tr>
<td>Open circuit</td>
<td align="right">(infinite)</td>
<td align="right">3.08V</td>
</tr>
<tr>
<td>Trigger present</td>
<td align="right">~41.4 kOhm</td>
<td align="right">2.49V</td>
</tr>
<tr>
<td>Focus</td>
<td align="right">~5.0 kOhm</td>
<td align="right">1.05V</td>
</tr>
<tr>
<td>Trigger</td>
<td align="right">~2.1 kOhm</td>
<td align="right">0.56V</td>
</tr>
</tbody></table>

We can make an educated guess on the detection circuit used by the camera, making use of a simple voltage divider, and reading out the voltage drop on the remote trigger.

{% include img.html src="/images/triggering-panasonic-lumix-from-arduino/arduino-trigger.png" alt="Possible detection circuit on the camera (the camera resistor may be connected on the ground side, and may be more complicated to avoid the need for an analog to digital converter)" %}

Obtaining V<sub>camera</sub> is straightforward: it is the open circuit voltage. We can guess R<sub>camera</sub> entering resistances R<sub>trigger</sub> and V<sub>detect</sub> in the following equation:

<div style="text-align: center;"><img src="/images/triggering-panasonic-lumix-from-arduino/res.png" alt="equation"></div>

Moving terms around, we get:

<div style="text-align: center;"><img src="/images/triggering-panasonic-lumix-from-arduino/res2.png" alt="equation"></div>

For the 3 values above, we get, respectively, R<sub>camera</sub> = 9.8, 9.7 and 9.45 kOhm. So we have a resistor around 9.5 kOhm on the camera side.

### Triggering circuit

Now comes the triggering circuit itself, as shown in the next figure.

{% include img.html src="/images/triggering-panasonic-lumix-from-arduino/arduino-trigger2.png" alt="Triggering circuit (optocoupler drawing from Wikipedia). Note: The polarity of the camera connector is important: if you reverse the pins, no current can flow through the the optocoupler (no damage to the camera, it just won't work). In my case, R_led = 200 Ohm, R_p = 36 kOhm, R_t = 0" %}

The optocoupler provides a separation between the Arduino circuit on the right, and the camera on the left. When Pin 2 on the Arduino is grounded, no current flows through the LED, and the transistor on the right is in the "off" state, i.e. it does not let any current through. When a voltage is applied to Pin 2 (digital 1 = 5V), the led shines on the transistor, allowing current through it ("on" state).

In this case I used a [TLP627](http://www.toshiba.com/taec/components2/Datasheet_Sync/207/4346.pdf) optocoupler. The easiest parameter to compute is R<sub>led</sub>: As indicated in the datasheet, the forward voltage of the LED is 1.15V, and we want a maximum of 25mA. A simple calculation (or an [online tool](http://led.linear1.org/1led.wiz)) will tell you that a 200 Ohm resistor is required.

What we want here is the circuit to have a 44kOhm resistance (~2.5V) in the rest state, and drop the resistance to 2 kOhm (~0.5V) when the optocoupler is on. A simple solution would be to set Rp to 42 kOhm, and Rt to 2.1 kOhm. When the optocoupler is off, the total resistance is 44 kOhm, when it is on, the optocoupler shorts Rp, and the total becomes 2.1 kOhm.

If you try this combination, you will realise that it is not enough to trigger the camera: the voltage drops, but only enough to pre-focus the camera, that is a voltage around 1 V.

What is happening? In reality, the optocoupler is not an ideal switch: in the "on" state, it has a small, but finite, voltage drop. In my case, this voltage drop is around 0.5 V, just what we need to trigger the camera. Therefore, we can get rid of Rt, and simply set Rp to 44 kOhm. I did not have a 44 kOhm resistor, but I realised that 36 kOhm also works, so I used that.

I'm not 100% sure of the theory behind, but I suspect that the transistor can only conduct current when it is close to its saturation voltage. However, for the [TLP627](http://www.toshiba.com/taec/components2/Datasheet_Sync/207/4346.pdf), the saturation voltage is anywhere between 0.3 to 1.2V, according to the datasheet. Therefore, you may need to adjust Rp depending on the individual properties of your optocoupler, or you may not be able to trigger the camera at all.

[These guys](http://dev-control.com/piclaps-diy.php) use a [LTV-355T](http://www.image.micros.com.pl/_dane_techniczne_auto/oopc355tltv.pdf) optocoupler, with Rp=1 kOhm. I'm not sure what makes that optocoupler better, if it is better. I did not use it because it is only available as a surface mount component, while the TLP627 is available as 4DIP package, compatible with breadboards and stripboards.

If there is an electrical engineer in the room, maybe that person can shed some light on this. I stopped thinking about it, because the circuit works, and that is all I need ,-)

{% include img.html src="/images/triggering-panasonic-lumix-from-arduino/P1160496-sm.JPG" alt="Finished circuit on a breadboard, connected to a Freeduino (Arduino clone)." %}

### Arduino sample code

The following code will trigger the Arduino every 10 seconds. This is useful for simple time-lapse photography:

```c
int trigger = 2;
int led = 13;

int time = 10; /* time between shots, in seconds */

void setup()  {
  Serial.begin(9600);
  analogReference(INTERNAL);
  pinMode(trigger, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(trigger, LOW);
}

void loop()  {
  for (int i = time-1; i > 0; i--) {
    delay(1000);
  }
  delay(900);
  digitalWrite(trigger, HIGH);
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(trigger, LOW);
  digitalWrite(led, LOW);
}
```

This code "holds down" the shutter for 100 milliseconds, enough to allow the camera to autofocus. It also blinks the Arduino LED on pin 13, to give you a visual feedback that the code is running, in case the camera does not trigger (wrong polarity, etc.).

---

*Imported from "[Random ideas of a drinking cat](https://drinkcat.blogspot.com/)" blog.*
