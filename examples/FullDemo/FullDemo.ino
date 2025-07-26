/**
 * Общая демонстрация управления MP3-модулем через UART. После загрузки откройте Serial Monitor и вводите команды.
 * 
 * Позволяет протестировать большинство функций управления MP3-модулем (например, на чипе JQ8400).
 *
 * @author Alash Engineer, 2020, alash.electronics@gmail.com
 * @license MIT License
 * @file
 */

// Пример подключения с использованием SoftwareSerial
//
//   Arduino Pin 8 подключается к TX модуля
//   Arduino Pin 9 подключается к одному концу резистора 1кОм, 
//     другой конец резистора подключается к RX модуля
//   Если ваш Arduino питается от 3.3В, можно убрать резистор 1кОм
//
// Если у вас есть дополнительный аппаратный последовательный порт, эти две строки не нужны

#include <SoftwareSerial.h>
SoftwareSerial mySerial(8,9);

// Создаём объект mp3, передавая ему последовательный порт
// Например, можно использовать mp3(Serial2) вместо SoftwareSerial
#include <AlashUartMP3.h>
AlashUartMP3 mp3(mySerial);

void setup() 
{  
  // Должны общаться на скорости 9600 с модулем, настраиваем последовательный порт
  mySerial.begin(9600);

  // Теперь можем сбросить модуль
  mp3.reset();

  // Выводим меню и статус на обычный последовательный порт
  Serial.begin(9600);
  statusAndHelpOutput();
}

void loop() {
  
    byte b;
    if(Serial.available())
    {
      b = Serial.read();

      switch(b)
      {
        case 'p': Serial.println("Воспроизведение");   mp3.play();     return;
        case 'r': Serial.println("Перезапуск"); mp3.restart(); return;
        case ' ': Serial.println("Пауза"); mp3.pause();     return;
        case '>': Serial.println("Следующий");  mp3.next();      return;
        case '<': Serial.println("Предыдущий");  mp3.prev();      return;
        
        case ']': Serial.println("Следующая папка");  mp3.nextFolder(); return;
        case '[': Serial.println("Предыдущая папка");  mp3.prevFolder(); return;
    
        case '+': Serial.println("Громкость +"); mp3.volumeUp(); return;
        case '-': Serial.println("Громкость -"); mp3.volumeDn(); return;
        case 'm': Serial.println("Громкость 0"); mp3.setVolume(0); return;
        
        case 'v': 
        {
          char volBuff[10]; 
          memset(volBuff, 0, sizeof(volBuff));          
          Serial.readBytesUntil('\n',volBuff, sizeof(volBuff)-1);
          mp3.setVolume(max(0,min(100, atoi(volBuff))));
          
          Serial.print("Громкость ");
          Serial.println(max(0,min(100, atoi(volBuff))));                    
        }
        return;

        case 'e':
        {
          do
          {
            while(!Serial.available()); // Ждём
            b = Serial.read();
            if(b != ' ') break; // Разрешаем "e N" или "eN" и т.д.
          } while(1);
          
          Serial.print("Эквалайзер ");
          switch(b)
          {
            case 'N': Serial.println("Обычный");  mp3.setEqualizer(MP3_EQ_NORMAL);  break;
            case 'P': Serial.println("Поп");     mp3.setEqualizer(MP3_EQ_POP);     break;
            case 'R': Serial.println("Рок");    mp3.setEqualizer(MP3_EQ_ROCK);    break;
            case 'J': Serial.println("Джаз");    mp3.setEqualizer(MP3_EQ_JAZZ);    break;
            case 'C': Serial.println("Классика"); mp3.setEqualizer(MP3_EQ_CLASSIC); break;
          }
        }
        return;

        case 'l':
        {
          do
          {
            while(!Serial.available()); // Ждём
            b = Serial.read();
            if(b != ' ') break; // Разрешаем "l N" или "lN" и т.д.
          } while(1);
          
          Serial.print("Цикл ");
          switch(b)
          {
            case 'A': Serial.println(F("Все")); mp3.setLoopMode(MP3_LOOP_ALL);      break;  // Воспроизводит треки один за другим и повторяет
            case 'R': Serial.println(F("Случайно")); mp3.setLoopMode(MP3_LOOP_ALL_RANDOM);       break;
            
            case 'F': Serial.println(F("Папка")); mp3.setLoopMode(MP3_LOOP_FOLDER);       break; // Цикл в пределах папки
            case 'r': Serial.println(F("Случайно в папке")); mp3.setLoopMode(MP3_LOOP_FOLDER_RANDOM);       break;
            
            case 'O': Serial.println(F("Один (повтор одного файла)")); mp3.setLoopMode(MP3_LOOP_ONE);       break;
            case 'N': 
            case 'S': Serial.println(F("Нет (воспроизвести файл и остановиться)")); mp3.setLoopMode(MP3_LOOP_ONE_STOP); break;  // По умолчанию, воспроизводит трек и останавливается
          }
        }
        return;

        case 's':
        {
          do
          {
            while(!Serial.available()); // Ждём
            b = Serial.read();
            if(b != ' ') break; // Разрешаем "s N" или "sN" и т.д.
          } while(1);
          
          Serial.print("Источник ");
          switch(b)
          {
            case 'S': Serial.println("SD-карта (если доступна)."); mp3.setSource(MP3_SRC_SDCARD);      break;
            case 'B': Serial.println("встроенная память.");mp3.setSource(MP3_SRC_BUILTIN);   break;            
          }
        }
        return;

        case 'f':
        {
          char fnumBuff[10]; 
          memset(fnumBuff, 0, sizeof(fnumBuff));          
          Serial.readBytesUntil('\n',fnumBuff, sizeof(fnumBuff)-1);
          unsigned int fnum = strtoul(fnumBuff, NULL, 10);

          Serial.println();
          Serial.print("Воспроизвести файл №");
          Serial.print(fnum);
          Serial.println(F(" (если существует)."));
          mp3.playFileByIndexNumber(fnum);   
          return;
          
        }
        return;
        
        case 'i':
        {
          char fnumBuff[10]; 
          memset(fnumBuff, 0, sizeof(fnumBuff));          
          Serial.readBytesUntil('\n',fnumBuff, sizeof(fnumBuff)-1);
          unsigned int fnum = strtoul(fnumBuff, NULL, 10);

          Serial.println();
          Serial.print("Вставить файл №");
          Serial.print(fnum);
          Serial.println(F(" (если существует)."));
          mp3.interjectFileByIndexNumber(fnum);   
          return;
          
        }
        return;
        
        case 'F':
        {
          char fnumBuff[10]; 
          memset(fnumBuff, 0, sizeof(fnumBuff));          
          Serial.readBytesUntil('/',fnumBuff, sizeof(fnumBuff)-1);
          unsigned int folnum = strtoul(fnumBuff, NULL, 10);
          
          memset(fnumBuff, 0, sizeof(fnumBuff));          
          Serial.readBytesUntil('\n',fnumBuff, sizeof(fnumBuff)-1);
          unsigned int fnum = strtoul(fnumBuff, NULL, 10);
          
          fnum   = max(1,min(fnum, 999));
          folnum = max(1,min(folnum, 99));
          
          Serial.print("Воспроизвести "); 
            if(folnum < 10) Serial.print('0');
          Serial.print(folnum);
          Serial.print('/');
            if(fnum < 10) Serial.print("00");
            else if(fnum < 100) Serial.print('0');
          Serial.print(fnum);
          Serial.println(".mp3 (если существует).");
          mp3.playFileNumberInFolderNumber(folnum, fnum);   
        }
        return;

         case 'D':
         {
            char fnumBuff[10]; 
            memset(fnumBuff, 0, sizeof(fnumBuff));          
            Serial.readBytesUntil('/',fnumBuff, sizeof(fnumBuff)-1);
            
            unsigned int folnum = strtoul(fnumBuff, NULL, 10);
            folnum = max(1,min(folnum, 99));
            
            Serial.print("Воспроизвести папку "); 
              if(folnum < 10) Serial.print('0');
            Serial.print(folnum);
            Serial.println("/*.mp3 (если существует).");
            mp3.playInFolderNumber(folnum);   
         }
         return;
                  
        case '?': statusAndHelpOutput(); return;
        
        case 'S': Serial.println("Сон"); mp3.sleep(); return;
        case 'z': Serial.println("Сброс"); mp3.reset(); return;
        
      }
      
    }
    
    static unsigned long m = millis();
    
    if(millis() > 1000 && m < (millis() - 1000))
    {
      if((mp3.getStatus() == MP3_STATUS_PLAYING))
      {
        Serial.print(F("Воспроизведение, текущая позиция: "));
        Serial.print(mp3.currentFilePositionInSeconds());
        Serial.print(F("с / "));
        Serial.print(mp3.currentFileLengthInSeconds());       
        Serial.println('с');
      }
      m = millis();
    }  
}

void statusAndHelpOutput()
{
   Serial.println();
   Serial.println(F("Демонстрация MP3-плеера"));
   Serial.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
   
   Serial.print(F("Статус           : "));
   switch(mp3.getStatus())
   {
     case MP3_STATUS_STOPPED: Serial.println(F("Остановлен")); break;
     case MP3_STATUS_PLAYING: Serial.println(F("Воспроизводит")); break;
     case MP3_STATUS_PAUSED:  Serial.println(F("На паузе"));  break;
   }
   
   Serial.print(F("Громкость (0-100)    : "));
   Serial.println(mp3.getVolume());
  
   Serial.print(F("Эквалайзер        : "));
   switch(mp3.getEqualizer())
   {
     case MP3_EQ_NORMAL:     Serial.println(F("Обычный"));  break;
     case MP3_EQ_POP:        Serial.println(F("Поп"));     break;
     case MP3_EQ_ROCK:       Serial.println(F("Рок"));    break;
     case MP3_EQ_JAZZ:       Serial.println(F("Джаз"));    break;
     case MP3_EQ_CLASSIC:    Serial.println(F("Классика")); break; 
   }
   
   Serial.print(F("Режим цикла        : "));
   switch(mp3.getLoopMode())
   {
     case MP3_LOOP_ALL:      Serial.println(F("Воспроизвести все треки, затем повторить."));  break;
     case MP3_LOOP_ALL_STOP:      Serial.println(F("Воспроизвести все треки, затем остановиться."));  break;
     case MP3_LOOP_ALL_RANDOM: Serial.println(F("Воспроизвести все треки случайно."));    break;          
          
     case MP3_LOOP_ONE:      Serial.println(F("Воспроизвести один трек, затем повторить (цикл трека)."));    break;     
     case MP3_LOOP_ONE_STOP: Serial.println(F("Воспроизвести один трек, затем остановиться (без цикла)."));    break;          
     
     case MP3_LOOP_FOLDER:   Serial.println(F("Воспроизвести все треки в папке, затем повторить."));     break;     
     case MP3_LOOP_FOLDER_STOP:   Serial.println(F("Воспроизвести все треки в папке, затем остановиться."));     break;
     case MP3_LOOP_FOLDER_RANDOM:   Serial.println(F("Воспроизвести все треки в папке случайно."));     break;     
     
   }
   Serial.println();

   if(!mp3.busy())
   {
     uint8_t currentSource = mp3.getSource();
   
     mp3.setSource(MP3_SRC_BUILTIN);   
     Serial.print(F("Количество файлов во встроенной памяти    : "));
     Serial.println(mp3.countFiles());
     
     if(mp3.sourceAvailable(MP3_SRC_SDCARD))
     {
       mp3.setSource(MP3_SRC_SDCARD);
       Serial.print(F("Количество файлов на SD-карте    : "));
       Serial.println(mp3.countFiles());
     }
     else
     {
       Serial.println(F("SD-карта недоступна"));
     }
     
     if(currentSource != mp3.getSource())
     {
       mp3.setSource(currentSource);
     }
   }
   Serial.print(F("\"Текущий\" индекс файла: "));
   Serial.println(mp3.currentFileIndexNumber());
   Serial.println();
   
   Serial.print(F("\"Текущее\" имя файла : "));   
   char buff[120];
   mp3.currentFileName(buff, sizeof(buff));
   Serial.println(buff);
      
   Serial.println();
   Serial.println(F("Управление (введите в serial monitor и нажмите send): "));
   Serial.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
   Serial.println(F("? Показать это меню.\n"));
   
   Serial.println(F("p Воспроизвести\t\t> Следующий\t\t< Предыдущий\n[пробел] Пауза\tr Перезапуск с начала файла\n] Следующая папка\t[ Предыдущая папка\n"));
   
   Serial.println(F("f[1-65534]      Воспроизвести файл по (FAT) индексу\nF[01-99]/[001-999].mp3 Воспроизвести [001-999].mp3 в папке [01-99]\n"));
   Serial.println(F("i[1-65534]      Вставить файл по (FAT) индексу\n"));
   
   Serial.println(F("+ Громкость +\t- Громкость -\tm Без звука\nv[0-100] Установить громкость\n\ne[N/P/R/J/C/B] Эквалайзер (N)обычный, (P)оп, (R)ок, (J)азз, (C)лассика\nl[A/F/O/R/N]   Цикл (A)все, (R)андом, (F)папка, (r)андом в папке, (O)дин,(N)о цикла\ns[S/B]         Переключиться на (S)D-карту/(B)встроенную память\n\n"));
}

