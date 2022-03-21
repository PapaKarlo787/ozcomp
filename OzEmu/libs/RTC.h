#include <ctime>
#include <cstdlib>
#include <map>
#define CENTURY 2000

class iarduino_RTC {
public:
	void begin(){}

	void settime(uint8_t sec, uint8_t min, uint8_t h, uint8_t day, uint8_t month, uint8_t year, uint8_t wday) {
		struct tm t = {0};
		t.tm_year = year + 100;
		t.tm_mon = month - 1;
		t.tm_mday = day;
		t.tm_wday = wday;
		t.tm_hour = h;
		t.tm_min = min;
		t.tm_sec = sec;
		time_t now = mktime (&t);
		ctime (&now)
	}

	string gettime(string templ) {
		string result = "";
		time_t now = time(0);
		tm t = *localtime(&now);
		for (uint8_t i = 0; i < templ.size(); i++) {
			string tmp = "";
			switch (templ[i]) {
				case 's':
					tmp += (char)(t.tm_sec / 10 + '0');
					tmp += (char)(t.tm_sec % 10 + '0');
					break;
				case 'i':
					tmp += (char)(t.tm_min / 10 + '0');
					tmp += (char)(t.tm_min % 10 + '0');					
					break;
				case 'h':
					uint8_t h = t.tm_hour % 12;
					tmp += (char)(h / 10 + '0');
					tmp += (char)(h % 10 + '0');
					break;
				case 'H':
					tmp += (char)(t.tm_hour / 10 + '0');
					tmp += (char)(t.tm_hour % 10 + '0');
					break;
				case 'd':
					tmp += (char)(t.tm_mday / 10 + '0');
					tmp += (char)(t.tm_mday % 10 + '0');
					break;
				case 'D':
					tmp = getNameOfDay(t.tm_mday);
					break;
				case 'm':
					uint8_t mon = t.tm_mon + 1; 
					tmp += (char)(mon / 10 + '0');
					tmp += (char)(mon % 10 + '0');
					break;
				case 'M':
					tmp = getNameOfMonth(t.tm_mon);
					break;
				case 'y':
					uint8_t y = t.tm_year % 100;
					tmp += (char)(y / 10 + '0');
					tmp += (char)(y % 10 + '0');
					break;
				case 'Y':
					uint32_t y = t.tm_year % 100 + CENTURY;
					while (y) {
						tmp = (char)(y % 10 + '0');
						y /= 10;
					}
					break;
				case '\\':
					tmp += templ[++i];
					break;
				default:
					tmp += templ[i];
			}
			result += tmp;
		}
		return result;
	}

	void blinktime(){}

	void period(){}
	
private:
	string getNameOfDay();
};
