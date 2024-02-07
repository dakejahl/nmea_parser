#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include "NMEAParser.hpp"

int main() {
	const char* portname = "/dev/ttyUSB0";
	NMEAParser parser;

	// Open the serial port
	int fd = open(portname, O_RDWR | O_NOCTTY);
	if (fd < 0) {
		PX4_INFO("Error opening %s: %s", portname, strerror(errno));
		return -1;
	}

	// Configure the serial port
	struct termios tty;
	memset(&tty, 0, sizeof tty);
	if (tcgetattr(fd, &tty) != 0) {
		PX4_INFO("Error from tcgetattr %s", strerror(errno));
		return -1;
	}

	cfsetospeed(&tty, B115200);
	cfsetispeed(&tty, B115200);

	tty.c_cflag |= (CLOCAL | CREAD);    // Ignore modem controls and enable reading
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;         // 8-bit characters
	tty.c_cflag &= ~PARENB;     // No parity bit
	tty.c_cflag &= ~CSTOPB;     // Only need 1 stop bit
	tty.c_cflag &= ~CRTSCTS;    // No hardware flowcontrol

	// Setup for non-canonical mode
	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	tty.c_oflag &= ~OPOST;

	// Fetch bytes as they become available
	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 1;

	if (tcsetattr(fd, TCSANOW, &tty) != 0) {
		PX4_INFO("Error from tcsetattr %s", strerror(errno));
		return -1;
	}

	const int BUF_SIZE = 1024;
	char readBuf[BUF_SIZE];

	while (true) {
		int numBytesRead = read(fd, readBuf, sizeof(readBuf));
		if (numBytesRead < 0) {
			PX4_INFO("Error reading: %s", strerror(errno));
			break;
		}

		if (numBytesRead > 1) {
			// std::cout << "numBytesRead " << numBytesRead << std::endl;
			int parsedCount = parser.parse(readBuf, numBytesRead);
#if defined(DEBUG_BUILD)
			PX4_INFO("Parsed %d messages", parsedCount);
#endif
		}
	}

	close(fd);
	return 0;
}
