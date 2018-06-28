#ifndef PROFILE
#define PROFILE

#include "AbstractProfile.h"
class EnhancedJoystick;

class Profile : public AbstractProfile
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "Profile")
	Q_INTERFACES(AbstractProfile)

	const uint			m_uiPULSE_AMT;

	EnhancedJoystick *	m_pJoystick;	// VKB Gunfighter mkII MCG Pro
	EnhancedJoystick *	m_pThrottle;	// Thrustmaster Warthog Throttle
	EnhancedJoystick *	m_pPedals;		// Saitek Pro Flight Combat Rudder Pedals
	VirtualJoystick *	m_pVirtualJoy1;	// Virtual joystick #1
	VirtualJoystick *	m_pVirtualJoy2;	// Virtual joystick #2

	bool m_bBacklit;
	quint8 m_iBrightness;

	// shields and targeting modes
	uint ncPulse;
	bool m_bShieldsHorizontalMode;
	uint m_targetsTypeToCycle;

public:
	Profile();
	Profile(const Profile &other) = delete;
	Profile(Profile &&other) = delete;
	Profile& operator=(const Profile &other) = delete;
	Profile& operator=(Profile &&other) = delete;
	virtual ~Profile();
		
	virtual void stop() override final;

private:
	virtual bool setupJoysticks() override final;
	virtual void runFirstStep() override final;

	// led configuration
	void ledBrightnessDown();
	void ledBrightnessUp();
	void toggleBacklit();

	// control mode
	void setControlsGround();
	void setControlsFlightLanding();
	void setControlsFlightCruise();

	// targeting
	void browseHostileTargets();
	void browseAllTargets();
	void browseFriendTargets();
	void browsePinnedTargets();
	void previousTarget();
	void nextTarget();

	// shields
	void switchShieldsMode();
	void shieldsDownArrow();
	void shieldsUpArrow();
	void releaseLongiShieldsButtons();

	// stick trims
	void reset_dxxy_trims();
	void set_dxxy_trims();

	// control modes sub-functions
	void set_JOYXY_for_turn();
	void set_JOYXY_for_landing();
	void set_JOYXY_for_strafe();
	void set_JOYXY_for_rollNpitch();

	void set_S4_for_horn();
	void set_S4_for_landing();
	void set_S4_for_nothing();

	void set_BUTTONS_for_landing();
	void set_BUTTONS_for_cruise();

	void reset_THR_and_pedals();
	void set_THR_and_pedals();
	void set_THR_for_nothing();
	void set_THR_for_landing();
	void set_THR_for_cruise();

	void set_BRKRIGHT_for_groundForward();
	void set_BRKRIGHT_for_groundBackward();
	void set_THRLEFT_for_verticalStrafe();
	void set_THRLEFT_for_nothing();
	void set_THRRIGHT_for_flightThrottle();
};
#endif
