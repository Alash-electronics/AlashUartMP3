/** Демонстрация воспроизведения файла по его (FAT) индексу.
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
  mp3.setLoopMode(MP3_LOOP_NONE);
}

void loop() {
  
  if(!mp3.busy())
  {
    // 1 — это FAT-индекс первого файла на устройстве, 2 — следующий, 3 — третий и т.д.
    // FAT-индекс не связан с именем файла. Для сортировки FAT используйте, например, fatsort.
    // В этом примере мы просто воспроизводим первый индекс снова и снова.
    mp3.playFileByIndexNumber(1);  
  }
  
}

