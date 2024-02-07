#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include "NMEAParser.hpp"

#if defined(LOG_RAW)
#include <iostream>
#include <fstream>
#endif

int main() {

#if defined(LOG_RAW)
	std::ofstream raw_data_file;
	raw_data_file.open("raw.txt", std::ios::out | std::ios::trunc);

	if (!raw_data_file.is_open()) {
		std::cerr << "Error opening file for writing.\n";
		return -1;
	}
#endif

	const char* port = "/dev/ttyUSB0";
	NMEAParser parser;

	// Open the serial port
	int fd = open(port, O_RDWR | O_NOCTTY);
	if (fd < 0) {
		PX4_INFO("Error opening %s: %s", port, strerror(errno));
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
	char buffer[BUF_SIZE];

	while (1) {
		int bytes_read = read(fd, buffer, sizeof(buffer));
		if (bytes_read < 0) {
			PX4_INFO("Error reading: %s", strerror(errno));
			break;
		}

		if (bytes_read > 1) {
			// std::cout << "bytes_read " << bytes_read << std::endl;
			int parsedCount = parser.parse(buffer, bytes_read);

			auto gps = parser.gps_report();

			// Parser stores data and residual bytes remain in internal buffer

			// TODO: query sensor_gps_s from the parser

#if defined(DEBUG_BUILD)
			PX4_INFO("Parsed %d messages", parsedCount);
#endif
#if defined(LOG_RAW)
			PX4_INFO("writing to file: %d", bytes_read);
			raw_data_file.write(buffer, bytes_read);
			raw_data_file.flush();
#endif
		}
	}

	close(fd);
	return 0;
}
