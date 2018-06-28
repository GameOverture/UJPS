#include "Profile.h"
#include "StarCitizenControls.h"

// UjpsCore includes
#include "MAPPINGS/Mappings.h"
#include "TRIGGERS/Triggers.h"
#include "ACTIONS/Actions.h"
#include "WindowsKeys.h"

// Virtual/Real joystick includes'
#include "VirtualJoystick.h"
#include "EnhancedJoystick.h"

// Plugin mappings
#include "ThrustmasterWarthogJoystick.h"
#include "ThrustmasterWarthogThrottle.h"
#include "SaitekProFlightCombatRudderPedals.h"

#include "Lim.h"

LayersCombo AllLayers{};
using namespace Keys;

namespace MCGP = ThrustmasterWarthogJoystick;
namespace TMWT = ThrustmasterWarthogThrottle;
namespace SPFC = SaitekProFlightCombatRudderPedals;
namespace SCK = StarCitizenControls_Keyboard;
namespace SC1 = StarCitizenControls_vJoy1;
namespace SC2 = StarCitizenControls_vJoy2;

#define LOG_MSG(str) emit message(str, Qt::black)
#define LOG_ERROR(str) emit message(str, Qt::red)

const bool  bUSE_LED = true;
const float fBRAKE_RIGHT_LDZ = 0.055f;
const float fBRAKE_RIGHT_RDZ = 0.1f;
const float fBRAKE_THRESHOLD = -0.80f;

Profile::Profile() :	AbstractProfile(),
						m_uiPULSE_AMT(ms2cycles(150)),	// 150 ms for Star Citizen because of the current low framerate
						m_pJoystick(nullptr),			// VKB Gunfighter mkII MCG Pro
						m_pThrottle(nullptr),			// Thrustmaster Warthog Throttle
						m_pPedals(nullptr),				// Saitek Pro Flight Combat Rudder Pedals
						m_pVirtualJoy1(nullptr),		// virtual joystick #1
						m_pVirtualJoy2(nullptr)			// virtual joystick #2
{
	m_bShieldsHorizontalMode = true;
	m_targetsTypeToCycle = 2;
	
	m_bBacklit = false;
	m_iBrightness = 1;
}

Profile::~Profile()
{
	this->stop();
}

/*virtual*/ void Profile::stop() /*override final*/
{
	// set led brightness at 0
	if(m_pThrottle)
	{
		m_pThrottle->setData("BRIGHTNESS",0);
		m_pThrottle->flush();
	}
	
	// UnmapAll, delete real and virtual joysticks
	this->AbstractProfile::stop();
	
	m_pJoystick = nullptr;
	m_pThrottle = nullptr;
	m_pPedals = nullptr;
	m_pVirtualJoy1  = nullptr;
	m_pVirtualJoy2  = nullptr;
}

/*virtual*/ bool Profile::setupJoysticks() /*override final*/
{
	// we retrieve pointers on real joysticks we are interested in
	m_pJoystick = this->registerRealJoystick(MCGP::Description);
	m_pThrottle = this->registerRealJoystick(TMWT::Description);
	m_pPedals = this->registerRealJoystick(SPFC::Description);
	
	if(m_pJoystick)
		LOG_MSG(MCGP::Description + " detected");
	else
		LOG_ERROR(MCGP::Description + " not detected!");
	
	if(m_pThrottle)
		LOG_MSG(TMWT::Description + " detected");
	else
		LOG_ERROR(TMWT::Description + " not detected!");

	if(m_pPedals)
		LOG_MSG(SPFC::Description + " detected");
	else
		LOG_ERROR(SPFC::Description + " not detected!");
	
	if (!m_pJoystick || !m_pThrottle || !m_pPedals)
		return false;
	
	// Virtual joysticks setup
	m_pVirtualJoy1 = new VirtualJoystick{1};
	LOG_MSG("Virtual joystick 1 configured");
	this->registerVirtualJoystick(m_pVirtualJoy1);
	
	m_pVirtualJoy2 = new VirtualJoystick{2};
	LOG_MSG("Virtual joystick 2 configured");
	this->registerVirtualJoystick(m_pVirtualJoy2);
	
	return true;
}

/*virtual*/ void Profile::runFirstStep() /*override final*/
{
	// Dealing with layers
	this->registerLayerDim1(Layers::In, m_pThrottle, TMWT::MSD);
	
	// Initialize the virtual joysticks data using the real joysticks data
	// be in sync with the initial mappings defined below
	m_pVirtualJoy1->resetReport();
	m_pVirtualJoy2->resetReport();
	m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeVertical,0.0f); // vertical strafe at 0 to avoid bad surprises
	
	// Leds initialisation
	m_iBrightness = bUSE_LED ? 1 : 0;
	
	m_pThrottle->setData("BRIGHTNESS",m_iBrightness);
	m_pThrottle->setData("BACKLIT",m_bBacklit);
	m_pThrottle->setData("LED4",false);
	m_pThrottle->setData("LED5",false);
	
	// Create the initial mapping
	// divers
	MapAxis(m_pPedals, SPFC::RUDDER, AllLayers, m_pVirtualJoy1, SC1::AxisFlightYaw);
	m_pPedals->setAxisTrim(SPFC::RUDDER,-0.0028f);
	m_pPedals->setSCurve(SPFC::RUDDER, 0.035f, 0.012f, 0.035f, 1.0f, 0.0f);
	m_pPedals->setSCurve(SPFC::BRK_LEFT, 0.04f, 0.00f, 0.06f, 0.0f, 0.0f);
	//m_pPedals->setSCurve(SPFC::BRK_RIGHT, fBRAKE_RIGHT_LDZ, 0.00f, fBRAKE_RIGHT_RDZ, 0.0f, 0.0f);
	
	//m_pJoystick->setAxisTrim(MCGP::JOYX,0.026f);
	//m_pJoystick->setAxisTrim(MCGP::JOYY,-0.006f);
	//m_pJoystick->setSCurve(MCGP::JOYX, 0.02f, 0.015f, 0.01f, 2.5f, 0.0f);
	//m_pJoystick->setSCurve(MCGP::JOYY, 0.01f, 0.015f, 0.015f, 2.5f, 0.0f);
	
	// CONTROL MODES AND LANDING
	// initialisation of control modes
	if (m_pThrottle->buttonPressed(TMWT::APALT))      {this->setControlsGround();}
	else if (m_pThrottle->buttonPressed(TMWT::APAH))  {this->setControlsFlightLanding();}
	else if (m_pThrottle->buttonPressed(TMWT::APATT)) {this->setControlsFlightCruise();}
	// set transitions between control modes
	Map(m_pThrottle, ControlType::Button, TMWT::APALT, AllLayers, new TriggerButtonPress{}, new ActionCallback([this](){this->setControlsGround();}));
	Map(m_pThrottle, ControlType::Button, TMWT::APAH,  AllLayers, new TriggerButtonPress{}, new ActionCallback([this](){this->setControlsFlightLanding();}));
	Map(m_pThrottle, ControlType::Button, TMWT::APATT, AllLayers, new TriggerButtonPress{}, new ActionCallback([this](){this->setControlsFlightCruise();}));
	// landing
	MapButton(m_pThrottle, TMWT::APENG, AllLayers, m_pVirtualJoy1, SC1::LandingSystemToggle);
	
	// throttle slider for power in relative (no axis provided for absolute power)
	m_pThrottle->setSCurve(TMWT::THR_FC, 0.02f, 0.02f, 0.02f, 0, 0);
	MapAxis2(m_pThrottle, TMWT::THR_FC, AllLayers, {-0.95f,0.95f},{
		new ActionButtonPress(m_pVirtualJoy1,SC1::IncreasePower),
		new ActionChain({new ActionButtonRelease{m_pVirtualJoy1,SC1::IncreasePower}, new ActionButtonRelease{m_pVirtualJoy1,SC1::DecreasePower}}),
		new ActionButtonPress(m_pVirtualJoy1,SC1::DecreasePower)
	});
	
	// brakes, boost, modes, radar, quantum drive
	MapButton(m_pThrottle, TMWT::MSL, AllLayers, m_pVirtualJoy1, SC1::CycleAmmoBack);
	MapButton(m_pThrottle, TMWT::MSR, AllLayers, m_pVirtualJoy1, SC1::CycleAmmo);
	MapButton(m_pThrottle, TMWT::MSU, AllLayers, m_pVirtualJoy1, SC1::CycleRadarRange);
	//MapButton(m_pThrottle, TMWT::MSD, AllLayers, ...); // deja utilise pour le shift
	MapButton(m_pThrottle, TMWT::SPDF, AllLayers, m_pVirtualJoy1, SC1::Afterburner);
	MapButton(m_pThrottle, TMWT::SPDB, AllLayers, m_pVirtualJoy1, SC1::Boost);
	MapButton(m_pThrottle, TMWT::BSB, AllLayers, m_pVirtualJoy1, SC1::SpaceBrake);
	auto callbackDecoupledTogglePulse = [this]()
	{
		if (!m_pThrottle->buttonPressed(TMWT::LDGH))
			DoAction(new ActionButtonPulse(m_pVirtualJoy1,SC1::DecoupledModeToggle,m_uiPULSE_AMT));
	};
	Map(m_pThrottle, ControlType::Button, TMWT::BSF, AllLayers, new TriggerButtonChange{}, new ActionCallback{callbackDecoupledTogglePulse});
	MapButton(m_pThrottle, TMWT::CHB, AllLayers, m_pVirtualJoy1, SC1::Autoland);
	MapButton(m_pThrottle, TMWT::CHF, AllLayers, m_pVirtualJoy1, SC1::QuantumDriveToggle);
	
	// APU button for yaw / roll swap
	auto swapYawRollToggle = [this]() {if (!m_pThrottle->buttonPressed(TMWT::LDGH)) DoAction(new ActionButtonPulse{m_pVirtualJoy1,SC1::SwapYawRollToggle,m_uiPULSE_AMT});};
	Map(m_pThrottle, ControlType::Button, TMWT::APUON, AllLayers, new TriggerButtonChange{}, new ActionCallback{swapYawRollToggle});
	
	//// weapons groups, missiles and quick targeting
	//MapButton(m_pJoystick, MCGP::TG1, AllLayers, m_pVirtualJoy1, SC1::FireGroup1);
	//MapButton(m_pJoystick, MCGP::TG2, AllLayers, m_pVirtualJoy1, SC1::FireGroup2); // to shoot 1 and 2 simultaneously with one finger only
	//MapButton(m_pJoystick, MCGP::S3,  AllLayers, m_pVirtualJoy1, SC1::FireGroup2);
	//
	//// IFCS safeties and look behind (HAT 1)
	//MapButton(m_pJoystick, MCGP::H1L, {"o"}, m_pVirtualJoy1, SC1::GForceSafetyToggle);
	//MapButton(m_pJoystick, MCGP::H1U, {"o"}, m_pVirtualJoy1, SC1::ComstabToggle);
	//MapButton(m_pJoystick, MCGP::H1R, {"o"}, m_pVirtualJoy1, SC1::ESPToggle);
	//MapButton(m_pJoystick, MCGP::H1D, AllLayers, m_pVirtualJoy1, SC1::LookBehind);
	//
	//// config led brightness and backlit
	//Map(m_pJoystick, ControlType::Button, MCGP::H1L, {"i"}, new TriggerButtonPress{}, new ActionCallback{[this](){this->ledBrightnessDown();}});
	//Map(m_pJoystick, ControlType::Button, MCGP::H1U, {"i"}, new TriggerButtonPress{}, new ActionCallback{[this](){this->toggleBacklit();}});
	//Map(m_pJoystick, ControlType::Button, MCGP::H1R, {"i"}, new TriggerButtonPress{}, new ActionCallback{[this](){this->ledBrightnessUp();}});
	//
	//// COUNTER-MEASURES (hat 2 L/R)
	//MapButton(m_pJoystick, MCGP::H2L, AllLayers, m_pVirtualJoy1, SC1::CycleCounterMeasures);
	//MapButton(m_pJoystick, MCGP::H2R, AllLayers, m_pVirtualJoy1, SC1::LaunchCounterMeasures);
	//
	//// STRAFE (HAT 3)
	//MapButton(m_pJoystick, MCGP::H3L, AllLayers, m_pVirtualJoy1, SC1::StrafeLeft);
	//MapButton(m_pJoystick, MCGP::H3R, AllLayers, m_pVirtualJoy1, SC1::StrafeRight);
	//
	//// TARGET SELECTION (hat 4)
	//Map(m_pJoystick, ControlType::Button, MCGP::H4L, {"i"}, new TriggerButtonPress{}, new ActionCallback{[this]() {this->browsePinnedTargets();}});
	//Map(m_pJoystick, ControlType::Button, MCGP::H4R, {"i"}, new TriggerButtonPress{}, new ActionCallback{[this]() {this->browseAllTargets();}});
	//Map(m_pJoystick, ControlType::Button, MCGP::H4U, {"i"}, new TriggerButtonPress{}, new ActionCallback{[this]() {this->browseHostileTargets();}});
	//Map(m_pJoystick, ControlType::Button, MCGP::H4D, {"i"}, new TriggerButtonPress{}, new ActionCallback{[this]() {this->browseFriendTargets();}});
	//Map(m_pJoystick, ControlType::Button, MCGP::H4L, {"o"}, new TriggerButtonPress{}, new ActionCallback{[this]() {this->previousTarget();}});
	//Map(m_pJoystick, ControlType::Button, MCGP::H4R, {"o"}, new TriggerButtonPress{}, new ActionCallback{[this]() {this->nextTarget();}});
	//MapButton(m_pJoystick, MCGP::H4U, {"o"}, m_pVirtualJoy1, SC1::PinFocusedTarget);
	//MapButton(m_pJoystick, MCGP::H4D, {"o"}, m_pVirtualJoy1, SC1::AcquireMissileLock);
	
	// POWER
	MapButton(m_pThrottle, TMWT::EOLIGN, AllLayers, m_pVirtualJoy2, SC2::FlightReady);
	Map(m_pThrottle, ControlType::Button, TMWT::EOLMOTOR, AllLayers, new TriggerButtonPress{}, new ActionButtonPulse{m_pVirtualJoy2,SC2::PowerOff,m_uiPULSE_AMT});
	// switchs on : off
	auto powerPreset1Toggle = [this]() {if (!m_pThrottle->buttonPressed(TMWT::LDGH)) DoAction(new ActionButtonPulse{m_pVirtualJoy1,SC1::PowerPreset1Toggle,m_uiPULSE_AMT});};
	auto powerPreset2Toggle = [this]() {if (!m_pThrottle->buttonPressed(TMWT::LDGH)) DoAction(new ActionButtonPulse{m_pVirtualJoy1,SC1::PowerPreset2Toggle,m_uiPULSE_AMT});};
	auto powerPreset3Toggle = [this]() {if (!m_pThrottle->buttonPressed(TMWT::LDGH)) DoAction(new ActionButtonPulse{m_pVirtualJoy1,SC1::PowerPreset3Toggle,m_uiPULSE_AMT});};
	Map(m_pThrottle, ControlType::Button, TMWT::EACON,   AllLayers, new TriggerButtonChange{}, new ActionCallback{powerPreset1Toggle});
	Map(m_pThrottle, ControlType::Button, TMWT::RDRNRM,  AllLayers, new TriggerButtonChange{}, new ActionCallback{powerPreset2Toggle});
	Map(m_pThrottle, ControlType::Button, TMWT::IDLELON, AllLayers, new TriggerButtonChange{}, new ActionCallback{powerPreset3Toggle});
	// power distribution
	MapButton(m_pThrottle, TMWT::CSL, AllLayers, m_pVirtualJoy1, SC1::PowerPreset1Increase);
	MapButton(m_pThrottle, TMWT::CSD, AllLayers, m_pVirtualJoy1, SC1::PowerPreset2Increase);
	MapButton(m_pThrottle, TMWT::CSR, AllLayers, m_pVirtualJoy1, SC1::PowerPreset3Increase);
	MapButton(m_pThrottle, TMWT::CSU, AllLayers, m_pVirtualJoy1, SC1::ResetPowerDistribution);
	
	// SHIELDS distribution (slew control)
	MapButtonTempo(m_pThrottle, TMWT::SC, AllLayers, ms2cycles(500),
		new ActionCallback{[this]() {this->switchShieldsMode();}},
		new ActionButtonPulse{m_pVirtualJoy2, SC2::ResetShieldsLevels, m_uiPULSE_AMT}
	);
	MapAxis2(m_pThrottle, TMWT::SCX, AllLayers, {-0.84f,0.84f},{
		new ActionButtonPress(m_pVirtualJoy2,SC2::ShieldRaiseLeft),
		new ActionChain({new ActionButtonRelease{m_pVirtualJoy2,SC2::ShieldRaiseLeft}, new ActionButtonRelease{m_pVirtualJoy2,SC2::ShieldRaiseRight}}),
		new ActionButtonPress(m_pVirtualJoy2,SC2::ShieldRaiseRight)
	});
	MapAxis2(m_pThrottle, TMWT::SCY, AllLayers, {-0.84f,0.84f},{
		new ActionCallback{[this]() {this->shieldsDownArrow();}},
		new ActionCallback{[this]() {this->releaseLongiShieldsButtons();}},
		new ActionCallback{[this]() {this->shieldsUpArrow();}}
	});
	
	// LIGHT AND TRANSPONDER
	auto headlightsToggle = [this]() {if (!m_pThrottle->buttonPressed(TMWT::LDGH)) DoAction(new ActionButtonPulse{m_pVirtualJoy1,SC1::HeadlightsToggle,m_uiPULSE_AMT});};
	auto pIdBroadcaToggle = [this]() {if (!m_pThrottle->buttonPressed(TMWT::LDGH)) DoAction(new ActionButtonPulse{m_pVirtualJoy1,SC1::PersonalIdBroadCastToggle,m_uiPULSE_AMT});};
	Map(m_pThrottle, ControlType::Button, TMWT::EFLNORM, AllLayers, new TriggerButtonChange{}, new ActionCallback{headlightsToggle});
	Map(m_pThrottle, ControlType::Button, TMWT::EFRNORM, AllLayers, new TriggerButtonChange{}, new ActionCallback{pIdBroadcaToggle});
	
	// track IR
	Map(m_pThrottle, ControlType::Button, TMWT::PSF, AllLayers, new TriggerButtonPress{}, new ActionKeyPulse{SCK::TrackIR_Center,0,m_uiPULSE_AMT});
	Map(m_pThrottle, ControlType::Button, TMWT::PSB, AllLayers, new TriggerButtonPress{}, new ActionKeyPulse{SCK::TrackIR_Pause,0,m_uiPULSE_AMT});
	
	// joystick trim
	//Map(m_pThrottle, ControlType::Button, TMWT::FLAPU, AllLayers, new TriggerButtonPress{}, new ActionCallback{[this]() {this->set_dxxy_trims();}});
	//Map(m_pThrottle, ControlType::Button, TMWT::FLAPD, AllLayers, new TriggerButtonPress{}, new ActionCallback{[this]() {this->reset_dxxy_trims();}});
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BROWSE HOSTILE TARGETS /////////////////////////////////////////////////////
void Profile::browseHostileTargets() { m_targetsTypeToCycle = 1; }

// BROWSE ALL TARGETS /////////////////////////////////////////////////////////
void Profile::browseAllTargets() { m_targetsTypeToCycle = 2; }

// BROWSE FRIEND TARGETS //////////////////////////////////////////////////////
void Profile::browseFriendTargets() { m_targetsTypeToCycle = 3; }

// BROWSE PINNED TARGETS //////////////////////////////////////////////////////
void Profile::browsePinnedTargets() { m_targetsTypeToCycle = 4; }


// PREVIOUS TARGET ////////////////////////////////////////////////////////////
void Profile::previousTarget()
{
	if (m_targetsTypeToCycle == 1) { DoAction(new ActionButtonPulse{ m_pVirtualJoy1,SC1::CycleHostileTargetsBack,m_uiPULSE_AMT }); }
	else if (m_targetsTypeToCycle == 2) { DoAction(new ActionButtonPulse{ m_pVirtualJoy1,SC1::CycleAllTargetsBack,m_uiPULSE_AMT }); }
	else if (m_targetsTypeToCycle == 3) { DoAction(new ActionButtonPulse{ m_pVirtualJoy1,SC1::CycleFriendlyTargetsBack,m_uiPULSE_AMT }); }
	else { DoAction(new ActionButtonPulse{ m_pVirtualJoy1,SC1::CyclePinnedTargetsBack,m_uiPULSE_AMT }); }
}

// NEXT TARGET ////////////////////////////////////////////////////////////////
void Profile::nextTarget()
{
	if (m_targetsTypeToCycle == 1) { DoAction(new ActionButtonPulse{ m_pVirtualJoy1,SC1::CycleHostileTargets,m_uiPULSE_AMT }); }
	else if (m_targetsTypeToCycle == 2) { DoAction(new ActionButtonPulse{ m_pVirtualJoy1,SC1::CycleAllTargets,m_uiPULSE_AMT }); }
	else if (m_targetsTypeToCycle == 3) { DoAction(new ActionButtonPulse{ m_pVirtualJoy1,SC1::CycleFriendlyTargets,m_uiPULSE_AMT }); }
	else { DoAction(new ActionButtonPulse{ m_pVirtualJoy1,SC1::CyclePinnedTargets,m_uiPULSE_AMT }); }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SWITCH SHIELDS MODES ///////////////////////////////////////////////////////
void Profile::switchShieldsMode()
{
	m_bShieldsHorizontalMode = !m_bShieldsHorizontalMode;
}

// SHIELDS DOWN ARROW /////////////////////////////////////////////////////////
void Profile::shieldsDownArrow()
{
	if (m_bShieldsHorizontalMode) { m_pVirtualJoy2->setButton(SC2::ShieldRaiseBack, true); }
	else { m_pVirtualJoy2->setButton(SC2::ShieldRaiseBottom, true); }
}

// SHIELDS UP ARROW ///////////////////////////////////////////////////////////
void Profile::shieldsUpArrow()
{
	if (m_bShieldsHorizontalMode) { m_pVirtualJoy2->setButton(SC2::ShieldRaiseFront, true); }
	else { m_pVirtualJoy2->setButton(SC2::ShieldRaiseTop, true); }
}

// RELEASE LONGI SHIELDS BUTTONS //////////////////////////////////////////////
void Profile::releaseLongiShieldsButtons()
{
	m_pVirtualJoy2->setButton(SC2::ShieldRaiseFront, false);
	m_pVirtualJoy2->setButton(SC2::ShieldRaiseBack, false);
	m_pVirtualJoy2->setButton(SC2::ShieldRaiseTop, false);
	m_pVirtualJoy2->setButton(SC2::ShieldRaiseBottom, false);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RESET DXXY TRIMS ///////////////////////////////////////////////////////////
void Profile::reset_dxxy_trims()
{
	//m_pJoystick->setAxisTrim(MCGP::JOYX,0.0f);
	//m_pJoystick->setAxisTrim(MCGP::JOYY,0.0f);
}

// SET DXXY TRIMS /////////////////////////////////////////////////////////////
void Profile::set_dxxy_trims()
{
	//m_pJoystick->setAxisTrim(MCGP::JOYX,-m_pJoystick->axisRawValue(MCGP::JOYX));
	//m_pJoystick->setAxisTrim(MCGP::JOYY,-m_pJoystick->axisRawValue(MCGP::JOYY));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SET CONTROLS GROUND ////////////////////////////////////////////////////////
void Profile::setControlsGround()
{
	if (bUSE_LED)
	{
		m_pThrottle->setData("LED1", true);
		m_pThrottle->setData("LED2", false);
		m_pThrottle->setData("LED3", false);
	}

	m_pVirtualJoy1->setAxis(SC1::AxisFlightRoll, 0.0f);
	m_pVirtualJoy1->setAxis(SC1::AxisFlightPitch, 0.0f);
	m_pVirtualJoy1->setAxis(SC1::AxisFlightThrust, 0.0f);
	m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeLat, 0.0f);
	m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeLongi, 0.0f);
	m_pVirtualJoy1->setAxis(SC1::AxisFlightYaw, 0.0f);
	m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeVertical, 0.0f);

	this->set_S4_for_horn();
	this->set_JOYXY_for_turn();
	this->set_BUTTONS_for_cruise();
	this->set_THR_and_pedals();
}

// SET CONTROLS FLIGHT LANDING ////////////////////////////////////////////////
void Profile::setControlsFlightLanding()
{
	if (bUSE_LED)
	{
		m_pThrottle->setData("LED1", false);
		m_pThrottle->setData("LED2", true);
		m_pThrottle->setData("LED3", false);
	}

	m_pVirtualJoy2->setAxis(SC2::AxisGroundLat, 0.0f);
	m_pVirtualJoy2->setAxis(SC2::AxisGroundThrust, 0.0f);
	m_pVirtualJoy2->setButton(SC2::Brake, false);

	this->set_S4_for_landing(); // to quickly switch JOYXY from roll&pitch to strafe and vice-versa
	this->set_JOYXY_for_landing();
	this->set_BUTTONS_for_landing();
	this->set_THR_for_landing();
}

// SET CONTROLS FLIGHT CRUISE /////////////////////////////////////////////////
void Profile::setControlsFlightCruise()
{
	if (bUSE_LED)
	{
		m_pThrottle->setData("LED1", false);
		m_pThrottle->setData("LED2", false);
		m_pThrottle->setData("LED3", true);
	}

	m_pVirtualJoy2->setAxis(SC2::AxisGroundLat, 0.0f);
	m_pVirtualJoy2->setAxis(SC2::AxisGroundThrust, 0.0f);
	m_pVirtualJoy2->setButton(SC2::Brake, false);
	m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeVertical, 0.0f);

	this->set_S4_for_nothing();
	this->set_JOYXY_for_rollNpitch();
	this->set_BUTTONS_for_cruise();
	this->set_THR_for_cruise();
}








// set_S4_for_horn
// set_S4_for_landing
// set_S4_for_nothing
void Profile::set_S4_for_horn()
{
	//UnmapButton(m_pJoystick, MCGP::S4);
	//MapButton(m_pJoystick, MCGP::S4, AllLayers, m_pVirtualJoy2, SC2::Horn);
}

void Profile::set_S4_for_landing()
{
	//UnmapButton(m_pJoystick, MCGP::S4);
	//Map(m_pJoystick, ControlType::Button, MCGP::S4, AllLayers, new TriggerButtonPress{}, new ActionCallback{[this]() {this->set_JOYXY_for_strafe();}});
	//Map(m_pJoystick, ControlType::Button, MCGP::S4, AllLayers, new TriggerButtonRelease{}, new ActionCallback{[this]() {this->set_JOYXY_for_rollNpitch();}});
}

void Profile::set_S4_for_nothing()
{
	//	UnmapButton(m_pJoystick, MCGP::S4);
}








// set_JOYXY_for_turn
// set_JOYXY_for_landing
// set_JOYXY_for_strafe
// set_JOYXY_for_rollNpitch
void Profile::set_JOYXY_for_turn()
{
	//UnmapAxis(m_pJoystick, MCGP::JOYX);
	//UnmapAxis(m_pJoystick, MCGP::JOYY);
	//
	//MapAxis(m_pJoystick, MCGP::JOYX, AllLayers, m_pVirtualJoy2, SC2::AxisGroundLat, AxisDirection::Normal);
	//m_pVirtualJoy2->setAxis(SC2::AxisGroundLat, m_pJoystick->axisValue(MCGP::JOYX));
}

void Profile::set_JOYXY_for_landing()
{
	//if (m_pJoystick->buttonPressed(MCGP::S4))
	//	this->set_JOYXY_for_strafe();
	//else
	//	this->set_JOYXY_for_rollNpitch();
}

void Profile::set_JOYXY_for_strafe()
{
	//UnmapAxis(m_pJoystick, MCGP::JOYX);
	//UnmapAxis(m_pJoystick, MCGP::JOYY);
	//
	//// set roll and pitch axes at 0, otherwise they keep their last values
	//m_pVirtualJoy1->setAxis(SC1::AxisFlightRoll, 0.0f);
	//m_pVirtualJoy1->setAxis(SC1::AxisFlightPitch, 0.0f);
	//
	//// joystick -> strafe
	//MapAxis(m_pJoystick, MCGP::JOYX, AllLayers, m_pVirtualJoy1, SC1::AxisFlightStrafeLat, AxisDirection::Normal);
	//MapAxis(m_pJoystick, MCGP::JOYY, AllLayers, m_pVirtualJoy1, SC1::AxisFlightStrafeLongi, AxisDirection::Reversed);
	//
	//// set strafe axes at correct value, otherwise it is necessary to wait for a movement of the stick
	//m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeLat, m_pJoystick->axisValue(MCGP::JOYX));
	//m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeLongi, -m_pJoystick->axisValue(MCGP::JOYY));
	//m_pVirtualJoy2->setButton(SC2::Brake,false);
}

void Profile::set_JOYXY_for_rollNpitch()
{
	//UnmapAxis(m_pJoystick, MCGP::JOYX);
	//UnmapAxis(m_pJoystick, MCGP::JOYY);

	// set strafe axes at 0, otherwise they keep their last values
	m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeLat, 0.0f);
	m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeLongi, 0.0f);

	//// joystick : roll and pitch
	//MapAxis(m_pJoystick, MCGP::JOYX, AllLayers, m_pVirtualJoy1, SC1::AxisFlightRoll, AxisDirection::Normal);
	//MapAxis(m_pJoystick, MCGP::JOYY, AllLayers, m_pVirtualJoy1, SC1::AxisFlightPitch, AxisDirection::Normal);

	// set roll and pitch axes at correct value, otherwise it is necessary to wait for a movement of the stick
	//m_pVirtualJoy1->setAxis(SC1::AxisFlightRoll, m_pJoystick->axisValue(MCGP::JOYX));
	//m_pVirtualJoy1->setAxis(SC1::AxisFlightPitch, m_pJoystick->axisValue(MCGP::JOYY));
	m_pVirtualJoy2->setButton(SC2::Brake, false);
}





// set_BUTTONS_for_landing
// set_BUTTONS_for_cruise
void Profile::set_BUTTONS_for_landing()
{
	//UnmapButton(m_pJoystick, MCGP::S1);
	//UnmapButton(m_pJoystick, MCGP::S2);
	//UnmapButton(m_pJoystick, MCGP::H2U);
	//UnmapButton(m_pJoystick, MCGP::H2D);
	//UnmapButton(m_pJoystick, MCGP::H3U);
	//UnmapButton(m_pJoystick, MCGP::H3D);
	//
	//MapButton(m_pJoystick, MCGP::S1, {"i"}, m_pVirtualJoy1, SC1::TargetNearestHostile);
	//MapButton(m_pJoystick, MCGP::S1, {"o"}, m_pVirtualJoy1, SC1::CycleAllTargets); // to cycle landing pads
	////MapButton(m_pJoystick, MCGP::S2, AllLayers, m_pVirtualJoy1, SC1::LandingTargetLockOnToggle); // to select a landing pad
	//
	//MapButton(m_pJoystick, MCGP::H2U, AllLayers, m_pVirtualJoy1, SC1::StrafeUp);
	//MapButton(m_pJoystick, MCGP::H2D, AllLayers, m_pVirtualJoy1, SC1::StrafeDown);
	//MapButton(m_pJoystick, MCGP::H3U, AllLayers, m_pVirtualJoy1, SC1::StrafeForward);
	//MapButton(m_pJoystick, MCGP::H3D, AllLayers, m_pVirtualJoy1, SC1::StrafeBackwards);
}

void Profile::set_BUTTONS_for_cruise()
{
	//UnmapButton(m_pJoystick, MCGP::S1);
	//UnmapButton(m_pJoystick, MCGP::S2);
	//UnmapButton(m_pJoystick, MCGP::H2U);
	//UnmapButton(m_pJoystick, MCGP::H2D);
	//UnmapButton(m_pJoystick, MCGP::H3U);
	//UnmapButton(m_pJoystick, MCGP::H3D);
	//
	//MapButton(m_pJoystick, MCGP::S1, {"i"}, m_pVirtualJoy1, SC1::TargetNearestHostile);
	//MapButton(m_pJoystick, MCGP::S1, {"o"}, m_pVirtualJoy1, SC1::CycleHostileTargets);
	//MapButton(m_pJoystick, MCGP::S2, AllLayers, m_pVirtualJoy1, SC1::LaunchMissile);
	//
	//MapButton(m_pJoystick, MCGP::H2U, AllLayers, m_pVirtualJoy1, SC1::StrafeForward);
	//MapButton(m_pJoystick, MCGP::H2D, AllLayers, m_pVirtualJoy1, SC1::StrafeBackwards);
	//MapButton(m_pJoystick, MCGP::H3U, AllLayers, m_pVirtualJoy1, SC1::StrafeUp);
	//MapButton(m_pJoystick, MCGP::H3D, AllLayers, m_pVirtualJoy1, SC1::StrafeDown);
}


// reset_THR_and_pedals
// set_THR_and_pedals
// set_THR_for_nothing
// set_THR_for_landing
// set_THR_for_cruise
void Profile::reset_THR_and_pedals()
{
	UnmapAxis(m_pPedals, SPFC::BRK_LEFT);
	UnmapAxis(m_pPedals, SPFC::BRK_RIGHT);
	this->set_THRLEFT_for_nothing();
	UnmapAxis(m_pThrottle, TMWT::THR_RIGHT);
	m_pThrottle->removeCurve(TMWT::THR_LEFT);
	m_pThrottle->removeCurve(TMWT::THR_RIGHT);
	UnmapButton(m_pThrottle, TMWT::LTB);
}

void Profile::set_THR_and_pedals()
{
	this->reset_THR_and_pedals();

	// mapping BRK_RIGHT function of THR_RIGHT position
	MapAxis2(m_pThrottle, TMWT::THR_RIGHT, AllLayers, 1, {
		new ActionCallback{ [this]() {this->set_BRKRIGHT_for_groundForward(); } },
		new ActionCallback{ [this]() {this->set_BRKRIGHT_for_groundBackward(); } }
		});
	if (m_pThrottle->axisValue(TMWT::THR_RIGHT) < 0)
		this->set_BRKRIGHT_for_groundForward();
	else
		this->set_BRKRIGHT_for_groundBackward();

	// mapping BRK_LEFT for brake
	Map(m_pPedals, ControlType::Axis, SPFC::BRK_LEFT, AllLayers, new TriggerAxisChangeComparison{true,fBRAKE_THRESHOLD}, new ActionButtonPress{m_pVirtualJoy2,SC2::Brake});
	Map(m_pPedals, ControlType::Axis, SPFC::BRK_LEFT, AllLayers, new TriggerAxisChangeComparison{false,fBRAKE_THRESHOLD}, new ActionButtonRelease{m_pVirtualJoy2,SC2::Brake});
	bool bBreak = m_pPedals->axisValue(SPFC::BRK_LEFT) > fBRAKE_THRESHOLD;
	m_pVirtualJoy2->setButton(SC2::Brake, bBreak);
}

void Profile::set_THR_for_landing()
{
	this->reset_THR_and_pedals();

	// mapping THR_RIGHT and THR_LEFT
	this->set_THRRIGHT_for_flightThrottle();
	if (m_pThrottle->buttonPressed(TMWT::LTB))
		this->set_THRLEFT_for_nothing();
	else
		this->set_THRLEFT_for_verticalStrafe();

	// mapping LTB (to switch the role of THR_LEFT)
	Map(m_pThrottle, ControlType::Button, TMWT::LTB, AllLayers, new TriggerButtonPress{}, new ActionCallback{ [this]() {this->set_THRLEFT_for_nothing(); } });
	Map(m_pThrottle, ControlType::Button, TMWT::LTB, AllLayers, new TriggerButtonRelease{}, new ActionCallback{ [this]() {this->set_THRLEFT_for_verticalStrafe(); } });
}

void Profile::set_THR_for_cruise()
{
	this->reset_THR_and_pedals();
	this->set_THRRIGHT_for_flightThrottle();
	this->set_THRLEFT_for_nothing();
}


// set_BRKRIGHT_for_groundForward
// set_BRKRIGHT_for_groundBackward
// set_THRLEFT_for_verticalStrafe
// set_THRLEFT_for_nothing
// set_THRRIGHT_for_flightThrottle
void Profile::set_BRKRIGHT_for_groundForward()
{
	MapAxis(m_pPedals, SPFC::BRK_RIGHT, AllLayers, m_pVirtualJoy2, SC2::AxisGroundThrust, AxisDirection::Normal);
	m_pPedals->setCustomCurve(SPFC::BRK_RIGHT, {-1.0f+2.0f*fBRAKE_RIGHT_LDZ,0.0f,  1.0f-2.0f*fBRAKE_RIGHT_RDZ,1.0f});
	m_pVirtualJoy2->setAxis(SC2::AxisGroundThrust, m_pPedals->axisValue(SPFC::BRK_RIGHT));
}

void Profile::set_BRKRIGHT_for_groundBackward()
{
	MapAxis(m_pPedals, SPFC::BRK_RIGHT, AllLayers, m_pVirtualJoy2, SC2::AxisGroundThrust, AxisDirection::Normal);
	m_pPedals->setCustomCurve(SPFC::BRK_RIGHT, {-1.0f+2.0f*fBRAKE_RIGHT_LDZ,0.0f,  1.0f-2.0f*fBRAKE_RIGHT_RDZ,-1.0f});
	m_pVirtualJoy2->setAxis(SC2::AxisGroundThrust, m_pPedals->axisValue(SPFC::BRK_RIGHT));
}

void Profile::set_THRLEFT_for_verticalStrafe()
{
	MapAxis(m_pThrottle, TMWT::THR_LEFT, AllLayers, m_pVirtualJoy1, SC1::AxisFlightStrafeVertical, AxisDirection::Reversed);
	float reductionFactor = 0.2f;
	m_pThrottle->setCustomCurve(TMWT::THR_LEFT, { -1.0f,-reductionFactor,  1.0f,reductionFactor });
	m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeVertical, 0.0f);
	m_pVirtualJoy1->setAxisTrim(SC1::AxisFlightStrafeVertical, m_pThrottle->axisValue(TMWT::THR_LEFT));
}

void Profile::set_THRLEFT_for_nothing()
{
	UnmapAxis(m_pThrottle, TMWT::THR_LEFT);
	m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeVertical, 0.0f); // set vertical strafe axis at 0, otherwise they keep their last values
}

void Profile::set_THRRIGHT_for_flightThrottle()
{
	MapAxis(m_pThrottle, TMWT::THR_RIGHT, AllLayers, m_pVirtualJoy1, SC1::AxisFlightThrust, AxisDirection::Normal); // THR_RIGHT mapped to thrust
	m_pVirtualJoy1->setAxis(SC1::AxisFlightThrust, m_pThrottle->axisValue(TMWT::THR_RIGHT)); // set axis at correct value, otherwise it is necessary to wait for a movement of the throttle
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LED BRIGHTNESS DOWN ////////////////////////////////////////////////////////
void Profile::ledBrightnessDown()
{
	if (m_iBrightness > 0)
	{
		m_iBrightness = lim<quint8>(m_iBrightness - 1, 0, 5);
		m_pThrottle->setData("BRIGHTNESS", m_iBrightness);
	}
}

// LED BRIGHTNESS UP //////////////////////////////////////////////////////////
void Profile::ledBrightnessUp()
{
	if (m_iBrightness < 5)
	{
		m_iBrightness = lim<quint8>(m_iBrightness + 1, 0, 5);
		m_pThrottle->setData("BRIGHTNESS", m_iBrightness);
	}
}

// TOGGLE BACKLIT /////////////////////////////////////////////////////////////
void Profile::toggleBacklit()
{
	m_bBacklit = !m_bBacklit;
	m_pThrottle->setData("BACKLIT", m_bBacklit);
}
