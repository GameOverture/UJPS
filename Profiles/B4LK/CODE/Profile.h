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

	EnhancedJoystick *	m_pMcgPro;		// VKB Gunfighter mkII MCG Pro
	EnhancedJoystick *	m_pThrottle;	// Virpil Mongoost-50 Throttle
	EnhancedJoystick *	m_pPedals;		// Saitek Pro Flight Combat Rudder Pedals
	EnhancedJoystick *	m_pG13;			// Logitech G13
	VirtualJoystick *	m_pVJoy1;
	VirtualJoystick *	m_pVJoy2;

	uint				m_uiPreviousMode;

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

	void PulseBtn(VirtualJoystick *vj, uint vButton);

	void DoThrMode(uint uiMode);
	void DoStick();
	void DoStrafe();
	void DoToeBrake();
	void DoTriggerPull(TriggerStage eStage);
};
#endif
