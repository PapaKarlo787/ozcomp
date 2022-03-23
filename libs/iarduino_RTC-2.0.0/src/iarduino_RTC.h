//	Библиотека для работы с часами реального времени: (на чипе DS1302) https://iarduino.ru/shop/Expansion-payments/rtc-modul-ds1302.html
//                                                    (на чипе DS1307) https://iarduino.ru/shop/Expansion-payments/kroshechnye-rtc-modul-realnogo-vremeni.html
//                                                                     https://iarduino.ru/shop/Expansion-payments/chasy-realnogo-vremeni-rtc-trema-modul.html
//                                                    (на чипе DS3231) https://iarduino.ru/shop/Expansion-payments/chasy-realnogo-vremeni-ds3231.html
//                                                                     https://iarduino.ru/shop/Expansion-payments/chasy-realnogo-vremeni-rtc-trema-modul-v2-0.html
//                                                    (на чипе RX8025)
//  Версия: 2.0.0
//  Последнюю версию библиотеки Вы можете скачать по ссылке: https://iarduino.ru/file/235.html
//  Подробное описание функции бибилиотеки доступно по ссылке: https://wiki.iarduino.ru/page/chasy-realnogo-vremeni-rtc-trema-modul/
//  Библиотека является собственностью интернет магазина iarduino.ru и может свободно использоваться и распространяться!
//  При публикации устройств или скетчей с использованием данной библиотеки, как целиком, так и её частей,
//  в том числе и в некоммерческих целях, просим Вас опубликовать ссылку: https://iarduino.ru
//  Автор библиотеки: Панькин Павел
//  Если у Вас возникли технические вопросы, напишите нам: shop@iarduino.ru

#ifndef iarduino_RTC_h																						//
#define iarduino_RTC_h																						//
																											//
#define	RTC_UNDEFINED 0																						//	Модуль часов реального времени не определён
																											//
#if defined(ARDUINO) && (ARDUINO >= 100)																	//
#include <Arduino.h>																						//
#else																										//
#include <WProgram.h>																						//
#endif																										//
																											//
#include "memorysaver.h"																					//	Подключаем файл «хранитель памяти»									(внутри файла есть комментарий поясняющий как сэкономить мапять)
																											//
class iarduino_RTC_BASE{																					//	Объявляем полиморфный класс
	public:																									//
		virtual void	begin				(void);															//	Объявляем функцию инициализации модуля								(без параметров)
		virtual uint8_t	funcReadTimeIndex	(uint8_t);														//	Объявляем функцию чтения 1 значения из регистров даты и времени		(0-секунды / 1-минуты / 2-часы / 3-день / 4-месяц / 5-год / 6-день недели)
		virtual void	funcWriteTimeIndex	(uint8_t, uint8_t);												//	Объявляем функцию записи 1 значения в  регистры  даты и времени		(0-секунды / 1-минуты / 2-часы / 3-день / 4-месяц / 5-год / 6-день недели, значение)
};																											//
																											//
#include "iarduino_RTC_DS1302.h"																			//	Подключаем файл iarduino_RTC_DS1302.h
#include "iarduino_RTC_I2C.h"																				//
#define valCentury 21

class iarduino_RTC{																							//
	public:																									//
	/**	Конструктор класса **/																				//
		iarduino_RTC(uint8_t i){																			//	Конструктор основного класса										(название [, вывод SS/RST [, вывод SCK/CLK [, вывод MOSI/DAT]]])
			switch(i){																						//	Тип выбранного модуля
				#ifdef RTC_ENABLE_DS1302																	//
				case RTC_DS1302: objClass = new iarduino_RTC_DS1302; break;									//	Если используется модуль на базе чипа DS1302, то присваеиваем указателю objClass ссылку на новый объект производного класса iarduino_RTC_DS1302 переопределяя на него все виртуальные функции полиморфного класса iarduino_RTC_BASE
				#endif																						//
				#ifdef RTC_ENABLE_DS1307																	//
				case RTC_DS1307: objClass = new iarduino_RTC_DS1307; break;									//	Если используется модуль на базе чипа DS1307, то присваеиваем указателю objClass ссылку на новый объект производного класса iarduino_RTC_DS1307 переопределяя на него все виртуальные функции полиморфного класса iarduino_RTC_BASE
				#endif																						//
				#ifdef RTC_ENABLE_DS3231																	//
				case RTC_DS3231: objClass = new iarduino_RTC_DS3231; break;									//	Если используется модуль на базе чипа DS3231, то присваеиваем указателю objClass ссылку на новый объект производного класса iarduino_RTC_DS3231 переопределяя на него все виртуальные функции полиморфного класса iarduino_RTC_BASE
				#endif																						//
				#ifdef RTC_ENABLE_RX8025																	//
				case RTC_RX8025: objClass = new iarduino_RTC_RX8025; break;									//	Если используется модуль на базе чипа RX8025, то присваеиваем указателю objClass ссылку на новый объект производного класса iarduino_RTC_RX8025 переопределяя на него все виртуальные функции полиморфного класса iarduino_RTC_BASE
				#endif																						//
			}																								//
		}																									//
	/**	Пользовательские функции **/																		//
		void	begin		(void)					{objClass -> begin(); funcReadTime();}					//	Определяем функцию инициализации модуля								(без параметров)
		void	period		(uint8_t i)				{valPeriod=i; valPeriod*=60000;}						//	Определяем функцию задания минимального периода обращения к модулю	(i = период в минутах)
	   uint32_t	gettimeUnix	(void)					{funcCalculationTime(); return Unix;}					//	Определяем функцию получения cекунд прошедших с начала эпохи Unix	(без параметров)
		void	settimeUnix	(uint32_t, bool needWrite=true);																		//	Объявляем  функцию установки cекунд прошедших с начала эпохи Unix	(сек)
																											//
	/**	Переменные доступные для пользователя **/															//
	   uint32_t	Unix					=	0;																//	Секунды			прошедшие с начала эпохи Unix						(01.01.1970 00:00:00 GMT)
																											//
	/**	Внутренние переменные **/																			//
		iarduino_RTC_BASE*	objClass;																		//	Объявляем указатель на объект полиморфного класса					(функции данного класса будут переопределены, т.к. указателю будет присвоена ссылка на производный класс)
		uint8_t	arrCalculationTime[7];																		//	Объявляем массив для рассчёта времени без обращения к модулю		(для хранения последних, прочитанных из модуля, значений даты и времени)
	   uint16_t	valPeriod				=	0;																//	Определяем минимальный период опроса модуля							(в минутах, от 00 до 255)
	   uint32_t	valRequest				=	0;																//	Определяем время последнего чтения регистров времени
	private:																								//
	/**	Внутренние функции **/																				//
		void	funcReadTime			(void);																//	Объявляем функцию чтения даты и времени из регистров модуля			(без параметров)
		void	funcWriteTime			(void);																	//	Объявляем функцию записи даты и времени в  регистры  модуля			(без параметров)
		uint8_t	funcConvertCodeToNum	(uint8_t i)	{return (i >> 4)*10 + (i & 0x0F);}						//	Определяем функцию преобразования двоично-десятичного кода в число	(код)
		uint8_t	funcConvertNumToCode	(uint8_t i)	{return ((i/10) << 4) + (i%10);}						//	Определяем функцию преобразования числа в двоично-десятичный код	(число)
		void	funcCalculationTime		(void);																//	Объявляем функцию расчёта времени без обращения к модулю			(без параметров)
	   uint32_t	funcCalculationUnix		(void);																//	Объявляем функцию расчёта cекунд прошедших с начала эпохи Unix		(без параметров)
};																											//
																											//
#endif																										//
