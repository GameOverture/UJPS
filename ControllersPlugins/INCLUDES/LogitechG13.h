#ifndef LOGITECH_G13_
#define LOGITECH_G13_


#include <QString>
using uint = unsigned int;


#ifndef INSIDE_PLUGIN
namespace LogitechG13
#else
namespace LogitechG13_
#endif
{
	const QString Description = "Logitech G13 Joystick";
	
	const uint JOYX = 1;	// X stick axis
	const uint JOYY = 0;	// Y stick axis

	const uint PRESS = 0;	// Pressing the analog stick
};

#endif
