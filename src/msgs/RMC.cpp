#include <NMEAParser.hpp>

void NMEAParser::handle_RMC(const char* msg)
{
	// $GNRMC,,V,,,,,,,,,,N,V*37

	// Recommended Minimum Specific GPS/Transit data. Time, date, position and speed data provided by the GNSS
	// Teseo. This sentence is transmitted at intervals not exceeding 2 seconds and is always accompanied by RMB
	// when destination way point is active.

	// $<TalkerID>RMC 	-- GN (multi-constellation), GP (GPS), GL (GLONASS), GA (GALILEO), BD (BEIDOU), QZ (QZSS)
	// <Timestamp> 		-- UTC time of GPS sample, hhmmss.sss
	// <Status> 		-- "A" = valid, "V" = Warning, "V" is reported in NO FIX conditions and "A" is reported in 2D and 3D fix conditions.
	// <Lat> 			-- Latitude as degrees:, DDDMM.MMMMM
	// <N/S> 			--  Latitude direction, "N" or "S"
	// <Long>			-- Longitude as degrees:, DDDMM.MMMMM
	// <E/W> 			-- Longitude direction, "E" or "W"
	// <Speed> 			-- Speed over ground in knots, ddd.d,
	// <Trackgood> 		-- Course made good, max. 999.9
	// <Date> 			-- Date of Fix: ddmmyy
	// <MagVar> 		-- Magnetic Variation, max.: 090.0
	// <MagVarDir> 		-- Magnetic Variation Direction, "E" or "W"
	// <mode> 			-- Positioning system Mode Indicator, "D" = Differential mode "A" = Autonomous mode "N" = data not valid "E" = Estimated (dead reckoning) mode
	// <Nav_status> 	-- Navigational status indicator: "S" = Safe "C" = Caution "U" = Unsafe "V" = Not valid
	// *
	// <checksum>
	// <cr><lf>

	RMC_Data data = {};

	char* endp = nullptr;
	if (msg && *(++msg) != ',') { data.timestamp = strtof(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { data.status = *(msg++); }

	if (msg && *(++msg) != ',') { data.lat = strtod(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { data.ns = *(msg++); }

	if (msg && *(++msg) != ',') { data.lon = strtod(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { data.ew = *(msg++); }

	if (msg && *(++msg) != ',') { data.speed = strtof(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { data.track_good = strtof(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { data.date = static_cast<int>(strtol(msg, &endp, 10)); msg = endp; }

	if (msg && *(++msg) != ',') { data.mag_var = strtof(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { data.mag_var_dir = *(msg++); }

	if (msg && *(++msg) != ',') { data.mode = *(msg++); }

	if (msg && *(++msg) != ',') { data.nav_status = *(msg++); }

#if defined(DEBUG_BUILD)
	PX4_INFO("handle_RMC");
#endif
}