#ifndef SAITEK_PRO_FLIGHT_COMBAT_RUDDER_PEDALS_
#define SAITEK_PRO_FLIGHT_COMBAT_RUDDER_PEDALS_


#include <QString>
using uint = unsigned int;


#ifndef INSIDE_PLUGIN
namespace SaitekProFlightCombatRudderPedals
#else
namespace SaitekProFlightCombatRudderPedals_
#endif
{
	const QString Description = "Saitek Pro Flight Combat Rudder Pedals (USB)";
	
	const uint RUDDER    = 2;
	const uint BRK_LEFT  = 1;
	const uint BRK_RIGHT = 0;
};

#endif
