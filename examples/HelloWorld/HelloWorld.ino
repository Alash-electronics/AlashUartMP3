/** Минимальный пример для MP3-плеера.
 *
 * Просто воспроизводит все треки по кругу.
 *
 * @author Alash Engineer, 2020, alash.electronics@gmail.com
 * @license MIT License
 * @file
 */

// Пример использования SoftwareSerial на пинах 8 и 9
#include <SoftwareSerial.h>
SoftwareSerial mySoftwareSerial(8,9);

// Создаём объект mp3, передавая ему последовательный порт
// Например, можно использовать mp3(Serial2) вместо SoftwareSerial
#include <AlashUartMP3.h>
AlashUartMP3 mp3(mySoftwareSerial);

void setup() 
{  
  mySoftwareSerial.begin(9600);
  mp3.reset();
  mp3.setVolume(67); // 67 в диапазоне 0-100 соответствует примерно 20 в старом диапазоне 0-30
  mp3.setLoopMode(MP3_LOOP_ALL);
  mp3.play();  
}

void loop() {
  // Здесь ничего не требуется, всё играет автоматически :-)
}


