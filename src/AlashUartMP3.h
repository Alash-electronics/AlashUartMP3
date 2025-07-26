/**
 * Библиотека для Arduino для управления MP3-модулями через UART (например, на чипе JQ8400)
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

#ifndef JQ8400Serial_h
#define JQ8400Serial_h

#define MP3_EQ_NORMAL     0
#define MP3_EQ_POP        1
#define MP3_EQ_ROCK       2
#define MP3_EQ_JAZZ       3
#define MP3_EQ_CLASSIC    4

#define MP3_SRC_USB       0
#define MP3_SRC_SDCARD    1
#define MP3_SRC_FLASH     2
#define MP3_SRC_BUILTIN   MP3_SRC_FLASH

// Опции цикла
//   ALL = все треки по кругу, ALL_STOP все треки затем остановка, ALL_RANDOM все треки случайно
//   ONE = один трек по кругу, ONE_STOP (по умолчанию) один трек затем остановка
//   FOLDER = треки в папке по кругу, FOLDER_STOP треки в папке затем остановка, FOLDER_RANDOM случайно в папке

#define MP3_LOOP_ALL             0
#define MP3_LOOP_ALL_STOP        7
#define MP3_LOOP_ALL_RANDOM      3

#define MP3_LOOP_ONE             1
#define MP3_LOOP_ONE_STOP        2

#define MP3_LOOP_FOLDER          4
#define MP3_LOOP_FOLDER_RANDOM   5
#define MP3_LOOP_FOLDER_STOP     6

#define MP3_LOOP_NONE            2

#define MP3_STATUS_STOPPED 0
#define MP3_STATUS_PLAYING 1
#define MP3_STATUS_PAUSED  2

// Ответ от запроса статуса может быть ненадежным
//  мы можем увеличить это, чтобы проверить несколько раз.
#define MP3_STATUS_CHECKS_IN_AGREEMENT 1

#define MP3_DEBUG 0

#define HEX_PRINT(a) if(a < 16) Serial.print(0); Serial.print(a, HEX);

class AlashUartMP3
{
  protected:
     Stream *_Serial; ///< Set in the constructor, the stream (eg HardwareSerial or SoftwareSerial object) that connects us to the device.

  public:

    /** Создание объекта AlashUartMP3 с заданным объектом последовательного порта для связи с устройством.
     *
     * Пример для программной последовательной шины...
     * --------------------------------------------------------------------------------
     *
     *     #include <SoftwareSerial.h>
     *     SoftwareSerial  mySerial(8,9);
     *
     *     #include <AlashUartMP3.h>
     *     AlashUartMP3   mp3(mySerial);
     *
     *     void setup()
     *     {
     *       mySerial.begin(9600);
     *       mp3.reset();
     *     }
     *
     * Пример через последовательный порт микроконтроллера по умолчанию...
     * --------------------------------------------------------------------------------
     *
     *     #include <AlashUartMP3.h>
     *     AlashUartMP3   mp3(Serial);
     *
     *     void setup()
     *     {
     *       Serial.begin(9600);
     *       mp3.reset();
     *     }
     *
     * Пример через последовательный порт другого аппаратного порта микроконтроллера...
     * --------------------------------------------------------------------------------
     *
     *     #include <AlashUartMP3.h>
     *     AlashUartMP3   mp3(Serial2);
     *
     *     void setup()
     *     {
     *       Serial2.begin(9600);
     *       mp3.reset();
     *     }
     *
     * Пример подключения для Arduino 5v с программной последовательной шиной на пинах 8 (RX) и 9 (TX):
     * -----------------
     *  * TX на JQ8400 подключается к D8 (программная последовательная RX) на Arduino
     *  * RX на JQ8400 подключается к одному концу резистора 1кОм,
     *      другой конец резистора подключается к D9 (программная последовательная TX) на Arduino
     *
     * Пример подключения для Arduino 3v3 с программной последовательной шиной на пинах 8 (RX) и 9 (TX):
     * -----------------
     *  * TX на JQ8400 подключается к D8 (программная последовательная RX) на Arduino
     *  * RX на JQ8400 подключается к D9 (программная последовательная TX) на Arduino
     *
     * Пример подключения для ESP32 Arduino с использованием портов Serial2...
     * -----------------
     *  * TX на JQ8400 подключается к GPIO16 (Serial2 RX) на ESP32
     *  * RX на JQ8400 подключается к GPIO17 (Serial2 TX) на ESP32
     *
     * Конечно, требуются питание и земля, VCC на JQ8400 5v-совместимый (но RX не полностью, поэтому резистор выше).  *
     *
     * Напряжение TX с JQ8400 составляет 3.3v, вам не нужно уровнять это
     * как 3.3v все еще будет читаться как высокий для Arduino 5v (и безопасно для Arduino 3v3).
     *
     */

    AlashUartMP3(Stream &_Stream) { _Serial = &_Stream; };

    /** Запуск текущего трека с начала.
     *
     */

    void play();

    /** Перезапуск текущего трека с начала.
     *
     */

    void restart();

    /** Пауза текущего воспроизведения (если оно есть).
     * 
     */

    void pause();

    /** Остановка текущего воспроизведения (если оно есть).
     */

    void stop();

    /** Быстрый переход вперед на заданное количество секунд (по умолчанию 5).
     */

    void fastForward(uint16_t seconds = 5);


    /** Перемотка назад на заданное количество секунд (по умолчанию 5).
     */

    void rewind(uint16_t seconds = 5);

    /** Переход к следующему треку.
     */

    void next();

    /** Переход к предыдущему треку.
     */

    void prev();

    /** Воспроизведение следующей папки.
     */

    void nextFolder();

    /** Воспроизведение предыдущей папки.
     */

    void prevFolder();

    /** Воспроизведение конкретного файла по его номеру FAT.
     *
     *  **О номере FAT файла**
     *
     *  Обратите внимание, что номер файла не имеет ничего общего с именем файла (за исключением, если вы
     *  загрузили/скопировали их в порядке имени файла).
     *
     *  Это индекс файла в таблице выделения файлов (FAT) диска, грубо
     *  это обычно порядок, в котором вы скопировали файлы, но из-за операционных систем это не совсем гарантировано.
     *
     *  Чтобы отсортировать FAT (после копирования файлов), или просмотреть порядок FAT, найдите утилиту сортировки FAT для вашей операционной системы.
     *
     *  В частности, на Linux [fatsort](https://sourceforge.net/projects/fatsort/) - это инструмент,
     *  который вам нужен, но убедитесь, что вы используете последнюю версию, которая поддерживает FAT-12, более старые версии нет.
     *
     *  @param fileNumber FAT индекс файла для воспроизведения.
     */

    void playFileByIndexNumber(uint16_t fileNumber);

    /** Вмешательство текущего воспроизводимого файла (если он есть) с файлом, имеющим заданный номер FAT.
     *
     *  Как если бы специальное объявление проходило над, оно прерывает
     *   музыку, когда объявление заканчивается, музыка продолжается.
     *
     *  **О номере FAT файла**
     *
     *  Обратите внимание, что номер файла не имеет ничего общего с именем файла (за исключением, если вы
     *  загрузили/скопировали их в порядке имени файла).
     *
     *  Это индекс файла в таблице выделения файлов (FAT) диска, грубо
     *  это обычно порядок, в котором вы скопировали файлы, но из-за операционных систем это не совсем гарантировано.
     *
     *  Чтобы отсортировать FAT (после копирования файлов), или просмотреть порядок FAT, найдите утилиту сортировки FAT для вашей операционной системы.
     *
     *  В частности, на Linux [fatsort](https://sourceforge.net/projects/fatsort/) - это инструмент,
     *  который вам нужен, но убедитесь, что вы используете последнюю версию, которая поддерживает FAT-12, более старые версии нет.
     *
     *  @param fileNumber FAT индекс файла для вмешательства.
     */

    void interjectFileByIndexNumber(uint16_t fileNumber);


    /** Воспроизведение конкретного файла в конкретной папке по имени папки и файла.
     *
     * Для использования этой функции папки должны быть названы от 00 до 99, а файлы в этих папках
     * должны быть названы от 000.mp3 до 999.mp3
     *
     * **Пример**
     *
     * Устройство содержит файл...
     *
     *     /03/006.mp3
     *
     * тогда следующий код воспроизведет этот файл...
     *
     *     mp3.playFileNumberInFolderNumber(3, 6);
     *
     * Обратите внимание, что нулевое дополнение имен папки и файла требуется - "01/002.mp3" хорошо, "1/2.mp3" плохо.
     *
     * @param folderNumber 0 to 99
     * @param fileNumber  0 to 999
     */

    void playFileNumberInFolderNumber(uint16_t folderNumber, uint16_t fileNumber);

    /** Воспроизведение первого (?) файла в конкретной папке 00 до 99.
     *
     * Для использования этой функции папки должны быть названы от 00 до 99.
     *
     * Чтобы воспроизвести папку "/03" используйте `mp3.playInFolderNumber(3);`
     *
     * Обратите внимание, что нулевое дополнение имен папки и файла требуется - "01/002.mp3" хорошо, "1/2.mp3" плохо.
     *
     * @param folderNumber 0 to 99
     *
     */

    void playInFolderNumber(uint16_t folderNumber);


    /** Поиск файла по его номеру FAT.
     *
     *   Если вы хотите воспроизвести трек, вы должны сначала вызвать `play()`
     *
     *   Поиск может быть полезен, если вы хотите, чтобы устройство продолжало воспроизведение
     *   после поиска, вы можете вызвать `sleep()`.
     *
     *  **О номере FAT файла**
     *
     *  Обратите внимание, что номер файла не имеет ничего общего с именем файла (за исключением, если вы
     *  загрузили/скопировали их в порядке имени файла).
     *
     *  Это индекс файла в таблице выделения файлов (FAT) диска, грубо
     *  это обычно порядок, в котором вы скопировали файлы, но из-за операционных систем это не совсем гарантировано.
     *
     *  Чтобы отсортировать FAT (после копирования файлов), или просмотреть порядок FAT, найдите утилиту сортировки FAT для вашей операционной системы.
     *
     *  В частности, на Linux [fatsort](https://sourceforge.net/projects/fatsort/) - это инструмент,
     *  который вам нужен, но убедитесь, что вы используете последнюю версию, которая поддерживает FAT-12, более старые версии нет.
     *
     *  @param fileNumber FAT индекс файла для воспроизведения.
     */

    void seekFileByIndexNumber(uint16_t fileNumber);

    /** Цикл A-B для текущего воспроизводимого файла.
     *
     *  Для **трека, который уже воспроизводится (и не приостановлен)** цикл воспроизведения между двумя отметками, определенными начальной и конечной секундами.
     *
     *  Если вы хотите начать файл в цикле ab, сначала запустите его воспроизведение и немедленно вызовите abLoopPlay.
     *
     *      mp3.playFileByIndexNumber(1);
     *      mp3.abLoopPlay(25,50);
     *
     *  будет работать, аналогично, если вы хотите возобновить паузу в цикл ab
     *
     *     mp3.play();
     *     mp3.abLoopPlay(25,50);
     *
     *  если вы приостановитесь в abLoop, когда снова воспроизведете, цикл ab будет завершен.
     *
     * Здесь кажется, что у этой функции устройства есть некоторая странная точность,
     *   первый раз через ab-loop точка старта иногда отличается
     *   от последующих циклов через, возможно, второй "выход", то есть если вы указываете
     *   начало в 25 секунд, первый цикл может начаться в 24, а последующие в
     *   25 (или наоборот).  В кратком смысле, здесь может потребоваться эксперимент.
     *
     * @param secondsStart Второй для установки как начальную отметку.
     * @param secondsEnd Второй для установки как конечную отметку.
     *
     */

    void abLoopPlay(uint16_t secondsStart, uint16_t secondsEnd);

    /** Используйте это, чтобы прервать текущий запущенный цикл A-B.
     *
     * Пауза, остановка, воспроизведение также прерывают цикл ab, это сделает это
     *  без необходимости разрыва воспроизведения, просто продолжится через
     *  предыдущую установленную отметку конца без цикла.
     *
     */

    void abLoopClear();

    /** Увеличение громкости на 1 (громкость от 0 до 100).
     */

    void volumeUp();

    /** Уменьшение громкости на 1 (громкость от 0 до 100).
     */

    void volumeDn();

    /** Установка громкости на определенный уровень (от 0 до 100).
     * 
     * Внутри библиотеки значение автоматически конвертируется в диапазон 0-30 для модуля.
     *
     * @param volumeFrom0To100 Уровень громкости от 0 до 100
     */

    void setVolume(byte volumeFrom0To100);

    /** Установка эквалайзера в один из 6 предустановленных режимов.
     *
     * @param equalizerMode Один из следующих,
     *
     *  *  MP3_EQ_NORMAL
     *  *  MP3_EQ_POP
     *  *  MP3_EQ_ROCK
     *  *  MP3_EQ_JAZZ
     *  *  MP3_EQ_CLASSIC
     *
     */

    void setEqualizer(byte equalizerMode); // EQ_NORMAL to EQ_BASS

    /** Установка режима циклирования.
     *
     * @param loopMode Один из следующих,
     *
     *  *  MP3_LOOP_NONE          - Не циклировать, просто воспроизвести один файл и остановиться. (aka MP3_LOOP_ONE_STOP)
     *  *  MP3_LOOP_ONE           - Цикл одного файла.
     *
     *  *  MP3_LOOP_ALL           - Пройти через все файлы и повторить.
     *  *  MP3_LOOP_ALL_STOP      - Пройти через все файлы и остановиться.
     *  *  MP3_LOOP_ALL_RANDOM    - Случайно воспроизводить все файлы (непрерывно)
     *
     *  *  MP3_LOOP_FOLDER        - Пройти через все файлы в одной папке и повторить.
     *  *  MP3_LOOP_FOLDER_STOP   - Пройти через все файлы в одной папке и остановиться
     *  *  MP3_LOOP_FOLDER_RANDOM - Случайно воспроизводить все файлы в одной папке (непрерывно)
     *
     * Обратите внимание, что случайные варианты, возможно, не являются оптимальными, особенно с низким
     *   количество файлов, трек, который воспроизводится дважды подряд, не является необычным, и случайный характер
     *   не совсем случайный.  (NB: Эти случайные варианты реализованы
     *   внутри устройства JQ8400, а не в коде, вы могли бы сделать свой собственный рандомизатор
     *   и просто использовать воспроизведение по индексу и т.д...)
     *
     */

    void setLoopMode(byte loopMode);

    /** Установка источника для чтения mp3 данных. Обратите внимание, что в даташите это называется "диск".
     *
     *  @param source Один из следующих...
     *
     *   * MP3_SRC_BUILTIN    - Файлы читаются из встроенной флеш-памяти
     *   * MP3_SRC_SDCARD     - Файлы читаются с SD-карты
     *   * MP3_SRC_USB        - Файлы из подключенного USB-устройства? Я не видел модулей, способных это сделать, но возможно?
     */

    void setSource(byte source);

    /** Возвращает текущий выбранный источник.
     *
     *  @return Один из следующих...
     *
     *   * MP3_SRC_BUILTIN    - Файлы читаются из встроенной флеш-памяти
     *   * MP3_SRC_SDCARD     - Файлы читаются с SD-карты
     *   * MP3_SRC_USB        - Файлы из подключенного USB-устройства? Я не видел модулей, способных это сделать, но возможно?
     */

    uint8_t getSource();

    /** Возвращает логическое значение, указывающее, доступен ли заданный источник (можно выбрать с помощью `setSource()`)
     *
     * @param  source Один из следующих
     *   * MP3_SRC_BUILTIN    - Файлы читаются из встроенной флеш-памяти
     *   * MP3_SRC_SDCARD     - Файлы читаются с SD-карты
     *   * MP3_SRC_USB        - Файлы из подключенного USB-устройства? Я не видел модулей, способных это сделать, но возможно?
     *
     * @return bool
     */

    uint8_t sourceAvailable(uint8_t source)
    {
      return getAvailableSources() & 1<<source;
    }

    /** Остановка всех воспроизведений.
     *
     *  Это остановит все воспроизведения.  Когда вы снова вызовете `play()`, это будет
     *  с начала текущего трека.
     *
     *  Обратите внимание, что JQ8400 кажется автоматически засыпающим, когда он
     *  останавливает воспроизведение **за исключением**, когда вы ищете трек (или пауза).
     *
     */

    void sleep();

    /** Сброс устройства (мягкий).
     *
     *  Возможно, потребуется в реальной практике для фактического питания-цикла устройства
     *  в случае, если оно запутано или что-то еще.
     *
     *  Поэтому, если вы проектируете печатную плату/схему, включающую модули JQ8400, это может быть
     *  стоит включить такую возможность (т.е. питать устройство через
     *  MOSFET, который вы можете включить/выключить по желанию).
     *
     */

    void reset();

    /** Получение статуса с устройства.
     *
     * @return Один из MP3_STATUS_PAUSED, MP3_STATUS_PLAYING и MP3_STATUS_STOPPED
     */

    byte getStatus();

    /** Возвращает, занят ли устройство (воспроизведение) или нет.
     *
     *  Просто удобный тест, эквивалентен `getStatus() == MP3_STATUS_PLAYING`
     *
     * @return bool
     */

    uint8_t busy() { return getStatus() == MP3_STATUS_PLAYING; }

    /** Получение текущего уровня громкости.
     *
     * @return Значение от 0 до 100 (внутреннее значение конвертируется из диапазона 0-30 модуля)
     */

    byte getVolume();

    /** Получение режима эквалайзера.
     *
     * @return Один из следующих,
     *
     *  *  MP3_EQ_NORMAL
     *  *  MP3_EQ_POP
     *  *  MP3_EQ_ROCK
     *  *  MP3_EQ_JAZZ
     *  *  MP3_EQ_CLASSIC
     *  *  MP3_EQ_BASS
     */

    byte getEqualizer();

    /** Получение режима циклирования.
     *
     * @return Один из режимов циклирования следующим образом...
     *
     *  *  MP3_LOOP_NONE          - Не циклировать, просто воспроизвести один файл и остановиться. (aka MP3_LOOP_ONE_STOP)
     *  *  MP3_LOOP_ONE           - Цикл одного файла.
     *
     *  *  MP3_LOOP_ALL           - Пройти через все файлы и повторить.
     *  *  MP3_LOOP_ALL_STOP      - Пройти через все файлы и остановиться.
     *  *  MP3_LOOP_ALL_RANDOM    - Случайно воспроизводить все файлы (непрерывно)
     *
     *  *  MP3_LOOP_FOLDER        - Пройти через все файлы в одной папке и повторить.
     *  *  MP3_LOOP_FOLDER_STOP   - Пройти через все файлы в одной папке и остановиться
     *  *  MP3_LOOP_FOLDER_RANDOM - Случайно воспроизводить все файлы в одной папке (непрерывно)
     */

    byte getLoopMode();


    /** Подсчет количества файлов на текущем носителе.
     *
     * @return Количество файлов на этом носителе.
     *
     */

    uint16_t   countFiles();

    /** Для текущего воспроизводимого (или приостановленного, или файла, который будет воспроизведен
     *  если остановлен) файла, возвращает номер FAT файла.
     *
     *  Этот номер можно использовать с playFileByIndexNumber();
     *
     *  **О номере FAT файла**
     *
     *  Обратите внимание, что номер файла не имеет ничего общего с именем файла (за исключением, если вы
     *  загрузили/скопировали их в порядке имени файла).
     *
     *  Это индекс файла в таблице выделения файлов (FAT) диска, грубо
     *  это обычно порядок, в котором вы скопировали файлы, но из-за операционных систем это не совсем гарантировано.
     *
     *  Чтобы отсортировать FAT (после копирования файлов), или просмотреть порядок FAT, найдите утилиту сортировки FAT для вашей операционной системы.
     *
     *  В частности, на Linux [fatsort](https://sourceforge.net/projects/fatsort/) - это инструмент,
     *  который вам нужен, но убедитесь, что вы используете последнюю версию, которая поддерживает FAT-12, более старые версии нет.
     *
     *  @return Номер FAT файла.
     */

    uint16_t   currentFileIndexNumber();

    /** Для текущего воспроизводимого или приостановленного файла, возвращает
     *  текущую позицию в секундах.
     *
     * @return Количество секунд, прошедших с начала файла, который сейчас воспроизводится.
     *
     */

    uint16_t   currentFilePositionInSeconds();

    /** Для текущего воспроизводимого или приостановленного файла, возвращает
     *  общую длину файла в секундах.
     *
     * @return Длина аудиофайла в секундах.
     *
     */

    uint16_t   currentFileLengthInSeconds();

    /** Получение имени "текущего" файла.
     *
     * Имя, возвращаемое, значительно сокращено до формата 8+3 без точки, все заглавные... то есть, это, вероятно, не очень полезно.
     *
     * Текущий файл - это тот, который воспроизводится, приостановлен или, если остановлен,
     * может быть следующим для воспроизведения или последним воспроизведенным, неопределенно.
     *
     * **Пример**
     *
     *     char buf[12];
     *     mp3.currentFileName(buf, sizeof(buf));
     *     Serial.println(buf);
     *
     * @param buffer character buffer of 12 bytes or more (eg `char buf[12]`)
     * @param bufferLength length of the buffer (eg 12)
     *
     */

    void           currentFileName(char *buffer, uint16_t bufferLength);

    /** Воспроизведение последовательности файлов, которые должны все существовать в папке, называемой "ZH", и иметь имена 00.mp3 через 99.mp3
     *
     * Не спрашивайте меня, почему папка должна называться "ZH", это то, что хочет JQ8400.
     *
     * **Пример**
     *
     * Учитывая, что JQ8400 содержит файлы...
     *
     *     /ZH/01.mp3
     *     /ZH/02.mp3
     *     /ZH/03.mp3
     *
     * тогда следующий код воспроизведет их в порядке 03.mp3, 01.mp3, 02.mp3
     *
     *     uint8_t playList[] = { 3, 1, 2 };
     *     mp3.playSequenceByFileNumber(playList, sizeof(playList));
     *
     * обратите внимание, что имена файлов состоят из 2 цифр, "`1.mp3`" не является допустимым.
     *
     * @param playList An array of the numbers of files in the "ZH" folder.
     * @param listLength          Number of filenames in the list.
     *
     */

    void playSequenceByFileNumber(uint8_t playList[], uint8_t listLength);

    /** Воспроизведение последовательности файлов, которые должны все существовать в папке, называемой "ZH", и иметь имена из 2 символов.
     *
     *  Не спрашивайте меня, почему папка должна называться "ZH", это то, что хочет JQ8400.
     *
     * **Пример**
     *
     * Учитывая, что JQ8400 содержит файлы...
     *
     *     /ZH/A1.mp3
     *     /ZH/1B.mp3
     *     /ZH/AZ.mp3
     *
     * тогда следующий код воспроизведет их в порядке 1B.mp3, A1.mp3, AZ.mp3
     *
     *     const char * playList[] = { "1B", "A1", "AZ" };
     *     mp3.playSequenceByFileName(playList, sizeof(playList)/sizeof(char *));
     *
     * @param playList   An array of the two character names (as strings).
     * @param listLength Number of filenames in the list.
     *
     */

    void playSequenceByFileName(const char *playList[], uint8_t listLength);



  protected:

    /** Отправка команды на модуль JQ8400,
     *
     * @param command        Byte value of to send as from the datasheet.
     * @param requestBuffer  Pointer to (or NULL) request data bytes.
     * @param requestLength  Number of bytes in the request buffer.
     * @param responseBuffer Buffer to store a single line of response, if NULL, no response is read.  Note that the response is NOT a null-terminated string, if you want that, do it yourself (and specify length-1).
     * @param buffLength     Length of response buffer.
     *
     */

    void sendCommandData(uint8_t command, uint8_t *requestBuffer, uint8_t requestLength, uint8_t *responseBuffer, uint8_t bufferLength);

    /** Отправка команды с нулевыми аргументами и без ответа.
     *
     * @param command       Byte value of to send as from the datasheet.
     */

    inline void sendCommand(uint8_t command, uint8_t *responseBuffer = 0, uint8_t bufferLength = 0)
    {
      sendCommandData(command, NULL,  0, responseBuffer, bufferLength);
    }

    /** Отправка команды с одним 8-битным аргументом и без ответа.
     *
     * @param command       Byte value of to send as from the datasheet.
     * @param arg           Single byte of data
     */

    inline void sendCommand(uint8_t command, uint8_t arg, uint8_t *responseBuffer = 0, uint8_t bufferLength = 0)
    {
      sendCommandData(command, &arg, 1, responseBuffer, bufferLength);
    }

    /** Отправка команды с 16-битным целым аргументом.
     *      *
     * @param command       Byte value of to send as from the datasheet.
     * @param arg           16 bit uint16_teger data
     */

    inline void sendCommand(uint8_t command, uint16_t arg, uint8_t *responseBuffer = 0, uint8_t bufferLength = 0)
    {
      #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        sendCommandData(command, ((uint8_t *)(&arg)), 2, responseBuffer, bufferLength);
      #else
        uint8_t buf[] = { *(((uint8_t *)(&arg))+1), *((uint8_t *)(&arg)) };
        sendCommandData(command, buf, 2, responseBuffer, bufferLength);
      #endif
    }

    /** Отправка команды на модуль JQ8400, и получение 16-битного целочисленного ответа.
     *
     * @param command        Byte value of to send as from the datasheet.
     * @return Response from module.
     */

    uint16_t sendCommandWithUnsignedIntResponse(byte command);

    /** Отправка команды на модуль JQ8400, и получение 8-битного целочисленного ответа.
     *
     * @param command        Byte value of to send as from the datasheet.
     * @return Response from module.
     */

    uint8_t sendCommandWithByteResponse(uint8_t command);


    /** Возвращает битовую маску доступных источников.
     *
     * @return Битовая маска, указывающая, какие источники подключены к модулю.
     *
     *    bit 0 = USB
     *    bit 1 = SD
     *    bit 2 = FLASH
     */

    uint8_t getAvailableSources();

    /** Блокирующий ожидание с таймаутом для последовательного ввода.
     *
     * @param maxWaitTime Milliseconds
     * @return bool Available (True) / Timed Out (False)
     */

    int    waitUntilAvailable(uint16_t maxWaitTime = 1000);


    uint8_t currentVolume = 67; ///< Запись текущего уровня громкости (0-100, конвертируется в 0-30 для модуля)
    uint8_t currentEq     = 0;  ///< Запись текущего эквалайзера (JQ8400 не имеет способа запросить)
    uint8_t currentLoop   = 2;  ///< Запись текущего режима циклирования (JQ8400 не имеет способа запросить)

    /** @name Определения байтов команд
     *
     */
    ///@{
    static const uint8_t MP3_CMD_BEGIN = 0xAA;

    static const uint8_t MP3_CMD_PLAY = 0x02;
    static const uint8_t MP3_CMD_PAUSE = 0x03;

    static const uint8_t MP3_CMD_FFWD  = 0x23;
    static const uint8_t MP3_CMD_RWND  = 0x22;

    static const uint8_t MP3_CMD_STOP = 0x10; // Не уверен, возможно 0x04?

    static const uint8_t MP3_CMD_NEXT = 0x06;
    static const uint8_t MP3_CMD_PREV = 0x05;
    static const uint8_t MP3_CMD_PLAY_IDX = 0x07;
    static const uint8_t MP3_CMD_SEEK_IDX = 0x1F;
    static const uint8_t MP3_CMD_INSERT_IDX = 0x16;

    static const uint8_t MP3_CMD_AB_PLAY      = 0x20;
    static const uint8_t MP3_CMD_AB_PLAY_STOP = 0x21;

    static const uint8_t MP3_CMD_NEXT_FOLDER = 0x0F;
    static const uint8_t MP3_CMD_PREV_FOLDER = 0x0E;

    static const uint8_t MP3_CMD_PLAY_FILE_FOLDER = 0x08;

    static const uint8_t MP3_CMD_VOL_UP = 0x14;
    static const uint8_t MP3_CMD_VOL_DN = 0x15;
    static const uint8_t MP3_CMD_VOL_SET = 0x13;

    static const uint8_t MP3_CMD_EQ_SET = 0x1A;
    static const uint8_t MP3_CMD_LOOP_SET = 0x18;
    static const uint8_t MP3_CMD_SOURCE_SET = 0x0B;

    static const uint8_t MP3_CMD_SLEEP = 0x04;    // Я не уверен, см. реализацию sleep() и reset()
    static const uint8_t MP3_CMD_RESET = 0x04;    //  то, что я сделал, может работать, может нет.

    static const uint8_t MP3_CMD_STATUS = 0x01;

    static const uint8_t MP3_CMD_GET_SOURCES = 0x09;
    static const uint8_t MP3_CMD_GET_SOURCE  = 0x0A;

    static const uint8_t MP3_CMD_COUNT_FILES     = 0x0C;
    static const uint8_t MP3_CMD_COUNT_IN_FOLDER = 0x12;

    static const uint8_t MP3_CMD_CURRENT_FILE_IDX         = 0x0D;
    static const uint8_t MP3_CMD_FIRST_FILE_IN_FOLDER_IDX = 0x11;

    static const uint8_t MP3_CMD_CURRENT_FILE_LEN = 0x24;
    static const uint8_t MP3_CMD_CURRENT_FILE_POS = 0x25; // Это включает непрерывное отчет о позиции
    static const uint8_t MP3_CMD_CURRENT_FILE_POS_STOP = 0x26; // Это останавливает это
    static const uint8_t MP3_CMD_CURRENT_FILE_NAME = 0x1E;

    static const uint8_t MP3_CMD_PLAYLIST = 0x1B;
    ///@}
};

#endif

