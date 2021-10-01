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

int fd;
int oldkbmode;
struct termios old;
unsigned char buf[1];

static void clean_up(int x) {
	if (ioctl(fd, KDSKBMODE, oldkbmode))
		exit(1);
	tcsetattr(fd, 0, &old);
	close(fd);
	exit(0);
}

unsigned char get_scan_code() {
	int n = read(fd, buf, sizeof(buf));
	return buf[0];
}

int kbd_begin(){
	struct termios new;

	fd = getfd();
	/* the program terminates when there is no input for 10 secs */
	signal(SIGALRM, clean_up);
	if (ioctl(fd, KDGKBMODE, &oldkbmode))
		exit(1);
	tcgetattr(fd, &old);
	tcgetattr(fd, &new);

	new.c_lflag &= ~ (ICANON | ECHO | ISIG);
	new.c_iflag = 0;
	new.c_cc[VMIN] = sizeof(buf);
	new.c_cc[VTIME] = 1;

	tcsetattr(fd, TCSAFLUSH, &new);
	if (ioctl(fd, KDSKBMODE, K_RAW))
		exit(1);
}
