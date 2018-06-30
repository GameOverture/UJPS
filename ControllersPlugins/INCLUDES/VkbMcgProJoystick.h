#ifndef VKB_MCG_PRO_JOYSTICK_
#define VKB_MCG_PRO_JOYSTICK_


#include <QString>
using uint = unsigned int;


#ifndef INSIDE_PLUGIN
namespace VkbMcgProJoystick
#else
namespace VkbMcgProJoystick_
#endif
{
	const QString Description = "VKBsim Gunfighter Modern Combat PRO ";	// NOTE: there's actually a space at the end of the description (lol)

	const uint JOYX = 0;
	const uint JOYY = 1;

	const uint TRIMX = 3;	// Upper analog stick
	const uint TRIMY = 2;	// Upper analog stick

	const uint SLEWX = 5;	// Bottom-Left analog stick
	const uint SLEWY = 4;	// Bottom-Left analog stick

	const uint BRAKE = 7;	// Hand Brake

	
	const uint TG1 = 0;			// Trigger stage 1
	const uint TG2 = 1;			// Trigger stage 2
	const uint TG3 = 2;			// Folding trigger pulled detent
	const uint BtnRed = 3;		// Red button
	const uint BtnTopRight = 4;
	const uint BtnMidRight = 5;
	const uint BtnGrip = 6;
	const uint PressSlew = 7;
	const uint HatGripPress = 8;
	const uint HatGripUp = 9;
	const uint HatGripRight = 10;
	const uint HatGripDown = 11;
	const uint HatGripLeft = 12;
	const uint HatMidUp = 13;
	const uint HatMidRight = 14;
	const uint HatMidDown = 15;
	const uint HatMidLeft = 16;
	const uint HatMidPress = 17;
	const uint HatHeadUp = 18;
	const uint HatHeadRight = 19;
	const uint HatHeadDown = 20;
	const uint HatHeadLeft = 21;
	const uint HatHeadPress = 22;
	const uint PressTrim = 23;
};

#endif
