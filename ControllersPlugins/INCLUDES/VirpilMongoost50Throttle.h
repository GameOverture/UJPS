#ifndef VIRPIL_MONGOOST_50_THROTTLE_
#define VIRPIL_MONGOOST_50_THROTTLE_


#include <QString>
using uint = unsigned int;


#ifndef INSIDE_PLUGIN
namespace VirpilMongoost50Throttle
#else
namespace VirpilMongoost50Throttle_
#endif
{
	const QString Description = "VPC-Throttle";
	
	// Axis
	const uint THR_LEFT = 0;
	const uint THR_RIGHT = 1;
	const uint AUX_AXIS  = 2;
	const uint THR_WHEEL = 3;
	const uint DIAL_A2 = 4;
	const uint DIAL_A1 = 5;

	// Buttons
	const uint BtnPinkyBack = 0;
	const uint BtnPinky = 1;
	const uint BtnMiddle = 2;

	const uint HatPointerUp = 3;
	const uint HatPointerDown = 4;
	const uint HatPointerLeft = 5;
	const uint HatPointerRight = 6;


	const uint HatThumbPress = 7;
	const uint HatThumbUp = 8;
	const uint HatThumbDown = 9;
	const uint HatThumbLeft = 10;
	const uint HatThumbRight = 11;

	const uint BtnThumb = 12;

	const uint HatThumbFarUp = 13;
	const uint HatThumbFarDown = 14;
	const uint HatThumbFarLeft = 15;
	const uint HatThumbFarRight = 16;

	const uint HatThumbCloseUp = 17;
	const uint HatThumbCloseDown = 18;
	const uint HatThumbCloseLeft = 19;
	const uint HatThumbCloseRight = 20;

	const uint B1 = 21;
	const uint B2 = 22;
	const uint B3 = 23;
	const uint B4 = 24;
	const uint B5 = 25;
	const uint B6 = 26;
	const uint B7 = 27;
	const uint B8 = 28;

	const uint T1up = 29;
	const uint T1LatchToggle = 30;

	const uint T2down = 31;

	const uint T3down = 32;

	const uint T4up = 33;
	const uint T4dwn = 34;

	const uint T5up = 35;
	const uint T5dwn = 36;

	const uint T6up = 37;
	const uint T6dwn = 38;

	const uint BigRedBtn = 39;

	const uint E1Press = 40;
	const uint E2Press = 41;
	const uint E3Press = 42;

	const uint E1Right = 43;
	const uint E1Left = 44;

	const uint E2Right = 45;
	const uint E2Left = 46;

	const uint E3Right = 47;
	const uint E3Left = 48;

	const uint Mode1 = 49;
	const uint Mode2 = 50;
	const uint Mode3 = 51;
	const uint Mode4 = 52;
	const uint Mode5 = 53;

	const uint BtnLatch = 54;
};

#endif
