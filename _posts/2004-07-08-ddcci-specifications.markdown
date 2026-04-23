---
layout: post
title: "DDC/CI specifications"
date: 2004-07-08 00:00:00+00:00
categories: linux
---

*(Note: imported verbatim from old website)*

Reverse-engineered using this [method](/blog/2004/07/12/ddcci-reverse-engineering-howto/).

## General data transfer

The DDC bus is nothing else than an I2C bus.

DDC2 informations, like monitor supported refresh rates (EDID), are transmitted using the I2C address 0x50.

DDC/CI packets are transmitted using the I2C address 0x37.

For more informations on I2C bus, see [I2C specifications](http://www.semiconductors.philips.com/buses/i2c/) at Phillips.

### Timing

Any application using the DDC/CI bus should make a pause of at least 30ms between every packet.

### Write packet

| Data | Description |
|------|-------------|
| 0x6E | I2C address : 0x37, writing |
| 0x51 | Sub-address. |
| 0x80 OR **n** | Last 4 bits indicates the number of following bytes (excluding checksum) |
| ... | **n** bytes |
| Checksum | Simple XOR of all preceding bytes (including the first) |

### Read packet

First, send a write packet, then, when reading, the monitor returns this:

| Data | Description |
|------|-------------|
| 0x6F | I2C address : 0x37, reading |
| 0x6E | ACK of the packet previously written. |
| 0x80 OR **n** (sometimes, simply **n**) | Last 7 bits indicates the number of following bytes (excluding checksum) |
| ... | **n** bytes |
| Checksum | XOR of all preceding bytes (including the first), expect the second, which is replaced by the subaddress (0x51). |

## Commands

### Read a value

First, send a 2 bytes write packet:

| Data | Description |
|------|-------------|
| 0x01 | Read a value |
| 0xXX | Index of the value to read (see below for monitor-specific indexes) |

Then, read the answer (8 bytes read packet):

| Data | Description |
|------|-------------|
| 0x02 | Value follows |
| 0x00 | NULL-byte |
| 0xXX | Index of the value which is read |
| 0x00 | NULL-byte |
| 0x00 | NULL-byte |
| 0xZZ | Maximum value (usually 0xFE) |
| 0x00 | NULL-byte |
| 0xYY | Current value |

### Modify a value

Send a 4 bytes write packet:

| Data | Description |
|------|-------------|
| 0x03 | Change a value |
| 0xXX | Index of the value to change (see below for monitor specific indexes) |
| 0x00 | NULL-byte |
| 0xYY | New value, in the range 0x00-0xFE |

## Monitor specific indexes

### Mitsubishi Diamond Pro 2060u

#### Monitor informations

Manufacturer: MEL
Model: 4513

| Index | Description | Index | Description |
|-------|-------------|-------|-------------|
| 0x00 | Degauss (value must be 0x01) | 0x26 | Pincushion balance |
| 0x?? | Auto-size | 0x46 | Top pincushion amplitude |
| 0x?? | Reset factory settings | 0x48 | Top pincushion balance |
| 0x10 | Brightness | 0xE6 | Center pincushion amplitude (value 0x00-0x01 ???, not working) |
| 0x12 | Contrast | 0x4A | Bottom pincushion amplitude |
| 0x16 | Red | 0x4C | Bottom pincushion balance |
| 0x18 | Green | 0x3A | Vertical linearity |
| 0x1A | Blue | 0x3C | Vertical linearity balance |
| 0x22 | Width | 0x28 | Horizontal static convergence |
| 0x32 | Height | 0x38 | Vertical static convergence |
| 0x30 | Vertical position | 0x56 | Moire cancel |
| 0x20 | Horizontal position | 0xE8 | Top left screen purity |
| 0x24 | Pincushion amplitude | 0xE9 | Top right screen purity |
| 0x42 | Pincushion phase | 0xEA | Bottom left screen purity |
| 0x40 | Keystone balance | 0xEB | Bottom right screen purity |

### Others?

If you have other monitors, you can find these values using [this method](/blog/2004/07/12/ddcci-reverse-engineering-howto/).

---

*DDC is a trademark owned by [VESA](http://www.vesa.org/) (Video Electronics Standard Association).*

*I2C is a trademark owned by Philips.*