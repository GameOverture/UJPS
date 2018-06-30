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
	const QString Description = "Virpil Mongoost 50 Throttle";
	
	// Axis
	const uint THR_LEFT = 0;
	const uint THR_RIGHT = 1;
	const uint AUX_AXIS  = 2;
	const uint DIAL_A1 = 3;
	const uint DIAL_A2 = 4;
	const uint THR_WHEEL = 5;
	const uint THR_ANALOGX = 6;
	const uint THR_ANALOGY = 7;

	// Buttons
	const uint B1_1 = 0;
	const uint B2_1 = 1;
	const uint B3_1 = 2;
	const uint B4_1 = 3;
	const uint B5_1 = 4;
	const uint B6_1 = 5;
	const uint B7_1 = 6;
	const uint B8_1 = 7;

	const uint B1_2 = 8;
	const uint B2_2 = 9;
	const uint B3_2 = 10;
	const uint B4_2 = 11;
	const uint B5_2 = 12;
	const uint B6_2 = 13;
	const uint B7_2 = 14;
	const uint B8_2 = 15;

	const uint B1_3 = 16;
	const uint B2_3 = 17;
	const uint B3_3 = 18;
	const uint B4_3 = 19;
	const uint B5_3 = 20;
	const uint B6_3 = 21;
	const uint B7_3 = 22;
	const uint B8_3 = 23;

	const uint B1_4 = 24;
	const uint B2_4 = 25;
	const uint B3_4 = 26;
	const uint B4_4 = 27;
	const uint B5_4 = 28;
	const uint B6_4 = 29;
	const uint B7_4 = 30;
	const uint B8_4 = 31;

	const uint B1_5 = 32;
	const uint B2_5 = 33;
	const uint B3_5 = 34;
	const uint B4_5 = 35;
	const uint B5_5 = 36;
	const uint B6_5 = 37;
	const uint B7_5 = 38;
	const uint B8_5 = 39;

	const uint BtnPinky = 40;
	const uint BtnRing = 41;
	const uint BtnMiddle = 42;
	const uint BtnThumb = 43;

	const uint T1 = 44;
	const uint T2 = 45;
	const uint T3 = 46;

	const uint T4_Up = 47;
	const uint T4_Down = 48;
	const uint T5_Up = 49;
	const uint T5_Down = 50;
	const uint T6_Up = 51;
	const uint T6_Down = 52;

	const uint THR_SW1_Up = 53;
	const uint THR_SW1_Down = 54;
	const uint THR_SW2_Up = 55;
	const uint THR_SW2_Down = 56;

	const uint THR_HAT_Up = 57;
	const uint THR_HAT_Down = 58;
	const uint THR_HAT_Left = 59;
	const uint THR_HAT_Right = 60;
	const uint THR_HAT_Press = 61;

	const uint E1_Press = 62;
	const uint E2_Press = 63;
	const uint E3_Press = 64;

	const uint B9 = 65;
	
	// Hats
	const uint THR_HAT = 0;
	const uint E1 = 1;
	const uint E2 = 2;
	const uint E3 = 3;
};

#endif
