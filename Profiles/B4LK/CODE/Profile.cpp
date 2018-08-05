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
#include "VkbMcgProJoystick.h"
#include "ThrustmasterWarthogThrottle.h"
#include "SaitekProFlightCombatRudderPedals.h"
#include "LogitechG13.h"

#include "Lim.h"

LayersCombo AllLayers{};
using namespace Keys;

namespace JOY = VkbMcgProJoystick;
namespace THR = ThrustmasterWarthogThrottle;
namespace RUD = SaitekProFlightCombatRudderPedals;
namespace G13 = LogitechG13;

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
						m_pMcgPro(nullptr),				// VKB Gunfighter mkII MCG Pro
						m_pThrottle(nullptr),			// Thrustmaster Warthog Throttle
						m_pPedals(nullptr),				// Saitek Pro Flight Combat Rudder Pedals
						m_pG13(nullptr),
						m_pVJoy1(nullptr),
						m_pVJoy2(nullptr)
{
	m_bShieldsHorizontalMode = true;
	m_targetsTypeToCycle = 2;
	
	//m_bBacklit = false;
	//m_iBrightness = 1;
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
	
	m_pMcgPro = nullptr;
	m_pThrottle = nullptr;
	m_pPedals = nullptr;
	m_pG13 = nullptr;
	m_pVJoy1 = nullptr;
	m_pVJoy2 = nullptr;
}

/*virtual*/ bool Profile::setupJoysticks() /*override final*/
{
	// we retrieve pointers on real joysticks we are interested in
	m_pMcgPro = this->registerRealJoystick(JOY::Description);
	m_pThrottle = this->registerRealJoystick(THR::Description);
	m_pPedals = this->registerRealJoystick(RUD::Description);
	m_pG13 = this->registerRealJoystick(G13::Description);
	
	if(m_pMcgPro)
		LOG_MSG(JOY::Description + " detected");
	else
		LOG_ERROR(JOY::Description + " not detected!");
	
	if(m_pThrottle)
		LOG_MSG(THR::Description + " detected");
	else
		LOG_ERROR(THR::Description + " not detected!");

	if(m_pPedals)
		LOG_MSG(RUD::Description + " detected");
	else
		LOG_ERROR(RUD::Description + " not detected!");

	if(m_pG13)
		LOG_MSG(G13::Description + " detected");
	else
		LOG_ERROR(G13::Description + " not detected!");
	
	if (!m_pMcgPro || !m_pThrottle || !m_pPedals || !m_pG13)
		return false;
	
	// Virtual joystick setup
	m_pVJoy1 = new VirtualJoystick{1};
	LOG_MSG("Virtual joystick 1 configured");
	this->registerVirtualJoystick(m_pVJoy1);

	m_pVJoy2 = new VirtualJoystick{2};
	LOG_MSG("Virtual joystick 2 configured");
	this->registerVirtualJoystick(m_pVJoy2);
	
	return true;
}

/*virtual*/ void Profile::runFirstStep() /*override final*/
{
	// Initialize the virtual joysticks data using the real joysticks data be in sync with the initial mappings defined below
	m_pVJoy1->resetReport();
	m_pVJoy2->resetReport();
	
	// Fixup joysticks axis
	m_pMcgPro->setSCurve(JOY::JOYX, 0.0f, 0.0f, 0.0f, .8f, 0.0f);
	m_pMcgPro->setSCurve(JOY::JOYY, 0.0f, 0.0f, 0.0f, .8f, 0.0f);

	m_pG13->setSCurve(G13::JOYX, 0.0f, 0.05f, 0.0f, 0.0f, 0.3f);
	m_pG13->setSCurve(G13::JOYY, 0.0f, 0.105f, 0.0f, 0.0f, 0.6f);
	m_pG13->setAxisInverted(G13::JOYY, true);

	m_pPedals->setSCurve(RUD::RUDDER, 0.0f, 0.05f, 0.0f, 0.0f, 0.0f);
	m_pPedals->setAxisInverted(RUD::RUDDER, true);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Cockpit
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::Eject);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::ExitSeat);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::SelfDestruct);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::IncreaseCoolerRate);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::DecreaseCoolerRate);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::FlightSystemsReady);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::OpenAllDoors);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::CloseAllDoors);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::LockAllDoors);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::UnlockAllDoors);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Camera
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::CycleCameraView);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::CycleCameraOrbitMode);
	MapButton(m_pThrottle, THR::SPDB, AllLayers, m_pVJoy1, SC1::LookBehind);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Propulsion
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::Spacebrake);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::MatchTargetVelocity);
	MapButton(m_pThrottle, THR::LTB, AllLayers, m_pVJoy1, SC1::ToggleDecoupledMode);
	MapButton(m_pMcgPro, JOY::HatGripRight, AllLayers, m_pVJoy1, SC1::CycleIFCS);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::ToggleGForceSafety);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::ToggleCOMSTAB);
	MapButton(m_pMcgPro, JOY::HatGripPress, AllLayers, m_pVJoy1, SC1::ToggleESP);
	MapButton(m_pThrottle, THR::SPDF, AllLayers, m_pVJoy1, SC1::Afterburner);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::Boost);
	MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::ToggleLandingGear);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::Autoland);
	MapButton(m_pThrottle, THR::EOLIGN, AllLayers, m_pVJoy1, SC1::ToggleQuantumTravelSystem);
	MapButton(m_pThrottle, THR::EORIGN, AllLayers, m_pVJoy1, SC1::QuantumDrive);
	
	Map(m_pMcgPro, ControlType::Axis, JOY::JOYX, AllLayers, new TriggerAxisChange, new ActionCallback([this]() { DoStick(); }));
	Map(m_pMcgPro, ControlType::Axis, JOY::JOYY, AllLayers, new TriggerAxisChange, new ActionCallback([this]() { DoStick(); }));
	MapAxis(m_pPedals, RUD::RUDDER, AllLayers, m_pVJoy1, SC1::AxisFlightRoll);

	Map(m_pMcgPro, ControlType::Axis, JOY::BRAKE, AllLayers, new TriggerAxisChange, new ActionCallback([this]() { DoStrafe(); }));
	Map(m_pThrottle, ControlType::Axis, THR::THR_LEFT, AllLayers, new TriggerAxisChange, new ActionCallback([this]() { DoStrafe(); }));
	Map(m_pThrottle, ControlType::Button, THR::MSR, AllLayers, new TriggerButtonState(true), new ActionCallback([this]() { DoStrafe(); }));
	Map(m_pThrottle, ControlType::Button, THR::MSL, AllLayers, new TriggerButtonState(true), new ActionCallback([this]() { DoStrafe(); }));
	Map(m_pThrottle, ControlType::Button, THR::CSU, AllLayers, new TriggerButtonState(true), new ActionCallback([this]() { DoStrafe(); }));
	Map(m_pThrottle, ControlType::Button, THR::CSD, AllLayers, new TriggerButtonState(true), new ActionCallback([this]() { DoStrafe(); }));
	Map(m_pThrottle, ControlType::Button, THR::MSR, AllLayers, new TriggerButtonRelease, new ActionCallback([this]() { DoStrafe(); }));
	Map(m_pThrottle, ControlType::Button, THR::MSL, AllLayers, new TriggerButtonRelease, new ActionCallback([this]() { DoStrafe(); }));
	Map(m_pThrottle, ControlType::Button, THR::CSU, AllLayers, new TriggerButtonRelease, new ActionCallback([this]() { DoStrafe(); }));
	Map(m_pThrottle, ControlType::Button, THR::CSD, AllLayers, new TriggerButtonRelease, new ActionCallback([this]() { DoStrafe(); }));

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Targeting
	//MapButton(m_pMcgPro, JOY::BtnRed, AllLayers, m_pVJoy1, SC1::ResetAim);
	//MapButton(m_pMcgPro, JOY::PressSlew, AllLayers, m_pVJoy1, SC1::LookAhead);
	MapButton(m_pMcgPro, JOY::PressSlew, AllLayers, m_pVJoy1, SC1::GimbalLock);

	MapButtonTempo(m_pMcgPro, JOY::HatHeadPress, AllLayers, ms2cycles(500), new ActionButtonPulse(m_pVJoy1, SC1::ReticleFocus, m_uiPULSE_AMT), new ActionButtonPulse(m_pVJoy1, SC1::PinFocusedTarget, m_uiPULSE_AMT));
	MapButtonTempo(m_pMcgPro, JOY::HatHeadUp, AllLayers, ms2cycles(500), new ActionButtonPulse(m_pVJoy1, SC1::CycleAllTargets, m_uiPULSE_AMT), new ActionButtonPulse(m_pVJoy1, SC1::CycleAllTargetsBack, m_uiPULSE_AMT));
	MapButtonTempo(m_pMcgPro, JOY::HatHeadLeft, AllLayers, ms2cycles(500), new ActionButtonPulse(m_pVJoy1, SC1::CycleHostileTargetsBack, m_uiPULSE_AMT), new ActionButtonPulse(m_pVJoy1, SC1::CycleFriendlyTargetBack, m_uiPULSE_AMT));
	MapButtonTempo(m_pMcgPro, JOY::HatHeadRight, AllLayers, ms2cycles(500), new ActionButtonPulse(m_pVJoy1, SC1::CycleHostileTargets, m_uiPULSE_AMT), new ActionButtonPulse(m_pVJoy1, SC1::CycleFriendlyTarget, m_uiPULSE_AMT));
	MapButtonTempo(m_pMcgPro, JOY::HatHeadDown, AllLayers, ms2cycles(500), new ActionButtonPulse(m_pVJoy1, SC1::TargetNearestHostile, m_uiPULSE_AMT), new ActionButtonPulse(m_pVJoy1, SC1::CyclePinnedTargets, m_uiPULSE_AMT));

	//MapButton(m_pMcgPro, JOY::PressSlew, AllLayers, m_pVJoy1, SC1::ToggleReticleMode);
	MapButton(m_pMcgPro, JOY::PressTrim, AllLayers, m_pVJoy1, SC1::TargetFocus);

	//MapButton(m_pMcgPro, JOY::BtnRed, AllLayers, m_pVJoy1, SC1::ToggleMiningMode);
	//MapButton(m_pMcgPro, JOY::BtnRed, AllLayers, m_pVJoy1, SC1::ToggleScanningMode);
	//MapButton(m_pMcgPro, JOY::BtnRed, AllLayers, m_pVJoy1, SC1::ScanningRadarPing);
	//MapButton(m_pMcgPro, JOY::BtnRed, AllLayers, m_pVJoy1, SC1::ScanningIncreaseRadarAngle);
	//MapButton(m_pMcgPro, JOY::BtnRed, AllLayers, m_pVJoy1, SC1::ScanningDecreaseRadarAngle);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Gunnery
	MapAxis(m_pMcgPro, JOY::SLEWX, AllLayers, m_pVJoy2, SC2::AxisAimLeftRight);
	MapAxis(m_pMcgPro, JOY::SLEWY, AllLayers, m_pVJoy2, SC2::AxisAimUpDown);
	MapAxis(m_pMcgPro, JOY::TRIMY, AllLayers, m_pVJoy1, SC1::AxisFlightDynamicZoom);

	Map(m_pMcgPro, ControlType::Button, JOY::TriggerStage1, AllLayers, new TriggerButtonState(true), new ActionCallback([this]() { DoTriggerPull(Stage1); }));
	Map(m_pMcgPro, ControlType::Button, JOY::TriggerStage2, AllLayers, new TriggerButtonState(true), new ActionCallback([this]() { DoTriggerPull(Stage2); }));
	Map(m_pMcgPro, ControlType::Button, JOY::FoldingTriggerDetent, AllLayers, new TriggerButtonState(true), new ActionCallback([this]() { DoTriggerPull(Mushy); }));
	Map(m_pMcgPro, ControlType::Button, JOY::TriggerStage1, AllLayers, new TriggerButtonRelease, new ActionCallback([this]() { DoTriggerPull(Stage1); }));
	Map(m_pMcgPro, ControlType::Button, JOY::TriggerStage2, AllLayers, new TriggerButtonRelease, new ActionCallback([this]() { DoTriggerPull(Stage2); }));
	Map(m_pMcgPro, ControlType::Button, JOY::FoldingTriggerDetent, AllLayers, new TriggerButtonRelease, new ActionCallback([this]() { DoTriggerPull(Mushy); }));
	//MapButton(m_pMcgPro, JOY::BtnRed, AllLayers, m_pVJoy2, SC2::CycleWeaponAmmo);
	//MapButton(m_pMcgPro, JOY::BtnRed, AllLayers, m_pVJoy2, SC2::CycleWeaponAmmoBack);
	//MapButton(m_pMcgPro, JOY::BtnRed, AllLayers, m_pVJoy2, SC2::AcquireMissileLock);
	MapButton(m_pMcgPro, JOY::BtnRed, AllLayers, m_pVJoy2, SC2::LaunchMissile);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Defensive
	MapButton(m_pMcgPro, JOY::BtnMidRight, AllLayers, m_pVJoy2, SC2::LaunchCountermeasure);
	MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::CycleCountermeasureAmmo);
	//MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::CycleCountermeasureAmmoBack);
	MapButton(m_pMcgPro, JOY::HatMidUp, AllLayers, m_pVJoy2, SC2::ShieldRaiseFront);
	MapButton(m_pMcgPro, JOY::HatMidDown, AllLayers, m_pVJoy2, SC2::ShieldRaiseBack);
	MapButton(m_pMcgPro, JOY::HatMidLeft, AllLayers, m_pVJoy2, SC2::ShieldRaiseLeft);
	MapButton(m_pMcgPro, JOY::HatMidRight, AllLayers, m_pVJoy2, SC2::ShieldRaiseRight);
	//MapButton(m_pMcgPro, JOY::HatMidPress, AllLayers, m_pVJoy2, SC2::ShieldRaiseTop);
	//MapButton(m_pMcgPro, JOY::HatMidPress, AllLayers, m_pVJoy2, SC2::ShieldRaiseBottom);
	MapButton(m_pMcgPro, JOY::HatMidPress, AllLayers, m_pVJoy2, SC2::ShieldResetLevels);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Power
	//MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::PowerPreset1);
	//MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::PowerPreset2);
	//MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::PowerPreset3);
	//MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::ResetPowerDistribution);
	//MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::IncreasePower);
	//MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::DecreasePower);
	//MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::IncreasePowerMax);
	//MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::DecreasePowerMin);
	//MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::TogglePowerPreset1);
	//MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::TogglePowerPreset2);
	//MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::TogglePowerPreset3);
	//MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::TogglePower);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Radar
	//MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::TogglePersonalIdentificationBroadcastSystem);
	//MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::CycleRadarRange);
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// UI
	//MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::Scoreboard);
	//MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::Map);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Lights
	//MapButton(m_pMcgPro, JOY::BtnTopRight, AllLayers, m_pVJoy2, SC2::ToggleLights);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// On Foot
	MapAxis(m_pG13, G13::JOYX, AllLayers, m_pVJoy2, SC2::AxisOnFootLeftRight);
	MapAxis(m_pG13, G13::JOYY, AllLayers, m_pVJoy2, SC2::AxisOnFootFwdBck);


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Map(m_pPedals, ControlType::Axis, RUD::BRK_RIGHT, AllLayers, new TriggerAxisChange, new ActionCallback([this]() { DoToeBrake(); }));
	Map(m_pPedals, ControlType::Axis, RUD::BRK_LEFT, AllLayers, new TriggerAxisChange, new ActionCallback([this]() { DoToeBrake(); }));

	//MapAxis(m_pPedals, RUD::RUDDER, AllLayers, m_pVirtualJoy1, SC1::AxisFlightYaw);
	//m_pPedals->setAxisTrim(RUD::RUDDER,-0.0028f);
	//m_pPedals->setSCurve(RUD::RUDDER, 0.035f, 0.012f, 0.035f, 1.0f, 0.0f);
	//m_pPedals->setSCurve(RUD::BRK_LEFT, 0.04f, 0.00f, 0.06f, 0.0f, 0.0f);
	//m_pPedals->setSCurve(RUD::BRK_RIGHT, fBRAKE_RIGHT_LDZ, 0.00f, fBRAKE_RIGHT_RDZ, 0.0f, 0.0f);
}

void Profile::DoStick()
{
	m_pVJoy1->setAxis(SC1::AxisFlightYaw, m_pMcgPro->axisValue(JOY::JOYX));
	m_pVJoy1->setAxis(SC1::AxisFlightPitch, m_pMcgPro->axisValue(JOY::JOYY));
}

void Profile::DoStrafe()
{
	if(m_pMcgPro->axisValue(JOY::BRAKE) == -1.0f)
	{
		m_pVJoy1->setAxis(SC1::AxisFlightThrottle, m_pThrottle->axisValue(THR::THR_LEFT));
		m_pVJoy1->setAxis(SC1::AxisFlightStrafeFwdBck, 0.0f);
	}
	else
	{
		m_pVJoy1->setAxis(SC1::AxisFlightThrottle, 1.0f);
		float fReverseThrottleAmt = m_pMcgPro->axisValue(JOY::BRAKE);
		fReverseThrottleAmt = (fReverseThrottleAmt * -0.5f) + 0.5f; // Normalize [0.0 - 1.0]
		m_pVJoy1->setAxis(SC1::AxisFlightStrafeFwdBck, (1.0f - fReverseThrottleAmt) * -1.0f);
	}

	float fSliderValue = 1.0f;

	if(m_pThrottle->buttonPressed(THR::FLAPD))
	{
		fSliderValue = m_pThrottle->axisValue(THR::THR_FC);
		fSliderValue = (fSliderValue * -0.5f) + 0.5f; // Normalize [0.0 - 1.0]
	}

	// LEFT / RIGHT
	if(m_pThrottle->buttonPressed(THR::MSL))
		m_pVJoy1->setAxis(SC1::AxisFlightStrafeLeftRight, fSliderValue * -1.0f);
	else if(m_pThrottle->buttonPressed(THR::MSR))
		m_pVJoy1->setAxis(SC1::AxisFlightStrafeLeftRight, fSliderValue);
	else
		m_pVJoy1->setAxis(SC1::AxisFlightStrafeLeftRight, 0.0f);

	// UP / DOWN
	if(m_pThrottle->buttonPressed(THR::CSD))
		m_pVJoy1->setAxis(SC1::AxisFlightStrafeUpDown, fSliderValue * -1.0f);
	else if(m_pThrottle->buttonPressed(THR::CSU))
		m_pVJoy1->setAxis(SC1::AxisFlightStrafeUpDown, fSliderValue);
	else
		m_pVJoy1->setAxis(SC1::AxisFlightStrafeUpDown, 0.0f);
}

void Profile::DoToeBrake()
{
	float fRightVal = m_pPedals->axisValue(RUD::BRK_RIGHT);
	float fLeftVal = m_pPedals->axisValue(RUD::BRK_LEFT);

	if(m_pThrottle->buttonPressed(THR::PSF)) // On foot mode
	{
		m_pVJoy2->setButton(SC2::LeanLeft, fLeftVal >= 0.0f);
		m_pVJoy2->setButton(SC2::LeanRight, fRightVal >= 0.0f);
	}
	else if(m_pThrottle->buttonPressed(THR::PSB)) // Vehicle mode
	{
		m_pVJoy2->setButton(SC2::VehicleBrake, fLeftVal >= 0.0f);
		
		fRightVal = (fRightVal * -0.5f) + 0.5f; // Normailize [0.0 - 1.0]
		m_pVJoy2->setAxis(SC2::AxisVehicleFwdBck, fRightVal);
	}
	else // Flight mode
	{
		m_pVJoy1->setButton(SC1::Spacebrake, fLeftVal >= 0.0f);
		m_pVJoy1->setButton(SC1::Boost, fRightVal >= 0.0f);
	}
}

void Profile::DoTriggerPull(TriggerStage eStage)
{
	switch(eStage)
	{
	case Stage1:
		m_pVJoy1->setButton(SC1::FireWeaponGroup1, m_pMcgPro->buttonPressed(JOY::TriggerStage1));
		m_pVJoy1->setButton(SC1::FireWeaponGroup2, m_pMcgPro->buttonPressed(JOY::TriggerStage1));
		break;

	case Stage2:
		m_pVJoy1->setAxis(SC1::AxisFlightDynamicZoom, m_pMcgPro->buttonPressed(JOY::TriggerStage2) ? 0.75f : 0.0f);
		break;

	case Mushy:
		break;
	}
}
