Телефонный справочник (Server)
Описание

Проект представляет собой сервер телефонного справочника, который принимает команды через сетевые соединения и выполняет операции с контактами.
Сборка проекта
Необходимые инструменты

    Компилятор: g++ (GCC) 15.2.1 или выше

    Система сборки: CMake версии 4.2.1 или выше

    Генератор: Ninja (обязательно)

Шаги сборки
bash

mkdir build
cd build
cmake -G Ninja ..
ninja

После успешной сборки в директории build появится исполняемый файл server.
Запуск сервера

Серверу требуется указать порт в качестве аргумента командной строки:
bash

./server <port>

Пример:
bash

./server 9009

Поддерживаемые команды
1. Добавление контакта
bash

INSERT <имя> <телефон> <тип>

Пример:
bash

echo "INSERT INAV +37529111111 work" | nc localhost 9009

Ответ: OK при успешном добавлении
2. Поиск по имени
bash

FIND_NAME <имя>

Пример:
bash

echo "FIND_NAME INAV" | nc localhost 9009

Ответ при успехе:
text

INAV -> +37529111111
OK

3. Поиск по номеру телефона
bash

FIND_PHONE <телефон>

Пример:
bash

echo "FIND_PHONE +37529111111" | nc localhost 9009

4. Показать все контакты
bash

SHOW_ALL

Пример:
bash

echo "SHOW_ALL" | nc localhost 9009

Пример ответа:
text

INAV -> +37529111111
PETROV -> +37529111111
SEMEN -> +37529211111
OK

Форматы ответов

    Успешное выполнение: OK

    Ошибка: ERR с дополнительным сообщением (например, ERR NOT FIND <имя>)

Примеры работы
Добавление контактов:
bash

echo "INSERT INAV +37529111111 work" | nc localhost 9009
echo "INSERT PETROV +37529111111 work" | nc localhost 9009
echo "INSERT SEMEN +37529211111 work" | nc localhost 9009

Поиск существующего контакта:
bash

echo "FIND_NAME INAV" | nc localhost 9009

Вывод:
text

INAV -> +37529111111
OK

Поиск несуществующего контакта:
bash

echo "FIND_NAME INAVV" | nc localhost 9009

Вывод:
text

ERR NOT FIND INAVV

Просмотр всех контактов:
bash

echo "SHOW_ALL" | nc localhost 9009

Вывод:
text

INAV -> +37529111111
PETROV -> +37529111111
SEMEN -> +37529211111
OK

Примечания

    Для тестирования сервера используется утилита netcat (nc)

    Сервер обрабатывает команды в реальном времени

    Все команды чувствительны к регистру
