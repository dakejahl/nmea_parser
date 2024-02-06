#include <iostream>
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
    int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        std::cerr << "Error opening " << portname << ": " << strerror(errno) << std::endl;
        return -1;
    }

    // Configure the serial port
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) != 0) {
        std::cerr << "Error from tcgetattr: " << strerror(errno) << std::endl;
        return -1;
    }

    cfsetospeed(&tty, B115200); // Set baud rate (you might need to change this)
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
        std::cerr << "Error from tcsetattr: " << strerror(errno) << std::endl;
        return -1;
    }

    // Buffer for reading from the serial port
    const int BUF_SIZE = 1024;
    char readBuf[BUF_SIZE];

    while (true) {
        int numBytesRead = read(fd, readBuf, sizeof(readBuf));
        if (numBytesRead < 0) {
            std::cerr << "Error reading: " << strerror(errno) << std::endl;
            break;
        }

        if (numBytesRead > 0) {
            int parsedCount = parser.parse(readBuf, numBytesRead);
            std::cout << "Parsed " << parsedCount << " messages." << std::endl;
        }
    }

    close(fd);
    return 0;
}
