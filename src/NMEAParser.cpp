#include "NMEAParser.hpp"

#include <msgs/GBS.hpp>
#include <msgs/GGA.hpp>
#include <msgs/GSA.hpp>
#include <msgs/GST.hpp>
#include <msgs/GSV.hpp>
#include <msgs/RMC.hpp>
#include <msgs/VTG.hpp>
#include <msgs/ZDA.hpp>

// Append data to the internal buffer, process the buffer, and return the number of messages parsed.
int NMEAParser::parse(const char* buffer, int length)
{
	if (_buffer_length + length < sizeof(_buffer)) {
		memcpy(_buffer + _buffer_length, buffer, length);
		_buffer_length += length;

	} else {
		PX4_INFO("buffer overflow -- clearing");
		_buffer_length = 0;
	}

	int messages_parsed = process_buffer();

	// Update the _gps_report
	if (messages_parsed) {
		update_gps_report();
	}

	return messages_parsed;
}

void NMEAParser::update_gps_report()
{
	// Move data from NMEA-structs to SensorGps-struct
}

// Handles a NMEA message which has already been validated.
void NMEAParser::handle_nmea_message(const char* buffer, int length)
{
	// For each message type a certain number of commas are expected
	int comma_count = 0;

	for (int i = 0 ; i < length; i++) {
		if (buffer[i] == ',') {
			comma_count++;
		}
	}

	// The ID starts after the first 3 bytes ($GN)
	// The data starts after the first 6 bytes ($GNRMC)

	if ((memcmp(buffer + 3, "GGA,", 4) == 0) && (comma_count >= 14)) {
		handle_GGA(buffer + 6);

	} else if ((memcmp(buffer + 3, "RMC,", 4) == 0) && (comma_count >= 11)) {
		handle_RMC(buffer + 6);

	} else if ((memcmp(buffer + 3, "GST,", 4) == 0) && (comma_count == 8)) {
		handle_GST(buffer + 6);

	} else if ((memcmp(buffer + 3, "GSA,", 4) == 0) && (comma_count >= 17)) {
		handle_GSA(buffer + 6);

	} else if ((memcmp(buffer + 3, "VTG,", 4) == 0) && (comma_count >= 8)) {
		handle_VTG(buffer + 6);

		// } else if ((memcmp(buffer + 3, "GBS,", 4) == 0) && (comma_count >= 69)) {
		// 	// TODO: this isn't implemented yet
		// 	handle_GBS(buffer + 6);

		// } else if ((memcmp(bufmakfer + 3, "ZDA,", 4) == 0) && (comma_count == 6)) {
		// 	handle_ZDA(buffer + 6);

		// } else if ((memcmp(buffer + 3, "GSV,", 4) == 0)) {
		// 	handle_GSV(buffer + 6);

	} else {
		char msg[4];
		memcpy(msg, buffer + 3, 3);
		msg[4] = '\0';
		PX4_INFO("unknown message: %s", msg);
	}
}

// Process the buffer and return the number of messages parsed.
int NMEAParser::process_buffer()
{
	int messages_parsed = 0;
	int start_pos = 0;
	int bytes_remaining = _buffer_length;
	int message_length = 0;

	while (bytes_remaining != 0) {

		bool found_message_frame = false;
		const char* start = nullptr;
		const char* end = nullptr;

		// Find the start ($) and end (*) an NMEA message
		for (int i = start_pos; i < _buffer_length; i++) {
			if (_buffer[i] == '$') {
				start = &_buffer[i];

			} else if (_buffer[i] == '*') {
				end = &_buffer[i];
			}

			message_length = end - start + 5;

			if (start && end && end > start && message_length <= bytes_remaining) {
				found_message_frame = true;
				break;
			}
		}

		if (!found_message_frame) {
			// 1. No start or end characters found
			// 2. No start character found _before_ an end character
			// 3. Start found but not end (incomplete, most common)
			// 4. Start and end found but we may be missing the checksum and CR LF bytes
			break;
		}

		if (validate_checksum(start, message_length)) {
			messages_parsed++;

			// Updates GNSS struct
			handle_nmea_message(start, message_length);

			// Increment to the start of next expected message
			start_pos = (start - _buffer) + message_length;

		} else {
			PX4_INFO("Invalid checksum");
			// If checksum is invalid or message incomplete, move start_pos just after this '$'
			start_pos = (start - _buffer) + 1;
		}

		bytes_remaining = _buffer_length - start_pos;
	}

	// If buffer iterator start_pos isn't pointing to the end of the buffer, shift remaining data to the beginning of the buffer
	if (start_pos < _buffer_length) {
		memmove(_buffer, _buffer + start_pos, bytes_remaining);
		_buffer_length -= start_pos;

#if defined(DEBUG_BUILD)
		PX4_INFO("Incomplete message");
		PX4_INFO("bytes_remaining: %d", bytes_remaining);

		for (size_t i = 0; i < _buffer_length; i++) {
			printf("%c", _buffer[i]);
		}

		printf("\n\n");
#endif

	} else {
		_buffer_length = 0;
	}

	return messages_parsed;
}

bool NMEAParser::validate_checksum(const char* nmea_message, int length)
{
	// Example --  $GNGSA,A,1,,,,,,,,,,,,,0.0,0.0,0.0,4*36<CR><LF>
	int recv_checksum = 0;
	unsigned char calc_checksum = 0;
	char recv_checksum_str[3] = {nmea_message[length - 4], nmea_message[length - 3], '\0'};
	sscanf(recv_checksum_str, "%X", &recv_checksum);

	// Start after $ and end before *
	for (int i = 1; i < length - 5; i++) {
		calc_checksum ^= nmea_message[i];
	}

	return recv_checksum == calc_checksum;
}
