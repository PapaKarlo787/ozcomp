OzComp
======

![title](Keyboard+Case/ComputerCase/pic.jpg)



### Общее описание

Мы создаем компьютер на базе Arduino с удобными инструкциями, которые позволяют просто общаться с устройствами, не используя их интерфейсы, не зная ничего о их протоколах и интерфейсах, что сокращает размер исходного кода. Так же наш проект расширяет возможности Arduino. Теперь ей доступно 4 Гб оперативной памяти, которая представлена в виде FLASH-памяти на SD карте. Так же имеется теоретическая (пока нет утилит) возможность, не прибегая к стороннему программированию (программатор, компьютер для загрузки прошивки и т.д.), писать программы, которые будут работать на нашем компьютере. Помимо прочего, наш проект устраняет скорый износ внутренней FLASH-памяти (например при частой смене программного обеспечения) и последующую негодность платы. Это происходит за счет наличия внешнего подключения SD карты.

Ниже дано краткое описание системы команд, регистров и системы в целом.
+ Операнд-источник справа, операнд-приемник слева.
+ Доступно по 16 регистров с именами R0-R15 для плавающей арифметики и для целых чисел.
+ Операции для плавающей запятой это арифметические, стековые и операции копирования (MOV), их имена предворяются знаком F (FADD, FMOV)
+ Все регистры 32-битные.
+ Некоторые из регистров используются для неявного сохранения в них результатов работы инструкций.
+ Пищалка на портах 15 и 17.


##### Память и системные регистры

+ Память реализована как SD-карта (в эмуляторе файл 'memory'). Программа стартует с нулевого байта.
+ SP это внутренний указатель На вершину стека.
+ IP это внутренний указатель на текущую инструкцию. При исполнении комады CALL его значение
кладется на вершину стека, уменьшая значение SP на 4. После выполнения инструкции RET,
значение со стека снимается увеличивая SP на 4.
+ FLAGS это внутренний регистр состояния, состоит из 4 битов Equal, Great, Less, Intersected (E, G, L, I).
Служит для основания условного перехода, изменяется арифметическими и логическими операциями.


##### Экран

Экран имеет разрешение 84*48 пикселя, это 6 рядов по 84 колонки. Каждый зажженый пиксель
в колонке n от правого верхнего угла последовательно - слева направо, сверху вниз,
означает установленный бит сверху вниз n-го байта в памяти экрана. При исполнении
инструкции SETC, устанавливается номер колонки, которая будет изменена первой следующей
инструкцией DRAW (которая отправляет 504 байта из памяти по указателю в память экрана)
Экран не позволяет узнать содержимое своей памяти, поэтому она хранится в памяти Arduino. 


##### Клавиатура

Встроенная таблица символов, считавыние клавиши происходит с помощью инструкции
GKEY, которая неявно использует регистр R15 для сохранения байта, полученного от 
клавиатуры. Клавиатура может питать компьютер, если имеет встроенный источник
питания.


##### Звуки

Звуковой источник - пьезо-пищалка, которая установлена на порты Arduino 17 и 15.
Аудио код состоит из количества пар 32-битных чисел, частоты пищания и задержки.
Реально задержки нет, поскольку сигнал генерируется отдельным таймером Arduino,
сам же контроллер проверяет между командами с карты время которое прошло с момента
запуска очередного тона и меняет его, если прошло достаточно.


##### Обозначения

+ R - регистр
+ C - константа
+ L - метка
+ E, G, L, I - флаги регистра состояния (равно, больше, меньше, пересечение - соответственно)
+ N/A - нет аргументов
+ M - ячейка памяти, варианты записи один из [L], [L+R]


##### Арифметические операции

|Название 	| Аргументы |	Описание
|-:|:-:|:-
ADD 		| R/(R,C)	| 	Сложение
SUB 		| R/(R,C)	| 	Вычитание
MUL 		| R/(R,C)	| 	Умножение
DIV 		| R/(R,C)	| 	Деление
MOD		 	| R/(R,C)	| 	Остаток от деления
POW			| R/(R,C)	| 	Возведение в степень
CMP			| R/(R,C)	| 	Изменить флаги вычитанием
TEST		| R	| 	Изменить флаги сравнением с нулем


##### Логические операции

|Название 	| Аргументы |	Описание
|-:|:-:|:-
AND 	| R/(R,C)	|	Побитовое И целых частей операндов
OR  	| R/(R,C)	|	Побитовое ИЛИ целых частей операндов
XOR 	| R/(R,C)	|	Побитовое исключающее ИЛИ целых частей операндов
SHR 	| R/(R,C)	|	Битовый сдвиг вправо
SHL 	| R/(R,C)	|	Битовый сдвиг влево

***Общий пример для этих групп команд.***

+ ADD R0, R1
+ ADD R0, 1


##### Переходы

|Название 	| Аргументы |	Описание
|-:|:-:|:-
JMP		| L			| 	Безусловный переход
J{E,G,L}| L			| 	Условный переход по E, G, L
LOOP	| L			| 	Условный переход пока R14 больше 0
CALL	| L			| 	Безусловный переход с сохранением IP в стек 
RET		| N/A		| 	Извлечение IP из стека

***Общий пример для этой группы команд.***

+ JNE my_label
+ CALL my_func
+ RET


##### Работа со стеком

|Название 	| Аргументы |	Описание
|-:|:-:|:-
PUSH		| R 		|	Положить на стек
POP		| R			|	Снять со стека

***Примеры***

+ PUSH R0
+ FPOP R1

##### Экран, клавиатура и динамик

|Название 	| Аргументы |	Описание
|-:|:-:|:-
FPRINT 	| (R,L) 	| 	Печать числа с плавающей запятой
LPRINT 	| (R,L) 	| 	Печать нуль-терминированной строки
PRINT	| (R,L) 	| 	Печать целого числа
SCUR	| R/R, C/C 	| 	Установка курсора на позицию X, Y
SEND	| (R,C)		| 	Отправка байта на экран в текущую позицию курсора
DRAW	| L			| 	Отправить 504 байта в память экрана из памяти с адреса L
GKEY	| N/A		| 	Получить символ клавиатуры, 0 если нет
SCOL  | C | Установить цвет рисуемого изображения (0 или не 0 для монохромного экрана)
PLAY  | L | Начать циклическое проигрывание музыки
NPLAY | L | Завершить проигрывание музыки

***Примеры***

+ PRINT my_str
+ IPRINT R0
+ SETC R0, R5
+ SETC 10, 20
+ SEND R0 (<256)
+ SEND 85
+ DRAW my_pic
+ GKEY


##### Макрокоманды MCASM

|Название 	| Аргументы |	Описание
|-:|:-:|:-
DD		| C1, C2, ...	|	Создать несколько 32-битных чисел
DB		| C1, C2, ...	|	Создать несколько 8-битных чисел, (строки тоже)
DF		| C1, C2, ...	|	Создать несколько 32-битных чисел с плавающей запятой
TIMES | CMD ARGS  N | Повторить команду CMD N раз

***Примеры***

+ DD -3, 100500, 0xffffffff
+ DB 10, 12, 255, "HELLO WORLD!"
+ DF 3.1415926535, 2.718281828
+ TIMES CALL INC 10


##### Другие

|Название 	| Аргументы |	Описание
|-:|:-:|:-
MOV 	| R/(R,C,M),M/R | 	Перемещение данных во float
MOVB	| R/M,M/R 		|	Перемещение байтов в ячейки памяти
INT		| R				|	Округление
DELAY	| (R,C)			|	Задержка в милисекундах
RND		| N/A			| 	Случайное целое в промежутке [0, 1024), результат в R13
ICVTF | R/R | Конвертация float в int
FCVTI | R/R | Конвертация int в float

***Примеры***

+ MOV [my_var], R0
+ MOV R0, [my_var+R1]
+ INT R0
+ DELAY R1
+ DELAY 100
+ RND
