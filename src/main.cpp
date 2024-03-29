#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <signal.h>

#include "NMEAParser.hpp"

#if defined(LOG_RAW)
#include <iostream>
#include <fstream>
#endif

static bool _should_exit = false;
static void signal_handler(int signum);
std::shared_ptr<ulog_cpp::SimpleWriter> _log_writer = nullptr;

int main()
{
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	setbuf(stdout, NULL); // Disable stdout buffering

#if defined(LOG_RAW)
	// Writes the raw data from serial port to a file
	std::ofstream raw_data_file;
	raw_data_file.open("raw.txt", std::ios::out | std::ios::trunc);

	if (!raw_data_file.is_open()) {
		std::cerr << "Error opening file for writing.\n";
		return -1;
	}

#endif

	const char* port = "/dev/ttyUSB0";
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

	// Setup ulog writer
	_log_writer = std::make_shared<ulog_cpp::SimpleWriter>("nmea.ulg", currentTimeUs());
	_log_writer->writeInfo("NMEAParser", "NMEADataLogger");
	_log_writer->writeMessageFormat(SensorGps::messageName(), SensorGps::fields());
	_log_writer->headerComplete();
	const uint16_t msg_id = _log_writer->writeAddLoggedMessage(SensorGps::messageName());
	_log_writer->writeTextMessage(ulog_cpp::Logging::Level::Info, "Hello world", currentTimeUs());

	// Setup buffer and begin read/parse/report loop
	const int BUF_SIZE = 1024;
	char buffer[BUF_SIZE];
	// flush, for some reason always read two null bytes first
	read(fd, buffer, sizeof(buffer));

	NMEAParser parser;

	// Send configuration commands

	parser.send_test_command();

	while (!_should_exit) {
		int bytes_read = read(fd, buffer, sizeof(buffer));

		if (bytes_read < 0) {
			PX4_INFO("Error reading: %s", strerror(errno));
			break;
		}

		if (bytes_read > 0) {
			int parsedCount = parser.parse(buffer, bytes_read);

			// Collect data and stamp it
			auto gps_data = parser.gps_report();
			gps_data.timestamp = currentTimeUs();

			// Write to ulog
			_log_writer->writeData(msg_id, gps_data);

#if defined(DEBUG_BUILD)
			PX4_INFO("Parsed %d messages", parsedCount);
#endif
#if defined(LOG_RAW)
			raw_data_file.write(buffer, bytes_read);
#endif

		}
	}

	close(fd);
	return 0;
}

static void signal_handler(int signum)
{
	_should_exit = true;
}