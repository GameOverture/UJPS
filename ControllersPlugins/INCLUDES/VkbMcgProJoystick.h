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


	// VKB controllers can have their firmware programmed to output basically any axis or button. These mappings reflect my personal firmware programming, but
	// they are close to the factory defaults. The only significant change I made was having the Trim analog stick press output button index 23, instead of it 
	// acting as a toggle between a HAT and Trim axis. I also made the trim axis act as absolute instead of relative (but that affects how you write your 
	// profile more than the below mappings).

	// Axis
	const uint JOYX = 0;
	const uint JOYY = 1;

	const uint TRIMX = 3;	// Upper analog stick
	const uint TRIMY = 2;	// Upper analog stick

	const uint SLEWX = 5;	// Bottom-Left analog stick
	const uint SLEWY = 4;	// Bottom-Left analog stick

	const uint BRAKE = 7;	// Hand Brake

	// Buttons
	const uint TriggerStage1 = 0;
	const uint TriggerStage2 = 1;
	const uint FoldingTriggerDetent = 2;
	const uint BtnRed = 3;
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
