#ifndef PROFILE
#define PROFILE

#include "AbstractProfile.h"
class EnhancedJoystick;

enum TriggerStage
{
	Stage1 = 1,
	Stage2,
	Mushy
};

class Profile : public AbstractProfile
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "Profile")
	Q_INTERFACES(AbstractProfile)

	const uint			m_uiPULSE_AMT;

	EnhancedJoystick *	m_pMcgPro;	// VKB Gunfighter mkII MCG Pro
	EnhancedJoystick *	m_pThrottle;	// Thrustmaster Warthog Throttle
	EnhancedJoystick *	m_pPedals;		// Saitek Pro Flight Combat Rudder Pedals
	EnhancedJoystick *	m_pG13;			// Logitech G13
	VirtualJoystick *	m_pVJoy1;
	VirtualJoystick *	m_pVJoy2;

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

	void DoStick();
	void DoStrafe();
	void DoToeBrake();
	void DoTriggerPull(TriggerStage eStage);
};
#endif
