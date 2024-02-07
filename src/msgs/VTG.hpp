#include <NMEAParser.hpp>

void NMEAParser::handle_VTG(const char* msg)
{
	// $GPVTG,73.2,T,,M,0.2,N,0.4,K,D*50
	// $GNVTG,,T,,M,,N,,K,N*32

	// Course over ground and ground speed, this message provides the actual course and speed relative to ground.

	// $<TalkerID>VTG 	-- GN (multi-constellation), GP (GPS), GL (GLONASS), GA (GALILEO), BD (BEIDOU), QZ (QZSS)
	// <TMGT> 			-- Track in reference to "true" earth poles, ddd.d in degrees
	// <T> 				-- Indicates "terrestrial"
	// <TMGM>			-- Track in reference to "magnetic" earth poles, ddd.d in degrees
	// <M> 				-- Indicates "magnetic"
	// <SoGN>			-- Speed over Ground in knots
	// <N> 				-- Indicates "knots"
	// <SoGK>			-- Speed over Ground in kilometers per hour, ddd.d in km/h
	// <K> 				-- Indicates "kilometres"
	// <D> 				-- Mode indicator: A = Autonomous mode, D = Differential mode, E = Estimated mode, N = none??(jake observed)
	// *
	// <checksum>
	// <cr><lf>

	char* endp = nullptr;

	if (msg && *(++msg) != ',') { _vtg.track_true = strtof(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { _vtg.T = *(msg++); }

	if (msg && *(++msg) != ',') { _vtg.track_mag = strtof(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { _vtg.M = *(msg++); }

	if (msg && *(++msg) != ',') { _vtg.ground_speed_knots = strtof(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { _vtg.N = *(msg++); }

	if (msg && *(++msg) != ',') { _vtg.ground_speed_kmh = strtof(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { _vtg.K = *(msg++); }

	if (msg && *(++msg) != ',') { _vtg.mode = *(msg++); }


#if defined(DEBUG_BUILD)
	_vtg.print();
#endif
}