#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <stdlib.h>

static int is_a_console(int fd) {
	char arg = 0;
	return (ioctl(fd, KDGKBTYPE, &arg) == 0
		&& ((arg == KB_101) || (arg == KB_84)));
}

int getfd() {
	for (int fd = 0; fd < 3; fd++)
		if (is_a_console(fd))
			return fd;
	exit(1);
}

int fdk;
int oldkbmode;
struct termios old;

void clean_up() {
	if (ioctl(fdk, KDSKBMODE, oldkbmode))
		exit(1);
	tcsetattr(fdk, 0, &old);
	close(fdk);
	close(fdm);
	exit(0);
}

unsigned char get_scan_code() {
	unsigned char buf[1];
	buf[0] = 0;
	int n = read(fdk, buf, sizeof(buf));
	return buf[0];
}

unsigned char get_mice_code() {
	unsigned char buf[1];
	buf[0] = 0;
	int n = read(fdm, buf, sizeof(buf));
	return buf[0];
}

int kbd_begin(){
	struct termios new;

	fdk = getfd();
	fdm = open("/dev/input/mice", O_RDONLY);
	if (ioctl(fdk, KDGKBMODE, &oldkbmode))
		exit(1);

	int flag = 1;
	ioctl(fdk, FIONBIO, &flag);
	tcgetattr(fdk, &old);
	tcgetattr(fdk, &new);

	new.c_lflag &= ~ (ICANON | ECHO | ISIG);
	new.c_iflag = 0;
	new.c_cc[VMIN] = 1;
	new.c_cc[VTIME] = 1;

	tcsetattr(fdk, TCSAFLUSH, &new);
	if (ioctl(fdk, KDSKBMODE, K_RAW))
		exit(1);
}
