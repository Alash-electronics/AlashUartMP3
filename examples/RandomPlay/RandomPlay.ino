/** Пример, который воспроизводит файлы на носителе в случайном порядке.
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

unsigned int numFiles; // Общее количество файлов на носителе (определяется в setup())

void setup() {
  // Запускаем отладочный терминал
  Serial.begin(9600);  
  
  // Инициализация соединения, сброс устройства и установка громкости
  mySoftwareSerial.begin(9600);
  mp3.reset();
  
  while(numFiles == 0)
  {
    // Проверяем наличие SD-карты и переключаемся на неё
    if(mp3.sourceAvailable(MP3_SRC_SDCARD))
    {
      mp3.setSource(MP3_SRC_SDCARD);    
      if(mp3.countFiles() == 0)
      {
        mp3.setSource(MP3_SRC_BUILTIN);
      }
    }
    
    numFiles = mp3.countFiles();
    
    if(!numFiles)
    {
      Serial.println(F("Ошибка! На носителе не найдено файлов, проверены SD-карта и встроенная память."));
      Serial.println(F("Повторим попытку через 3 секунды."));
      Serial.println(F("Если файлы есть, но не находятся, попробуйте выключить и включить всё заново — возможно, модуль завис."));
      Serial.println(F("Иногда это может случиться, если вставлять/вынимать SD-карту при включённом питании."));
      Serial.println(F("В реальном проекте можно предусмотреть питание модуля через MOSFET или BJT, чтобы можно было перезапустить модуль, если он зависнет!"));
      delay(3000);
    }
  }
}

void loop() 
{
  if(!mp3.busy()) 
  {
    // Выбираем случайный файл с номером от 1 до numFiles (random() возвращает до, но не включая верхнюю границу)
    // Если выбран тот же файл, что только что играл — выбираем другой
    unsigned int pick;
    do
    { 
      pick = random(1,numFiles+1);
    } while(pick == mp3.currentFileIndexNumber());
    
    Serial.print("Случайно выбран файл №");
    Serial.print(pick);
    Serial.print(" из ");
    Serial.println(numFiles);

    // Воспроизводим выбранный файл
    mp3.playFileByIndexNumber(pick);
   
    char buffer[20];
    mp3.currentFileName(buffer, sizeof(buffer));
    Serial.print("> ");
    Serial.println(buffer);
  }
}

