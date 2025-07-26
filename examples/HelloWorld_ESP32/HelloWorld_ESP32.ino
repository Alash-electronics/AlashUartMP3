/** Минимальный пример для MP3-плеера на ESP32.
 *
 * Просто воспроизводит все треки по кругу.
 *
 * | MP3-модуль (например, на чипе JQ8400) | ESP32    |
 * | ------------- | -------- |
 * | RX            | GPIO17   |
 * | TX            | GPIO16   |
 * | GND (любой)   | GND      |
 * | VCC (любой)   | VCC      |
 *
 * @author Alash Engineer, 2020, alash.electronics@gmail.com
 * @license MIT License
 * @file
 */

// Создаём объект mp3, передавая ему последовательный порт 2
#include <AlashUartMP3.h>
AlashUartMP3 mp3(Serial2);

void setup() 
{  
  Serial2.begin(9600);
  mp3.reset();
  mp3.setVolume(67); // 67 в диапазоне 0-100 соответствует примерно 20 в старом диапазоне 0-30
  mp3.setLoopMode(MP3_LOOP_ALL);
  mp3.play();  
}

void loop() {
  // Здесь ничего не требуется, всё играет автоматически :-)
}
