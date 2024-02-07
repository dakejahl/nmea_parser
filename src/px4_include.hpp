#pragma once

#include <cstdint>

#ifndef PX4_INFO
#define PX4_INFO(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)
#endif

struct SensorGps {
	uint64_t timestamp;
	uint64_t timestamp_sample;
	double latitude_deg;
	double longitude_deg;
	double altitude_msl_m;
	double altitude_ellipsoid_m;
	uint64_t time_utc_usec;
	uint32_t device_id;
	float s_variance_m_s;
	float c_variance_rad;
	float eph;
	float epv;
	float hdop;
	float vdop;
	float ehpe;
	int32_t noise_per_ms;
	int32_t jamming_indicator;
	float vel_m_s;
	float vel_n_m_s;
	float vel_e_m_s;
	float vel_d_m_s;
	float cog_rad;
	int32_t timestamp_time_relative;
	float heading;
	float heading_offset;
	float heading_accuracy;
	float rtcm_injection_rate;
	uint16_t automatic_gain_control;
	uint8_t fix_type;
	uint8_t jamming_state;
	uint8_t spoofing_state;
	bool vel_ned_valid;
	uint8_t satellites_used;
	uint8_t selected_rtcm_instance;
	bool rtcm_crc_failed;
	uint8_t rtcm_msg_used;
	uint8_t _padding0[6]; // required for logger

	static constexpr uint8_t JAMMING_STATE_UNKNOWN = 0;
	static constexpr uint8_t JAMMING_STATE_OK = 1;
	static constexpr uint8_t JAMMING_STATE_WARNING = 2;
	static constexpr uint8_t JAMMING_STATE_CRITICAL = 3;
	static constexpr uint8_t SPOOFING_STATE_UNKNOWN = 0;
	static constexpr uint8_t SPOOFING_STATE_NONE = 1;
	static constexpr uint8_t SPOOFING_STATE_INDICATED = 2;
	static constexpr uint8_t SPOOFING_STATE_MULTIPLE = 3;
	static constexpr uint8_t RTCM_MSG_USED_UNKNOWN = 0;
	static constexpr uint8_t RTCM_MSG_USED_NOT_USED = 1;
	static constexpr uint8_t RTCM_MSG_USED_USED = 2;
};