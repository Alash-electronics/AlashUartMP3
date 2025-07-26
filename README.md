# AlashUartMP3

Библиотека для управления MP3-модулями через UART (например, на чипе JQ8400)

## Установка

* Скачать: https://github.com/Alash-electronics/AlashUartMP3
* В Arduino IDE выберите: Скетч > Подключить библиотеку > Добавить .ZIP библиотеку
* Выберите скачанный файл AlashUartMP3.zip
* Теперь доступны примеры: Файл > Примеры > AlashUartMP3 > HelloWorld

## Использование

Библиотека AlashUartMP3 может работать с любым объектом "Serial", включая "SoftwareSerial" — всё, что реализует интерфейс Stream.

### Пример подключения:

```cpp
#include <AlashUartMP3.h>
AlashUartMP3 mp3(mySerial);
```

### Пример для ESP32:

```cpp
#include <AlashUartMP3.h>
AlashUartMP3 mp3(Serial2);
```

### Примеры инициализации:

```cpp
#include <AlashUartMP3.h>
AlashUartMP3 mp3(mySerial);

#include <AlashUartMP3.h>
AlashUartMP3 mp3(Serial);

#include <AlashUartMP3.h>
AlashUartMP3 mp3(Serial2);
```

