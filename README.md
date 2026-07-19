# AlashUartMP3

> 🌐 **English** | [Русский](README_ru.md)

Arduino library for controlling UART-based MP3 player modules (e.g. boards built on the JQ8400 chip).

## Installation

* Download: https://github.com/Alash-electronics/AlashUartMP3
* In the Arduino IDE: Sketch → Include Library → Add .ZIP Library…
* Select the downloaded `AlashUartMP3.zip`
* Examples are now available under File → Examples → AlashUartMP3 → HelloWorld

## Usage

AlashUartMP3 works with any object that implements the `Stream` interface — a hardware `Serial` port, `SoftwareSerial`, or an ESP32's extra hardware UARTs.

### Basic wiring

```cpp
#include <AlashUartMP3.h>
AlashUartMP3 mp3(mySerial);
```

### ESP32

```cpp
#include <AlashUartMP3.h>
AlashUartMP3 mp3(Serial2);
```

### Other initialization examples

```cpp
#include <AlashUartMP3.h>
AlashUartMP3 mp3(mySerial); // SoftwareSerial

#include <AlashUartMP3.h>
AlashUartMP3 mp3(Serial);   // hardware Serial

#include <AlashUartMP3.h>
AlashUartMP3 mp3(Serial2);  // ESP32 second hardware UART
```

## Examples

* `HelloWorld` / `HelloWorld_ESP32` — minimal init and playback
* `FullDemo` — walkthrough of the full API
* `PlayByNumber` — play a specific track by index
* `RandomPlay` — shuffle playback
* `SpecialAnnouncement` — interrupt playback with a priority track

## Other Arduino libraries from Alash-electronics

[github.com/Alash-electronics](https://github.com/Alash-electronics)
