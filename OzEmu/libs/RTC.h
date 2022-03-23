#include <ctime>
#include <cstdlib>
#include <map>
#define CENTURY 21

class iarduino_RTC {
public:
	void begin(){}

	void settimeUnix(uint32_t sec) {
		time_t now = sec; //mktime (&t);
		ctime (&now);
	}

	uint32_t gettimeUnix() {
		return (uint32_t)time(0);
	}
};
