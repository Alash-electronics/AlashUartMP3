/**
 * Arduino библиотека для управления MP3-модулями через UART
 *
 * Copyright (C) 2020 Alash Engineer <alash.electronics@gmail.com>
 *
 * Данная библиотека предоставляется бесплатно для использования, копирования, модификации и распространения без ограничений.
 *
 * Библиотека предоставляется "КАК ЕСТЬ", без каких-либо гарантий.
 *
 * @author Alash Engineer, alash.electronics@gmail.com
 * @license MIT License
 * @file
 */

#include <Arduino.h>
#include "AlashUartMP3.h"

void  AlashUartMP3::play()
{
  this->sendCommand(MP3_CMD_PLAY);
}

void  AlashUartMP3::restart()
{
  this->sendCommand(MP3_CMD_STOP); // Убеждаемся, что действительно перезапустится
  this->sendCommand(MP3_CMD_PLAY);
}

void  AlashUartMP3::pause()
{
  this->sendCommand(MP3_CMD_PAUSE);
}

void  AlashUartMP3::stop()
{
  this->sendCommand(MP3_CMD_STOP);
}

void  AlashUartMP3::next()
{
  this->sendCommand(MP3_CMD_NEXT);
}

void  AlashUartMP3::prev()
{
  this->sendCommand(MP3_CMD_PREV);
}

void  AlashUartMP3::playFileByIndexNumber(uint16_t fileNumber)
{  
  // this->sendCommand(MP3_CMD_PLAY_IDX, (fileNumber>>8) & 0xFF, fileNumber & (byte)0xFF);
  this->sendCommand(MP3_CMD_PLAY_IDX, fileNumber);
}

void  AlashUartMP3::interjectFileByIndexNumber(uint16_t fileNumber)
{  
  uint8_t buf[3] = { getSource(), (uint8_t)((fileNumber>>8)&0xFF), (uint8_t)(fileNumber & (byte)0xFF) };
  this->sendCommandData(MP3_CMD_INSERT_IDX, buf, 3, 0, 0);
}

void  AlashUartMP3::seekFileByIndexNumber(uint16_t fileNumber)
{  
  // this->sendCommand(MP3_CMD_SEEK_IDX, (fileNumber>>8) & 0xFF, fileNumber & (byte)0xFF);
  this->sendCommand(MP3_CMD_SEEK_IDX, fileNumber);
}

void AlashUartMP3::abLoopPlay(uint16_t secondsStart, uint16_t secondsEnd)
{
  uint8_t buf[4] = { (uint8_t)(secondsStart / 60), (uint8_t)(secondsStart % 60), (uint8_t)(secondsEnd / 60), (uint8_t)(secondsEnd % 60) };
  this->sendCommandData(MP3_CMD_AB_PLAY, buf, sizeof(buf), 0, 0);
}

void AlashUartMP3::abLoopClear()
{
  this->sendCommand(MP3_CMD_AB_PLAY_STOP);
}

void AlashUartMP3::fastForward(uint16_t seconds)
{
  //this->sendCommand(MP3_CMD_FFWD, (seconds>>8)&0xFF, seconds&0xFF);
  this->sendCommand(MP3_CMD_FFWD, seconds);
}

void AlashUartMP3::rewind(uint16_t seconds)
{
  //this->sendCommand(MP3_CMD_RWND, (seconds>>8)&0xFF, seconds&0xFF);
  this->sendCommand(MP3_CMD_RWND, seconds);
}

void  AlashUartMP3::nextFolder()
{
  this->sendCommand(MP3_CMD_NEXT_FOLDER);
}

void  AlashUartMP3::prevFolder()
{
  this->sendCommand(MP3_CMD_PREV_FOLDER);
}

void  AlashUartMP3::playFileNumberInFolderNumber(uint16_t folderNumber, uint16_t fileNumber)
{
  // Это довольно странно, символ подстановки *ОБЯЗАТЕЛЕН*, без него файл НЕ БУДЕТ найден.
  //
  // Действительно странно. В любом случае, это формат данных:
  //  первый байт - источник (как байтовое значение, а не ASCII число)
  //  затем компоненты пути, разделенные слешем, с завершающим символом подстановки для каждого ОБЯЗАТЕЛЬНО
  //  базовое имя файла также должно иметь символ подстановки, а расширение должно быть
  //  3 символами подстановки в виде вопросительных знаков, нельзя даже сопоставить ".mp3", черт, это странно
  
  char buf[] = " /42*/032*???";
  
  buf[0] = this->getSource();
  
  uint8_t i = 2; // 1-я цифра компонента папки
  if(folderNumber<10)
  {
    buf[i] = '0';
    i++;
  }
  itoa(folderNumber,&buf[i], 10);
  
  i = 4;
  buf[i] = '*'; // itoa затерла это своим null
  
  i = 6; // 1-я цифра компонента имени файла
  if(fileNumber < 100)
  {
    buf[i] = '0';
    i++;
  }
  
  if(fileNumber < 10)
  {
      buf[i] = '0';
      i++;
  }
  itoa(fileNumber, &buf[i], 10);
  
  buf[9] = '*'; // itoa затерла это своим null
  
  this->sendCommandData(MP3_CMD_PLAY_FILE_FOLDER, (uint8_t*)buf, sizeof(buf)-1, 0, 0);
}

void  AlashUartMP3::playInFolderNumber(uint16_t folderNumber)
{
  char buf[] = " /42*/*???";
  
  buf[0] = this->getSource();
  
  uint8_t i = 2; // 1-я цифра компонента папки
  if(folderNumber<10)
  {
    buf[i] = '0';
    i++;
  }
  itoa(folderNumber,&buf[i], 10);
  
  i = 4;
  buf[i] = '*'; // itoa затерла это своим null
  
  this->sendCommandData(MP3_CMD_PLAY_FILE_FOLDER, (uint8_t*)buf, sizeof(buf)-1, 0, 0);
}

void AlashUartMP3::playSequenceByFileNumber(uint8_t playList[], uint8_t listLength)
{
  char buf[listLength*2+1]; // itoa потребует дополнительный null
  
  uint8_t i = 0;
  for(uint8_t x = 0; x < listLength; x++)
  {
    if(playList[x]<10)
    {
      buf[i++] = '0';
    }
    itoa(playList[x], &buf[i++], 10);
  }
  
  this->sendCommandData(MP3_CMD_PLAYLIST, (uint8_t *)buf, sizeof(buf)-1, 0, 0);
}

void AlashUartMP3::playSequenceByFileName(const char * playList[], uint8_t listLength)
{
  char buf[listLength*2];
  
  uint8_t i = 0;
  for(uint8_t x = 0; x < listLength; x++)
  {
    buf[i++] = playList[x][0];
    buf[i++] = playList[x][1];
  }
  
  this->sendCommandData(MP3_CMD_PLAYLIST, (uint8_t *)buf, sizeof(buf), 0, 0);
}

void  AlashUartMP3::volumeUp()
{
  if(currentVolume < 100) currentVolume++;
  // Конвертируем 0-100 в 0-30 для модуля
  uint8_t moduleVolume = (currentVolume * 30) / 100;
  this->sendCommand(MP3_CMD_VOL_UP); // Мы не можем запросить громкость с устройства, поэтому отслеживаем её локально
}

void  AlashUartMP3::volumeDn()
{
  if(currentVolume > 0 ) currentVolume--;
  // Конвертируем 0-100 в 0-30 для модуля
  uint8_t moduleVolume = (currentVolume * 30) / 100;
  this->sendCommand(MP3_CMD_VOL_DN); // Мы не можем запросить громкость с устройства, поэтому отслеживаем её локально
}

void  AlashUartMP3::setVolume(byte volumeFrom0To100)
{
  // Ограничиваем диапазон 0-100
  if(volumeFrom0To100 > 100) volumeFrom0To100 = 100;
  currentVolume = volumeFrom0To100;
  
  // Конвертируем 0-100 в 0-30 для модуля
  uint8_t moduleVolume = (volumeFrom0To100 * 30) / 100;
  this->sendCommand(MP3_CMD_VOL_SET, moduleVolume);
}

void  AlashUartMP3::setEqualizer(byte equalizerMode)
{
  currentEq = equalizerMode;
  this->sendCommand(MP3_CMD_EQ_SET, equalizerMode);
}

void  AlashUartMP3::setLoopMode(byte loopMode)
{
  currentLoop = loopMode;
  this->sendCommand(MP3_CMD_LOOP_SET, loopMode);
}


uint8_t AlashUartMP3::getAvailableSources() 
{
  return this->sendCommandWithByteResponse(MP3_CMD_GET_SOURCES);
}

void  AlashUartMP3::setSource(byte source)
{
  this->sendCommand(MP3_CMD_SOURCE_SET, source);
}

uint8_t AlashUartMP3::getSource() 
{
  return this->sendCommandWithByteResponse(MP3_CMD_GET_SOURCE);
}


void  AlashUartMP3::sleep()
{
  // В документации есть два команды остановки, но нет команды сброса
  //
  // Я считаю, что устройство автоматически засыпает при остановке.
  //
  //  Я решил сделать то, что выглядит больше как "универсальная остановка" 0x10
  //  остановкой, и определил для удобства другую команду остановки
  //  как "СБРОС", мы отправим обе, чтобы быть уверенными
    
  this->sendCommand(MP3_CMD_SLEEP);
  this->sendCommand(MP3_CMD_STOP);
}

void  AlashUartMP3::reset()
{
  uint8_t retry = 5; // Максимальное количество попыток сброса, на случай если устройство зависло
  do
  {
    // В даташите определены две команды остановки, но нет команды сброса
    //  Я решил сделать то, что выглядит больше как "универсальная остановка" 0x10
    //  остановкой, и определил для удобства другую команду остановки
    //  как "СБРОС", мы отправим обе, чтобы быть уверенными, а затем
    //  вернем вещи к "значениям по умолчанию", в отсутствие фактического сброса
    
    this->sendCommand(MP3_CMD_STOP);  delay(1); // Похоже, здесь что-то связано с таймингом
    this->sendCommand(MP3_CMD_RESET); delay(1); //  связанное с таймингом
    
    
    // Сброс к значениям по умолчанию при запуске
    this->setVolume(67); // 67 в диапазоне 0-100 соответствует примерно 20 в старом диапазоне 0-30
    this->setEqualizer(0);
    this->setLoopMode(2);
    this->seekFileByIndexNumber(1);
    this->sendCommand(MP3_CMD_STOP);
    
    uint8_t timeout = 9;
    while(timeout-- > 0 )
    {
      if(getAvailableSources())
      {
        retry = 0;
        break; 
      }
      delay(1);
    }
  }
  while(retry-- > 0);
}


    byte  AlashUartMP3::getStatus()    
    {
      if(MP3_STATUS_CHECKS_IN_AGREEMENT <= 1)
      {
        return this->sendCommandWithByteResponse(MP3_CMD_STATUS); 
      }
      
      byte statTotal = 0;
      byte stat       = 0;
      do
      {
        statTotal = 0;
        for(byte x = 0; x < MP3_STATUS_CHECKS_IN_AGREEMENT; x++)
        {
          stat = this->sendCommandWithByteResponse(MP3_CMD_STATUS);      
          if(stat == 0) return 0; // ОСТАНОВКА довольно надежна
          statTotal += stat;
        }

      } while (statTotal != 1 * MP3_STATUS_CHECKS_IN_AGREEMENT && statTotal != 2 * MP3_STATUS_CHECKS_IN_AGREEMENT);
      
      return statTotal / MP3_STATUS_CHECKS_IN_AGREEMENT;      
    }
    
    byte  AlashUartMP3::getVolume()    { return currentVolume; }
    byte  AlashUartMP3::getEqualizer() { return currentEq;     }
    byte  AlashUartMP3::getLoopMode()  { return currentLoop;   }
    
    
    uint16_t  AlashUartMP3::countFiles()   
    {
      return this->sendCommandWithUnsignedIntResponse(MP3_CMD_COUNT_FILES); 
    }
    
    uint16_t  AlashUartMP3::currentFileIndexNumber()
    {
      return this->sendCommandWithUnsignedIntResponse(MP3_CMD_CURRENT_FILE_IDX); 
    }
    
    uint16_t  AlashUartMP3::currentFilePositionInSeconds() 
    {
      uint8_t buf[3];
      
      // Это включает непрерывную отчетность о позиции, каждую секунду
      this->sendCommandData(MP3_CMD_CURRENT_FILE_POS, 0, 0, buf, 3);
      
      // Останавливаем это
      this->sendCommand(MP3_CMD_CURRENT_FILE_POS_STOP);
      
      return (buf[0]*60*60) + (buf[1]*60) + buf[2];
    }
    
    uint16_t  AlashUartMP3::currentFileLengthInSeconds()   
    {
      uint8_t buf[3];
      
      this->sendCommandData(MP3_CMD_CURRENT_FILE_LEN, 0, 0, buf, 3);
      
      return (buf[0]*60*60) + (buf[1]*60) + buf[2];
      
      return 0; /* FIXME this->sendCommandWithUnsignedIntResponse(MP3_CMD_CURRENT_FILE_LEN_SEC); */ 
    }
    
    void          AlashUartMP3::currentFileName(char *buffer, uint16_t bufferLength) 
    {
      // this->sendCommand(MP3_CMD_CURRENT_FILE_NAME, 0, 0, buffer, bufferLength);
      this->sendCommand(MP3_CMD_CURRENT_FILE_NAME, (uint8_t *)buffer, bufferLength);
      buffer[bufferLength-1] = 0; // Обеспечиваем завершение null, поскольку это строка.
    }
    
    // Вспомогательная функция для получения 16-битного ответа, как и в других функциях
    // 8-16 бит ответа в big-endian формате
    uint16_t AlashUartMP3::sendCommandWithUnsignedIntResponse(byte command)
    {      
      uint8_t buffer[4];
      this->sendCommand(command, buffer, sizeof(buffer));
      return ((uint8_t)buffer[0]<<8) | ((uint8_t)buffer[1]);
    }
    
    uint8_t AlashUartMP3::sendCommandWithByteResponse(uint8_t command)
    {
      uint8_t response = 0;
      this->sendCommand(command, &response, 1);
      return response;
    }
    
    void  AlashUartMP3::sendCommandData(uint8_t command, uint8_t *requestBuffer, uint8_t requestLength, uint8_t *responseBuffer, uint8_t bufferLength)
    {
      // Вычисляем контрольную сумму, включая все данные запроса
      uint8_t MP3_CHECKSUM = MP3_CMD_BEGIN + command + requestLength;
      
      for(uint8_t x = 0; x < requestLength; x++)
      {
        MP3_CHECKSUM += (uint8_t)requestBuffer[x];
      }
      
#if MP3_DEBUG
      Serial.println();
      
      HEX_PRINT(MP3_CMD_BEGIN);  Serial.print(" ");
      HEX_PRINT(command);        Serial.print(" ");
      HEX_PRINT(requestLength);  Serial.print(" ");
      
      for(uint8_t x = 0; x < requestLength; x++)
      {
          HEX_PRINT(requestBuffer[x]); 
          Serial.print(' ');
      }
      
      HEX_PRINT(MP3_CHECKSUM);  Serial.print(" ");
#endif
      
      // Если на линии есть случайный мусор, очищаем его сейчас.
      while(this->waitUntilAvailable(10)) this->_Serial->read();

      this->_Serial->write(MP3_CMD_BEGIN);
      this->_Serial->write(command);
      this->_Serial->write(requestLength);
      for(uint8_t x = 0; x < requestLength; x++)
      {
          this->_Serial->write(requestBuffer[x]);
      }
      this->_Serial->write(MP3_CHECKSUM);
            
      if(responseBuffer && bufferLength) 
      {
        memset(responseBuffer, 0, bufferLength);
      }
      
      // Если мы не ожидаем ответа (или не заботимся), не ждем его
      else
      {
        return;
      }
      
      // Даем время устройству обработать то, что мы сделали, и
      // ответить, до 1 секунды, но обычно только несколько мс.
      this->waitUntilAvailable(1000);

      
#if MP3_DEBUG
      Serial.print(" ==> [");
#endif
      
      // Формат ответа такой же, как формат команды
      //  AA [CMD] [DATA_COUNT] [B1..N] [SUM]
      MP3_CHECKSUM = 0;
      
      uint8_t      i = 0;
      uint8_t      j = 0;
      uint8_t      dataCount = 0;
      while(this->waitUntilAvailable(150))
      {
        j = this->_Serial->read();
                
#if MP3_DEBUG
        HEX_PRINT(j); Serial.print(" ");
#endif
        if(i == 2)
        {
          // Количество байтов данных для чтения
          dataCount = j;
        }
        
        // Мы записываем только байты данных, поэтому байты 0,1 и 2 отбрасываются
        //   за исключением вычисления контрольной суммы
        if(i <= 2) 
        {
          MP3_CHECKSUM += j;
          i++;
          continue;
        }
        else
        {
          if(dataCount > 0)
          {
            // Это байт данных для чтения
            if((i-3) <= (bufferLength-1))
            {
              responseBuffer[i-3] = j;
            }
            i++;
            dataCount--;
            MP3_CHECKSUM += j;
          }
          else
          {
            // Это байт контрольной суммы
            if((MP3_CHECKSUM & 0xFF) != j)
            {
              // Контрольная сумма не прошла
              #if MP3_DEBUG
                Serial.print(" ** КОНТРОЛЬНАЯ СУММА НЕ ПРОШЛА " );
                HEX_PRINT((MP3_CHECKSUM & 0xFF)); 
                Serial.print(" != ");
                HEX_PRINT(j); 
              #endif
              memset(responseBuffer, 0, bufferLength);
            }
            else
            {
               #if MP3_DEBUG
                Serial.print(" ** КОНТРОЛЬНАЯ СУММА ОК " );
                HEX_PRINT((MP3_CHECKSUM & 0xFF)); 
                Serial.print(" == ");
                HEX_PRINT(j); 
              #endif
            }
          }
        }
      }
      
#if MP3_DEBUG      
      Serial.print("] --> ");
      for(uint8_t x = 0; x < bufferLength; x++)
      {
        HEX_PRINT(responseBuffer[x]);
      }
      
      Serial.println();
#endif
      
    }
    

// Блокирующее ожидание с таймаутом для последовательного ввода
int AlashUartMP3::waitUntilAvailable(uint16_t maxWaitTime)
{
  uint32_t startTime;
  int c = 0;
  startTime = millis();
  do {
    c = this->_Serial->available();
    if (c) break;
  } while(millis() - startTime < maxWaitTime);
  
  return c;
}

