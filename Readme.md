﻿### О проекте ###

В проекте представлено клиентское приложение к базе данных.

Реализованные функции:

- подключение к базе данных;
- отображение содержимого фиксированной таблицы;
- добавление в таблицу новой строки, заполненной значениями по-умолчанию;
- удаление выделенной строки;
- редактирование содержимого строк.

### Инструкция по сборке ###

Для сборки проекта требуется библиотека Qt не ниже 5.6 и компилятор с поддержкой C++11.

### Инструкция по запуску ###

Для работы программного обеспечения требуется сервер базы данных PostgreSQL (версии не ниже 9.4.1),
установленный на локальной или удаленной системе. На сервере должно быть свободно имя базы данных 
"inobitec". 

На сервере должна быть развернута база данных. Для разворачивания базы данных необходимо выполнить 
скрипт database/create-db.sh (database/create-db.cmd для Windows).

Предусмотрены скрипты для начального наполнения базы данных: database/fill-db.sh и database/fill-db.cmd.

База данных в проекте вынесена отдельно, так как это позволяет ее рассматривать как самостоятельную 
единицу со своими внутренними алгоритмами, механизмами развертывания и конфигурирования. 
Возможно вынесение её развертывания (совместно с установкой postgresql) как в отдельный дистрибутив 
так и в виде компонента общего дистрибутива.