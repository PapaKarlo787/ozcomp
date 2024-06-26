#include <fstream>
using namespace std;

fstream fs;

void sd_raw_read(unsigned int ip, unsigned char* data, unsigned int count){
	fs.seekg(ip);
	fs.read((char*)data, count);
}

void sd_raw_write(unsigned int ip, unsigned char* data, unsigned int count){
	fs.seekg(ip);
	for(unsigned int i = 0; i < count; i++)
		fs << data[i];
}

uint8_t sd_raw_copy_sector(uint32_t from, uint32_t to) {
	uint8_t d[512];
	sd_raw_read(from, d, 512);
	sd_raw_write(to, d, 512);
	return 0;
}

bool sd_raw_init(){
	fs.open("memory");
	return fs.is_open();
}


void sd_raw_sync(){
	fs.flush();
}
