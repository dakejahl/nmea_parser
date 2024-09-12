#include "NMEAParser.hpp"

#include <math.h>
#include <time.h>

void NMEAParser::update_gps_report()
{
	///// GGA
	if (_gga.ns == 'S') {
		_gga.lat = -_gga.lat;
	}

	if (_gga.ew == 'W') {
		_gga.lon = -_gga.lon;
	}

	// Convert to PX4 definitions
	switch (_gga.fix_quality) {
	case 1:
		// 3D fix
		_gps_report.fix_type = 3;
		break;

	case 2:
		// Differential
		_gps_report.fix_type = 4;
		break;

	case 6:
		// Dead reckoning we'll call 2D fix
		_gps_report.fix_type = 2;
		break;
	}

	_gps_report.longitude_deg = int(_gga.lon * 0.01) + (_gga.lon * 0.01 - int(_gga.lon * 0.01)) * 100.0 / 60.0;
	_gps_report.latitude_deg = int(_gga.lat * 0.01) + (_gga.lat * 0.01 - int(_gga.lat * 0.01)) * 100.0 / 60.0;
	_gps_report.hdop = _gga.hdop;
	_gps_report.altitude_msl_m = (double)_gga.alt;
	_gps_report.altitude_ellipsoid_m = (double)(_gga.alt + _gga.geo_sep);

	// Only report sats if there's a fix
	if (_gps_report.fix_type > 1) {
		_gps_report.satellites_used = _gga.sats;

	} else {
		_gps_report.satellites_used = 0;
	}

	///// GST
	_gps_report.eph = sqrtf(_gst.lat_err * _gst.lat_err + _gst.lon_err * _gst.lon_err);
	_gps_report.epv = _gst.alt_err;
	_gps_report.ehpe = _gst.ehpe;

	///// GSA
	_gps_report.hdop = _gsa.hdop;
	_gps_report.vdop = _gsa.vdop;

	///// RMC
	float velocity_ms = _rmc.speed / 1.9438445f; // knots to m/s
	float track_rad = _rmc.track_good * M_PI / 180.0f; // rad in range [0, 2pi]

	_gps_report.vel_m_s = velocity_ms;
	_gps_report.vel_n_m_s = velocity_ms * cosf(track_rad);
	_gps_report.vel_e_m_s = velocity_ms * sinf(track_rad);
	_gps_report.cog_rad = track_rad;

	_gps_report.vel_ned_valid = _rmc.mode != 'N';

	// If RMC says No Fix
	if (_rmc.status == 'V') {
		_gps_report.fix_type = 0;
	}

	// Calculate UTC time since epoch
	double utc_time = _rmc.timestamp;
	int utc_hour = static_cast<int>(utc_time / 10000);
	int utc_minute = static_cast<int>((utc_time - utc_hour * 10000) / 100);
	double utc_sec = static_cast<double>(utc_time - utc_hour * 10000 - utc_minute * 100);
	int nmea_day = static_cast<int>(_rmc.date / 10000);
	int nmea_mth = static_cast<int>((_rmc.date - nmea_day * 10000) / 100);
	int nmea_year = static_cast<int>(_rmc.date - nmea_day * 10000 - nmea_mth * 100);
	// convert to unix timestamp
	struct tm timeinfo = {};
	timeinfo.tm_year = nmea_year + 100;
	timeinfo.tm_mon = nmea_mth - 1;
	timeinfo.tm_mday = nmea_day;
	timeinfo.tm_hour = utc_hour;
	timeinfo.tm_min = utc_minute;
	timeinfo.tm_sec = int(utc_sec);
	timeinfo.tm_isdst = 0;

	time_t epoch = mktime(&timeinfo);
	uint64_t usecs = static_cast<uint64_t>((utc_sec - static_cast<uint64_t>(utc_sec)) * 1000000);
	_gps_report.time_utc_usec = static_cast<uint64_t>(epoch) * 1000000ULL;
	_gps_report.time_utc_usec += usecs;
}

// Append data to the internal buffer, process the buffer, and return the number of messages parsed.
int NMEAParser::parse(const char* buffer, int length)
{
	if (_buffer_length + length < sizeof(_buffer)) {
		memcpy(_buffer + _buffer_length, buffer, length);
		_buffer_length += length;

	} else {
		PX4_INFO("buffer overflow -- clearing");
		_log_writer->writeTextMessage(ulog_cpp::Logging::Level::Info, "buffer overflow -- clearing", currentTimeUs());
		_buffer_length = 0;
	}

	int messages_parsed = process_buffer();

	// Update the _gps_report
	if (messages_parsed) {
		update_gps_report();
	}

	return messages_parsed;
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
			_log_writer->writeTextMessage(ulog_cpp::Logging::Level::Info, "Invalid checksum", currentTimeUs());
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
		_gga = handle_GGA(buffer + 6);

	} else if ((memcmp(buffer + 3, "RMC,", 4) == 0) && (comma_count >= 11)) {
		_rmc = handle_RMC(buffer + 6);

	} else if ((memcmp(buffer + 3, "GST,", 4) == 0) && (comma_count == 8)) {
		_gst = handle_GST(buffer + 6);

	} else if ((memcmp(buffer + 3, "GSA,", 4) == 0) && (comma_count >= 17)) {
		_gsa = handle_GSA(buffer + 6);

	} else if ((memcmp(buffer + 3, "VTG,", 4) == 0) && (comma_count >= 8)) {
		_vtg = handle_VTG(buffer + 6);

	} else if ((memcmp(buffer + 1, "PSTM,", 5) == 0)) {
		PX4_INFO("Got PSTM return: %s", buffer);

		// TODO: GSV, GBS
	} else {
		char msg[4];
		memcpy(msg, buffer + 3, 3);
		msg[3] = '\0';
		PX4_INFO("unknown message: %s", msg);
		_log_writer->writeTextMessage(ulog_cpp::Logging::Level::Info, msg, currentTimeUs());
	}
}

void NMEAParser::send_test_command()
{
// Reset to defaults
// Enable SBAS and report SBAS
// Enables Galileo and BeiDou
// Set baudrate to 115200
// Set fix rate to 10Hz
// save params
// reset to apply

// $PSTMRESTOREPAR
// $PSTMSETPAR,1200,0x1000000C,1
// $PSTMSETPAR,1227,0x3C0,1
// $PSTMSETPAR,1102,0xA
// $PSTMSETPAR,1303,0.1
// $PSTMSAVEPAR
// $PSTMSRR


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
