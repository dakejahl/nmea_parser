#include <NMEAParser.hpp>

void NMEAParser::handle_GSA(const char* msg)
{
	// $GNGSA,A,3,23,03,22,09,01,19,17,06,31,11,,,1.1,0.6,0.9,1*3E

	// GNSS DOP and Active Satellites. Satellites from different constellations are sent on separate messages.
	// In case of multi-constellation mode, the talker ID is always GN.

	// $<TalkerID>GSA 	-- GN (multi-constellation), GP (GPS), GL (GLONASS), GA (GALILEO), BD (BEIDOU), QZ (QZSS)
	// <Mode> 			-- M = manual, A = automatic
	// <CurrentMode> 	-- 1 = Fix not available or invalid, 2 = GPS SPS Mode fix valid, 3 = Differential GPS SPS Mode fix valid
	// <SatPRN1> 		-- Satellites list used for positioning. See Preliminary notes about satellites PRN ranges for more info about available values.
	// ...
	// <SatPRNN> 		--
	// <PDOP> 			-- Position Dilution of Precision, max: 99.0
	// <HDOP> 			-- Horizontal Dilution of Precision, max: 99.0
	// <VDOP> 			-- Vertical Dilution of Precision, max: 99.0
	// <SystemID> 		-- 1 = GPS, 2 = GLONASS, 3 = GALILEO, 4 = BEIDOU, 5 = QZSS
	// *
	// <checksum>
	// <cr><lf>

	char* endp = nullptr;

	if (msg && *(++msg) != ',') { _gsa.mode = *(msg++); }

	if (msg && *(++msg) != ',') { _gsa.current_mode = strtol(msg, &endp, 10); msg = endp; }

	for (int i = 0; i < 12; i++) {
		if (msg && *(++msg) != ',') { _gsa.sat_prn[i] = strtol(msg, &endp, 10); msg = endp; }
	}

	if (msg && *(++msg) != ',') { _gsa.pdop = strtof(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { _gsa.hdop = strtof(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { _gsa.vdop = strtof(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { _gsa.system_id = *(msg++); }

#if defined(DEBUG_BUILD)
	_gsa.print();
#endif
}
