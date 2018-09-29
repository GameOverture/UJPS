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
	const uint BtnLatch = 0;
	
	const uint BtnPinky = 1;
	const uint BtnPinkyBack = 2;

	const uint HatPointerLeft = 3;
	const uint HatPointerDown = 4;
	const uint HatPointerRight = 5;
	const uint HatPointerUp = 6;

	const uint BtnMiddle = 7;
	const uint BtnThumb = 8;

	const uint HatThumbPress = 9;
	const uint __UnknownBtn = 10;
	const uint HatThumbLeft = 11;
	const uint HatThumbDown = 12;
	const uint HatThumbRight = 13;
	const uint HatThumbUp = 14;

	const uint HatThumb2Right = 15;
	const uint HatThumb2Down = 16;
	const uint HatThumb2Left = 17;
	const uint HatThumb2Up = 18;

	const uint HatThumb3Right = 19;
	const uint HatThumb3Down = 20;
	const uint HatThumb3Left = 21;
	const uint HatThumb3Up = 22;

	const uint __UnknownBtn2 = 23;
	const uint __UnknownBtn3 = 24;
	const uint __UnknownBtn4 = 25;
	const uint __UnknownBtn5 = 26;

	const uint T2dwn = 27;
	const uint T2up = 28;

	const uint T3dwn = 29;
	const uint T3up = 30;

	const uint T1dwn = 31;
	const uint T1up = 32;

	const uint _UnknownBtn6 = 33;
	const uint _UnknownBtn7 = 34;

	const uint B8 = 35;
	const uint B7 = 36;
	const uint B6 = 37;
	const uint B5 = 38;
	const uint B1 = 39;
	const uint B2 = 40;
	const uint B3 = 41;
	const uint B4 = 42;

	const uint T6up = 43;
	const uint T5up = 44;
	const uint T4up = 45;
	const uint T4dwn = 46;
	const uint T5dwn = 47;
	const uint T6dwn = 48;

	const uint BigRedBtn = 49;

	const uint __UnknownBtn8 = 50;

	const uint E1Press = 51;
	const uint E3Press = 52;
	const uint E2Press = 53;

	const uint ModeSelect1 = 54;
	const uint ModeSelect2 = 55;
	const uint ModeSelect3 = 56;
	const uint ModeSelect4 = 57;
	const uint ModeSelect5 = 58;
};

#endif
