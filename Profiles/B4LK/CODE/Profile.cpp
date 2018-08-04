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
	m_pVJoy1->setAxis(SC1::AxisFlightStrafeUpDown, 0.0f); // vertical strafe at 0 to avoid bad surprises

	m_pVJoy2->resetReport();
	
	// Leds initialisation
	m_iBrightness = bUSE_LED ? 1 : 0;
	m_pThrottle->setData("BRIGHTNESS",m_iBrightness);
	m_pThrottle->setData("BACKLIT",m_bBacklit);
	m_pThrottle->setData("LED4",false);
	m_pThrottle->setData("LED5",false);

	// Fixup joysticks axis
	m_pMcgPro->setAxisTrim(JOY::JOYY, -0.21f); // Heavy trim needed on MCG Pro DansGame
	//m_pMcgPro->setSCurve(JOY::JOYY, 0.0f, 0.0f, 0.0f, 0.0f, 0.2f);
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
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::CycleIFCS);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::ToggleGForceSafety);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::ToggleCOMSTAB);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::ToggleESP);
	MapButton(m_pThrottle, THR::SPDF, AllLayers, m_pVJoy1, SC1::Afterburner);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::Boost);
	MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::ToggleLandingGear);
	//MapButton(m_pThrottle, THR::LDGH, AllLayers, m_pVJoy1, SC1::Autoland);
	MapButton(m_pThrottle, THR::EOLIGN, AllLayers, m_pVJoy1, SC1::ToggleQuantumTravelSystem);
	MapButton(m_pThrottle, THR::EORIGN, AllLayers, m_pVJoy1, SC1::QuantumDrive);
	
	MapAxis(m_pMcgPro, JOY::JOYX, AllLayers, m_pVJoy1, SC1::AxisFlightYaw);
	MapAxis(m_pMcgPro, JOY::JOYY, AllLayers, m_pVJoy1, SC1::AxisFlightPitch);
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
	//MapButton(m_pMcgPro, JOY::BtnRed, AllLayers, m_pVJoy1, SC1::GimbalLock);

	MapButtonTempo(m_pMcgPro, JOY::HatHeadPress, AllLayers, ms2cycles(500), new ActionButtonPulse(m_pVJoy1, SC1::ReticleFocus, m_uiPULSE_AMT), new ActionButtonPulse(m_pVJoy1, SC1::PinFocusedTarget, m_uiPULSE_AMT));
	MapButtonTempo(m_pMcgPro, JOY::HatHeadUp, AllLayers, ms2cycles(500), new ActionButtonPulse(m_pVJoy1, SC1::CycleAllTargets, m_uiPULSE_AMT), new ActionButtonPulse(m_pVJoy1, SC1::CycleAllTargetsBack, m_uiPULSE_AMT));
	MapButtonTempo(m_pMcgPro, JOY::HatHeadLeft, AllLayers, ms2cycles(500), new ActionButtonPulse(m_pVJoy1, SC1::CycleHostileTargetsBack, m_uiPULSE_AMT), new ActionButtonPulse(m_pVJoy1, SC1::CycleFriendlyTargetBack, m_uiPULSE_AMT));
	MapButtonTempo(m_pMcgPro, JOY::HatHeadRight, AllLayers, ms2cycles(500), new ActionButtonPulse(m_pVJoy1, SC1::CycleHostileTargets, m_uiPULSE_AMT), new ActionButtonPulse(m_pVJoy1, SC1::CycleFriendlyTarget, m_uiPULSE_AMT));
	MapButtonTempo(m_pMcgPro, JOY::HatHeadDown, AllLayers, ms2cycles(500), new ActionButtonPulse(m_pVJoy1, SC1::TargetNearestHostile, m_uiPULSE_AMT), new ActionButtonPulse(m_pVJoy1, SC1::CyclePinnedTargets, m_uiPULSE_AMT));

	MapButton(m_pMcgPro, JOY::PressSlew, AllLayers, m_pVJoy1, SC1::ToggleReticleMode);
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
	
	//m_pMcgPro->setAxisTrim(JOY::JOYX,0.026f);
	//m_pMcgPro->setAxisTrim(JOY::JOYY,-0.006f);
	//m_pMcgPro->setSCurve(JOY::JOYX, 0.02f, 0.015f, 0.01f, 2.5f, 0.0f);
	//m_pMcgPro->setSCurve(JOY::JOYY, 0.01f, 0.015f, 0.015f, 2.5f, 0.0f);
	
	//// CONTROL MODES AND LANDING
	//// initialisation of control modes
	//if (m_pThrottle->buttonPressed(THR::APALT))      {this->setControlsGround();}
	//else if (m_pThrottle->buttonPressed(THR::APAH))  {this->setControlsFlightLanding();}
	//else if (m_pThrottle->buttonPressed(THR::APATT)) {this->setControlsFlightCruise();}
	//// set transitions between control modes
	//Map(m_pThrottle, ControlType::Button, THR::APALT, AllLayers, new TriggerButtonPress{}, new ActionCallback([this](){this->setControlsGround();}));
	//Map(m_pThrottle, ControlType::Button, THR::APAH,  AllLayers, new TriggerButtonPress{}, new ActionCallback([this](){this->setControlsFlightLanding();}));
	//Map(m_pThrottle, ControlType::Button, THR::APATT, AllLayers, new TriggerButtonPress{}, new ActionCallback([this](){this->setControlsFlightCruise();}));
	//// landing
	//MapButton(m_pThrottle, THR::APENG, AllLayers, m_pVirtualJoy1, SC1::LandingSystemToggle);
	//
	//// throttle slider for power in relative (no axis provided for absolute power)
	//m_pThrottle->setSCurve(THR::THR_FC, 0.02f, 0.02f, 0.02f, 0, 0);
	//MapAxis2(m_pThrottle, THR::THR_FC, AllLayers, {-0.95f,0.95f},{
	//	new ActionButtonPress(m_pVirtualJoy1,SC1::IncreasePower),
	//	new ActionChain({new ActionButtonRelease{m_pVirtualJoy1,SC1::IncreasePower}, new ActionButtonRelease{m_pVirtualJoy1,SC1::DecreasePower}}),
	//	new ActionButtonPress(m_pVirtualJoy1,SC1::DecreasePower)
	//});
	//
	//// brakes, boost, modes, radar, quantum drive
	//MapButton(m_pThrottle, THR::MSL, AllLayers, m_pVirtualJoy1, SC1::CycleAmmoBack);
	//MapButton(m_pThrottle, THR::MSR, AllLayers, m_pVirtualJoy1, SC1::CycleAmmo);
	//MapButton(m_pThrottle, THR::MSU, AllLayers, m_pVirtualJoy1, SC1::CycleRadarRange);
	////MapButton(m_pThrottle, THR::MSD, AllLayers, ...); // deja utilise pour le shift
	//MapButton(m_pThrottle, THR::SPDF, AllLayers, m_pVirtualJoy1, SC1::Afterburner);
	//MapButton(m_pThrottle, THR::SPDB, AllLayers, m_pVirtualJoy1, SC1::Boost);
	//MapButton(m_pThrottle, THR::BSB, AllLayers, m_pVirtualJoy1, SC1::SpaceBrake);
	//auto callbackDecoupledTogglePulse = [this]()
	//{
	//	if (!m_pThrottle->buttonPressed(THR::LDGH))
	//		DoAction(new ActionButtonPulse(m_pVirtualJoy1,SC1::DecoupledModeToggle,m_uiPULSE_AMT));
	//};
	//Map(m_pThrottle, ControlType::Button, THR::BSF, AllLayers, new TriggerButtonChange{}, new ActionCallback{callbackDecoupledTogglePulse});
	//MapButton(m_pThrottle, THR::CHB, AllLayers, m_pVirtualJoy1, SC1::Autoland);
	//MapButton(m_pThrottle, THR::CHF, AllLayers, m_pVirtualJoy1, SC1::QuantumDriveToggle);
	//
	//// APU button for yaw / roll swap
	//auto swapYawRollToggle = [this]() {if (!m_pThrottle->buttonPressed(THR::LDGH)) DoAction(new ActionButtonPulse{m_pVirtualJoy1,SC1::SwapYawRollToggle,m_uiPULSE_AMT});};
	//Map(m_pThrottle, ControlType::Button, THR::APUON, AllLayers, new TriggerButtonChange{}, new ActionCallback{swapYawRollToggle});
	
	//// weapons groups, missiles and quick targeting
	//MapButton(m_pMcgPro, JOY::TG1, AllLayers, m_pVirtualJoy1, SC1::FireGroup1);
	//MapButton(m_pMcgPro, JOY::TG2, AllLayers, m_pVirtualJoy1, SC1::FireGroup2); // to shoot 1 and 2 simultaneously with one finger only
	//MapButton(m_pMcgPro, JOY::S3,  AllLayers, m_pVirtualJoy1, SC1::FireGroup2);
	//
	//// IFCS safeties and look behind (HAT 1)
	//MapButton(m_pMcgPro, JOY::H1L, {"o"}, m_pVirtualJoy1, SC1::GForceSafetyToggle);
	//MapButton(m_pMcgPro, JOY::H1U, {"o"}, m_pVirtualJoy1, SC1::ComstabToggle);
	//MapButton(m_pMcgPro, JOY::H1R, {"o"}, m_pVirtualJoy1, SC1::ESPToggle);
	//MapButton(m_pMcgPro, JOY::H1D, AllLayers, m_pVirtualJoy1, SC1::LookBehind);
	//
	//// config led brightness and backlit
	//Map(m_pMcgPro, ControlType::Button, JOY::H1L, {"i"}, new TriggerButtonPress{}, new ActionCallback{[this](){this->ledBrightnessDown();}});
	//Map(m_pMcgPro, ControlType::Button, JOY::H1U, {"i"}, new TriggerButtonPress{}, new ActionCallback{[this](){this->toggleBacklit();}});
	//Map(m_pMcgPro, ControlType::Button, JOY::H1R, {"i"}, new TriggerButtonPress{}, new ActionCallback{[this](){this->ledBrightnessUp();}});
	//
	//// COUNTER-MEASURES (hat 2 L/R)
	//MapButton(m_pMcgPro, JOY::H2L, AllLayers, m_pVirtualJoy1, SC1::CycleCounterMeasures);
	//MapButton(m_pMcgPro, JOY::H2R, AllLayers, m_pVirtualJoy1, SC1::LaunchCounterMeasures);
	//
	//// STRAFE (HAT 3)
	//MapButton(m_pMcgPro, JOY::H3L, AllLayers, m_pVirtualJoy1, SC1::StrafeLeft);
	//MapButton(m_pMcgPro, JOY::H3R, AllLayers, m_pVirtualJoy1, SC1::StrafeRight);
	//
	//// TARGET SELECTION (hat 4)
	//Map(m_pMcgPro, ControlType::Button, JOY::H4L, {"i"}, new TriggerButtonPress{}, new ActionCallback{[this]() {this->browsePinnedTargets();}});
	//Map(m_pMcgPro, ControlType::Button, JOY::H4R, {"i"}, new TriggerButtonPress{}, new ActionCallback{[this]() {this->browseAllTargets();}});
	//Map(m_pMcgPro, ControlType::Button, JOY::H4U, {"i"}, new TriggerButtonPress{}, new ActionCallback{[this]() {this->browseHostileTargets();}});
	//Map(m_pMcgPro, ControlType::Button, JOY::H4D, {"i"}, new TriggerButtonPress{}, new ActionCallback{[this]() {this->browseFriendTargets();}});
	//Map(m_pMcgPro, ControlType::Button, JOY::H4L, {"o"}, new TriggerButtonPress{}, new ActionCallback{[this]() {this->previousTarget();}});
	//Map(m_pMcgPro, ControlType::Button, JOY::H4R, {"o"}, new TriggerButtonPress{}, new ActionCallback{[this]() {this->nextTarget();}});
	//MapButton(m_pMcgPro, JOY::H4U, {"o"}, m_pVirtualJoy1, SC1::PinFocusedTarget);
	//MapButton(m_pMcgPro, JOY::H4D, {"o"}, m_pVirtualJoy1, SC1::AcquireMissileLock);
	
	//// POWER
	//MapButton(m_pThrottle, THR::EOLIGN, AllLayers, m_pVirtualJoy2, SC2::FlightReady);
	//Map(m_pThrottle, ControlType::Button, THR::EOLMOTOR, AllLayers, new TriggerButtonPress{}, new ActionButtonPulse{m_pVirtualJoy2,SC2::PowerOff,m_uiPULSE_AMT});
	//// switchs on : off
	//auto powerPreset1Toggle = [this]() {if (!m_pThrottle->buttonPressed(THR::LDGH)) DoAction(new ActionButtonPulse{m_pVirtualJoy1,SC1::PowerPreset1Toggle,m_uiPULSE_AMT});};
	//auto powerPreset2Toggle = [this]() {if (!m_pThrottle->buttonPressed(THR::LDGH)) DoAction(new ActionButtonPulse{m_pVirtualJoy1,SC1::PowerPreset2Toggle,m_uiPULSE_AMT});};
	//auto powerPreset3Toggle = [this]() {if (!m_pThrottle->buttonPressed(THR::LDGH)) DoAction(new ActionButtonPulse{m_pVirtualJoy1,SC1::PowerPreset3Toggle,m_uiPULSE_AMT});};
	//Map(m_pThrottle, ControlType::Button, THR::EACON,   AllLayers, new TriggerButtonChange{}, new ActionCallback{powerPreset1Toggle});
	//Map(m_pThrottle, ControlType::Button, THR::RDRNRM,  AllLayers, new TriggerButtonChange{}, new ActionCallback{powerPreset2Toggle});
	//Map(m_pThrottle, ControlType::Button, THR::IDLELON, AllLayers, new TriggerButtonChange{}, new ActionCallback{powerPreset3Toggle});
	//// power distribution
	//MapButton(m_pThrottle, THR::CSL, AllLayers, m_pVirtualJoy1, SC1::PowerPreset1Increase);
	//MapButton(m_pThrottle, THR::CSD, AllLayers, m_pVirtualJoy1, SC1::PowerPreset2Increase);
	//MapButton(m_pThrottle, THR::CSR, AllLayers, m_pVirtualJoy1, SC1::PowerPreset3Increase);
	//MapButton(m_pThrottle, THR::CSU, AllLayers, m_pVirtualJoy1, SC1::ResetPowerDistribution);
	//
	//// SHIELDS distribution (slew control)
	//MapButtonTempo(m_pThrottle, THR::SC, AllLayers, ms2cycles(500),
	//	new ActionCallback{[this]() {this->switchShieldsMode();}},
	//	new ActionButtonPulse{m_pVirtualJoy2, SC2::ResetShieldsLevels, m_uiPULSE_AMT}
	//);
	//MapAxis2(m_pThrottle, THR::SCX, AllLayers, {-0.84f,0.84f},{
	//	new ActionButtonPress(m_pVirtualJoy2,SC2::ShieldRaiseLeft),
	//	new ActionChain({new ActionButtonRelease{m_pVirtualJoy2,SC2::ShieldRaiseLeft}, new ActionButtonRelease{m_pVirtualJoy2,SC2::ShieldRaiseRight}}),
	//	new ActionButtonPress(m_pVirtualJoy2,SC2::ShieldRaiseRight)
	//});
	//MapAxis2(m_pThrottle, THR::SCY, AllLayers, {-0.84f,0.84f},{
	//	new ActionCallback{[this]() {this->shieldsDownArrow();}},
	//	new ActionCallback{[this]() {this->releaseLongiShieldsButtons();}},
	//	new ActionCallback{[this]() {this->shieldsUpArrow();}}
	//});
	//
	//// LIGHT AND TRANSPONDER
	//auto headlightsToggle = [this]() {if (!m_pThrottle->buttonPressed(THR::LDGH)) DoAction(new ActionButtonPulse{m_pVirtualJoy1,SC1::HeadlightsToggle,m_uiPULSE_AMT});};
	//auto pIdBroadcaToggle = [this]() {if (!m_pThrottle->buttonPressed(THR::LDGH)) DoAction(new ActionButtonPulse{m_pVirtualJoy1,SC1::PersonalIdBroadCastToggle,m_uiPULSE_AMT});};
	//Map(m_pThrottle, ControlType::Button, THR::EFLNORM, AllLayers, new TriggerButtonChange{}, new ActionCallback{headlightsToggle});
	//Map(m_pThrottle, ControlType::Button, THR::EFRNORM, AllLayers, new TriggerButtonChange{}, new ActionCallback{pIdBroadcaToggle});
	//
	//// track IR
	//Map(m_pThrottle, ControlType::Button, THR::PSF, AllLayers, new TriggerButtonPress{}, new ActionKeyPulse{SCK::TrackIR_Center,0,m_uiPULSE_AMT});
	//Map(m_pThrottle, ControlType::Button, THR::PSB, AllLayers, new TriggerButtonPress{}, new ActionKeyPulse{SCK::TrackIR_Pause,0,m_uiPULSE_AMT});
	
	// joystick trim
	//Map(m_pThrottle, ControlType::Button, THR::FLAPU, AllLayers, new TriggerButtonPress{}, new ActionCallback{[this]() {this->set_dxxy_trims();}});
	//Map(m_pThrottle, ControlType::Button, THR::FLAPD, AllLayers, new TriggerButtonPress{}, new ActionCallback{[this]() {this->reset_dxxy_trims();}});
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// BROWSE HOSTILE TARGETS /////////////////////////////////////////////////////
//void Profile::browseHostileTargets() { m_targetsTypeToCycle = 1; }
//
//// BROWSE ALL TARGETS /////////////////////////////////////////////////////////
//void Profile::browseAllTargets() { m_targetsTypeToCycle = 2; }
//
//// BROWSE FRIEND TARGETS //////////////////////////////////////////////////////
//void Profile::browseFriendTargets() { m_targetsTypeToCycle = 3; }
//
//// BROWSE PINNED TARGETS //////////////////////////////////////////////////////
//void Profile::browsePinnedTargets() { m_targetsTypeToCycle = 4; }
//
//
//// PREVIOUS TARGET ////////////////////////////////////////////////////////////
//void Profile::previousTarget()
//{
//	if (m_targetsTypeToCycle == 1) { DoAction(new ActionButtonPulse{ m_pVirtualJoy1,SC1::CycleHostileTargetsBack,m_uiPULSE_AMT }); }
//	else if (m_targetsTypeToCycle == 2) { DoAction(new ActionButtonPulse{ m_pVirtualJoy1,SC1::CycleAllTargetsBack,m_uiPULSE_AMT }); }
//	else if (m_targetsTypeToCycle == 3) { DoAction(new ActionButtonPulse{ m_pVirtualJoy1,SC1::CycleFriendlyTargetsBack,m_uiPULSE_AMT }); }
//	else { DoAction(new ActionButtonPulse{ m_pVirtualJoy1,SC1::CyclePinnedTargetsBack,m_uiPULSE_AMT }); }
//}
//
//// NEXT TARGET ////////////////////////////////////////////////////////////////
//void Profile::nextTarget()
//{
//	if (m_targetsTypeToCycle == 1) { DoAction(new ActionButtonPulse{ m_pVirtualJoy1,SC1::CycleHostileTargets,m_uiPULSE_AMT }); }
//	else if (m_targetsTypeToCycle == 2) { DoAction(new ActionButtonPulse{ m_pVirtualJoy1,SC1::CycleAllTargets,m_uiPULSE_AMT }); }
//	else if (m_targetsTypeToCycle == 3) { DoAction(new ActionButtonPulse{ m_pVirtualJoy1,SC1::CycleFriendlyTargets,m_uiPULSE_AMT }); }
//	else { DoAction(new ActionButtonPulse{ m_pVirtualJoy1,SC1::CyclePinnedTargets,m_uiPULSE_AMT }); }
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// SWITCH SHIELDS MODES ///////////////////////////////////////////////////////
//void Profile::switchShieldsMode()
//{
//	m_bShieldsHorizontalMode = !m_bShieldsHorizontalMode;
//}
//
//// SHIELDS DOWN ARROW /////////////////////////////////////////////////////////
//void Profile::shieldsDownArrow()
//{
//	if (m_bShieldsHorizontalMode) { m_pVirtualJoy2->setButton(SC2::ShieldRaiseBack, true); }
//	else { m_pVirtualJoy2->setButton(SC2::ShieldRaiseBottom, true); }
//}
//
//// SHIELDS UP ARROW ///////////////////////////////////////////////////////////
//void Profile::shieldsUpArrow()
//{
//	if (m_bShieldsHorizontalMode) { m_pVirtualJoy2->setButton(SC2::ShieldRaiseFront, true); }
//	else { m_pVirtualJoy2->setButton(SC2::ShieldRaiseTop, true); }
//}
//
//// RELEASE LONGI SHIELDS BUTTONS //////////////////////////////////////////////
//void Profile::releaseLongiShieldsButtons()
//{
//	m_pVirtualJoy2->setButton(SC2::ShieldRaiseFront, false);
//	m_pVirtualJoy2->setButton(SC2::ShieldRaiseBack, false);
//	m_pVirtualJoy2->setButton(SC2::ShieldRaiseTop, false);
//	m_pVirtualJoy2->setButton(SC2::ShieldRaiseBottom, false);
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// RESET DXXY TRIMS ///////////////////////////////////////////////////////////
//void Profile::reset_dxxy_trims()
//{
//	//m_pMcgPro->setAxisTrim(JOY::JOYX,0.0f);
//	//m_pMcgPro->setAxisTrim(JOY::JOYY,0.0f);
//}
//
//// SET DXXY TRIMS /////////////////////////////////////////////////////////////
//void Profile::set_dxxy_trims()
//{
//	//m_pMcgPro->setAxisTrim(JOY::JOYX,-m_pMcgPro->axisRawValue(JOY::JOYX));
//	//m_pMcgPro->setAxisTrim(JOY::JOYY,-m_pMcgPro->axisRawValue(JOY::JOYY));
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// SET CONTROLS GROUND ////////////////////////////////////////////////////////
//void Profile::setControlsGround()
//{
//	if (bUSE_LED)
//	{
//		m_pThrottle->setData("LED1", true);
//		m_pThrottle->setData("LED2", false);
//		m_pThrottle->setData("LED3", false);
//	}
//
//	m_pVirtualJoy1->setAxis(SC1::AxisFlightRoll, 0.0f);
//	m_pVirtualJoy1->setAxis(SC1::AxisFlightPitch, 0.0f);
//	m_pVirtualJoy1->setAxis(SC1::AxisFlightThrust, 0.0f);
//	m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeLat, 0.0f);
//	m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeLongi, 0.0f);
//	m_pVirtualJoy1->setAxis(SC1::AxisFlightYaw, 0.0f);
//	m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeVertical, 0.0f);
//
//	this->set_S4_for_horn();
//	this->set_JOYXY_for_turn();
//	this->set_BUTTONS_for_cruise();
//	this->set_THR_and_pedals();
//}
//
//// SET CONTROLS FLIGHT LANDING ////////////////////////////////////////////////
//void Profile::setControlsFlightLanding()
//{
//	if (bUSE_LED)
//	{
//		m_pThrottle->setData("LED1", false);
//		m_pThrottle->setData("LED2", true);
//		m_pThrottle->setData("LED3", false);
//	}
//
//	m_pVirtualJoy2->setAxis(SC2::AxisGroundLat, 0.0f);
//	m_pVirtualJoy2->setAxis(SC2::AxisGroundThrust, 0.0f);
//	m_pVirtualJoy2->setButton(SC2::Brake, false);
//
//	this->set_S4_for_landing(); // to quickly switch JOYXY from roll&pitch to strafe and vice-versa
//	this->set_JOYXY_for_landing();
//	this->set_BUTTONS_for_landing();
//	this->set_THR_for_landing();
//}
//
//// SET CONTROLS FLIGHT CRUISE /////////////////////////////////////////////////
//void Profile::setControlsFlightCruise()
//{
//	if (bUSE_LED)
//	{
//		m_pThrottle->setData("LED1", false);
//		m_pThrottle->setData("LED2", false);
//		m_pThrottle->setData("LED3", true);
//	}
//
//	m_pVirtualJoy2->setAxis(SC2::AxisGroundLat, 0.0f);
//	m_pVirtualJoy2->setAxis(SC2::AxisGroundThrust, 0.0f);
//	m_pVirtualJoy2->setButton(SC2::Brake, false);
//	m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeVertical, 0.0f);
//
//	this->set_S4_for_nothing();
//	this->set_JOYXY_for_rollNpitch();
//	this->set_BUTTONS_for_cruise();
//	this->set_THR_for_cruise();
//}
//
//
//
//
//
//
//
//
//// set_S4_for_horn
//// set_S4_for_landing
//// set_S4_for_nothing
//void Profile::set_S4_for_horn()
//{
//	//UnmapButton(m_pMcgPro, JOY::S4);
//	//MapButton(m_pMcgPro, JOY::S4, AllLayers, m_pVirtualJoy2, SC2::Horn);
//}
//
//void Profile::set_S4_for_landing()
//{
//	//UnmapButton(m_pMcgPro, JOY::S4);
//	//Map(m_pMcgPro, ControlType::Button, JOY::S4, AllLayers, new TriggerButtonPress{}, new ActionCallback{[this]() {this->set_JOYXY_for_strafe();}});
//	//Map(m_pMcgPro, ControlType::Button, JOY::S4, AllLayers, new TriggerButtonRelease{}, new ActionCallback{[this]() {this->set_JOYXY_for_rollNpitch();}});
//}
//
//void Profile::set_S4_for_nothing()
//{
//	//	UnmapButton(m_pMcgPro, JOY::S4);
//}
//
//
//
//
//
//
//
//
//// set_JOYXY_for_turn
//// set_JOYXY_for_landing
//// set_JOYXY_for_strafe
//// set_JOYXY_for_rollNpitch
//void Profile::set_JOYXY_for_turn()
//{
//	//UnmapAxis(m_pMcgPro, JOY::JOYX);
//	//UnmapAxis(m_pMcgPro, JOY::JOYY);
//	//
//	//MapAxis(m_pMcgPro, JOY::JOYX, AllLayers, m_pVirtualJoy2, SC2::AxisGroundLat, AxisDirection::Normal);
//	//m_pVirtualJoy2->setAxis(SC2::AxisGroundLat, m_pMcgPro->axisValue(JOY::JOYX));
//}
//
//void Profile::set_JOYXY_for_landing()
//{
//	//if (m_pMcgPro->buttonPressed(JOY::S4))
//	//	this->set_JOYXY_for_strafe();
//	//else
//	//	this->set_JOYXY_for_rollNpitch();
//}
//
//void Profile::set_JOYXY_for_strafe()
//{
//	//UnmapAxis(m_pMcgPro, JOY::JOYX);
//	//UnmapAxis(m_pMcgPro, JOY::JOYY);
//	//
//	//// set roll and pitch axes at 0, otherwise they keep their last values
//	//m_pVirtualJoy1->setAxis(SC1::AxisFlightRoll, 0.0f);
//	//m_pVirtualJoy1->setAxis(SC1::AxisFlightPitch, 0.0f);
//	//
//	//// joystick -> strafe
//	//MapAxis(m_pMcgPro, JOY::JOYX, AllLayers, m_pVirtualJoy1, SC1::AxisFlightStrafeLat, AxisDirection::Normal);
//	//MapAxis(m_pMcgPro, JOY::JOYY, AllLayers, m_pVirtualJoy1, SC1::AxisFlightStrafeLongi, AxisDirection::Reversed);
//	//
//	//// set strafe axes at correct value, otherwise it is necessary to wait for a movement of the stick
//	//m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeLat, m_pMcgPro->axisValue(JOY::JOYX));
//	//m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeLongi, -m_pMcgPro->axisValue(JOY::JOYY));
//	//m_pVirtualJoy2->setButton(SC2::Brake,false);
//}
//
//void Profile::set_JOYXY_for_rollNpitch()
//{
//	//UnmapAxis(m_pMcgPro, JOY::JOYX);
//	//UnmapAxis(m_pMcgPro, JOY::JOYY);
//
//	// set strafe axes at 0, otherwise they keep their last values
//	m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeLat, 0.0f);
//	m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeLongi, 0.0f);
//
//	//// joystick : roll and pitch
//	//MapAxis(m_pMcgPro, JOY::JOYX, AllLayers, m_pVirtualJoy1, SC1::AxisFlightRoll, AxisDirection::Normal);
//	//MapAxis(m_pMcgPro, JOY::JOYY, AllLayers, m_pVirtualJoy1, SC1::AxisFlightPitch, AxisDirection::Normal);
//
//	// set roll and pitch axes at correct value, otherwise it is necessary to wait for a movement of the stick
//	//m_pVirtualJoy1->setAxis(SC1::AxisFlightRoll, m_pMcgPro->axisValue(JOY::JOYX));
//	//m_pVirtualJoy1->setAxis(SC1::AxisFlightPitch, m_pMcgPro->axisValue(JOY::JOYY));
//	m_pVirtualJoy2->setButton(SC2::Brake, false);
//}
//
//
//
//
//
//// set_BUTTONS_for_landing
//// set_BUTTONS_for_cruise
//void Profile::set_BUTTONS_for_landing()
//{
//	//UnmapButton(m_pMcgPro, JOY::S1);
//	//UnmapButton(m_pMcgPro, JOY::S2);
//	//UnmapButton(m_pMcgPro, JOY::H2U);
//	//UnmapButton(m_pMcgPro, JOY::H2D);
//	//UnmapButton(m_pMcgPro, JOY::H3U);
//	//UnmapButton(m_pMcgPro, JOY::H3D);
//	//
//	//MapButton(m_pMcgPro, JOY::S1, {"i"}, m_pVirtualJoy1, SC1::TargetNearestHostile);
//	//MapButton(m_pMcgPro, JOY::S1, {"o"}, m_pVirtualJoy1, SC1::CycleAllTargets); // to cycle landing pads
//	////MapButton(m_pMcgPro, JOY::S2, AllLayers, m_pVirtualJoy1, SC1::LandingTargetLockOnToggle); // to select a landing pad
//	//
//	//MapButton(m_pMcgPro, JOY::H2U, AllLayers, m_pVirtualJoy1, SC1::StrafeUp);
//	//MapButton(m_pMcgPro, JOY::H2D, AllLayers, m_pVirtualJoy1, SC1::StrafeDown);
//	//MapButton(m_pMcgPro, JOY::H3U, AllLayers, m_pVirtualJoy1, SC1::StrafeForward);
//	//MapButton(m_pMcgPro, JOY::H3D, AllLayers, m_pVirtualJoy1, SC1::StrafeBackwards);
//}
//
//void Profile::set_BUTTONS_for_cruise()
//{
//	//UnmapButton(m_pMcgPro, JOY::S1);
//	//UnmapButton(m_pMcgPro, JOY::S2);
//	//UnmapButton(m_pMcgPro, JOY::H2U);
//	//UnmapButton(m_pMcgPro, JOY::H2D);
//	//UnmapButton(m_pMcgPro, JOY::H3U);
//	//UnmapButton(m_pMcgPro, JOY::H3D);
//	//
//	//MapButton(m_pMcgPro, JOY::S1, {"i"}, m_pVirtualJoy1, SC1::TargetNearestHostile);
//	//MapButton(m_pMcgPro, JOY::S1, {"o"}, m_pVirtualJoy1, SC1::CycleHostileTargets);
//	//MapButton(m_pMcgPro, JOY::S2, AllLayers, m_pVirtualJoy1, SC1::LaunchMissile);
//	//
//	//MapButton(m_pMcgPro, JOY::H2U, AllLayers, m_pVirtualJoy1, SC1::StrafeForward);
//	//MapButton(m_pMcgPro, JOY::H2D, AllLayers, m_pVirtualJoy1, SC1::StrafeBackwards);
//	//MapButton(m_pMcgPro, JOY::H3U, AllLayers, m_pVirtualJoy1, SC1::StrafeUp);
//	//MapButton(m_pMcgPro, JOY::H3D, AllLayers, m_pVirtualJoy1, SC1::StrafeDown);
//}
//
//
//// reset_THR_and_pedals
//// set_THR_and_pedals
//// set_THR_for_nothing
//// set_THR_for_landing
//// set_THR_for_cruise
//void Profile::reset_THR_and_pedals()
//{
//	UnmapAxis(m_pPedals, RUD::BRK_LEFT);
//	UnmapAxis(m_pPedals, RUD::BRK_RIGHT);
//	this->set_THRLEFT_for_nothing();
//	UnmapAxis(m_pThrottle, THR::THR_RIGHT);
//	m_pThrottle->removeCurve(THR::THR_LEFT);
//	m_pThrottle->removeCurve(THR::THR_RIGHT);
//	UnmapButton(m_pThrottle, THR::LTB);
//}
//
//void Profile::set_THR_and_pedals()
//{
//	this->reset_THR_and_pedals();
//
//	// mapping BRK_RIGHT function of THR_RIGHT position
//	MapAxis2(m_pThrottle, THR::THR_RIGHT, AllLayers, 1, {
//		new ActionCallback{ [this]() {this->set_BRKRIGHT_for_groundForward(); } },
//		new ActionCallback{ [this]() {this->set_BRKRIGHT_for_groundBackward(); } }
//		});
//	if (m_pThrottle->axisValue(THR::THR_RIGHT) < 0)
//		this->set_BRKRIGHT_for_groundForward();
//	else
//		this->set_BRKRIGHT_for_groundBackward();
//
//	// mapping BRK_LEFT for brake
//	Map(m_pPedals, ControlType::Axis, RUD::BRK_LEFT, AllLayers, new TriggerAxisChangeComparison{true,fBRAKE_THRESHOLD}, new ActionButtonPress{m_pVirtualJoy2,SC2::Brake});
//	Map(m_pPedals, ControlType::Axis, RUD::BRK_LEFT, AllLayers, new TriggerAxisChangeComparison{false,fBRAKE_THRESHOLD}, new ActionButtonRelease{m_pVirtualJoy2,SC2::Brake});
//	bool bBreak = m_pPedals->axisValue(RUD::BRK_LEFT) > fBRAKE_THRESHOLD;
//	m_pVirtualJoy2->setButton(SC2::Brake, bBreak);
//}
//
//void Profile::set_THR_for_landing()
//{
//	this->reset_THR_and_pedals();
//
//	// mapping THR_RIGHT and THR_LEFT
//	this->set_THRRIGHT_for_flightThrottle();
//	if (m_pThrottle->buttonPressed(THR::LTB))
//		this->set_THRLEFT_for_nothing();
//	else
//		this->set_THRLEFT_for_verticalStrafe();
//
//	// mapping LTB (to switch the role of THR_LEFT)
//	Map(m_pThrottle, ControlType::Button, THR::LTB, AllLayers, new TriggerButtonPress{}, new ActionCallback{ [this]() {this->set_THRLEFT_for_nothing(); } });
//	Map(m_pThrottle, ControlType::Button, THR::LTB, AllLayers, new TriggerButtonRelease{}, new ActionCallback{ [this]() {this->set_THRLEFT_for_verticalStrafe(); } });
//}
//
//void Profile::set_THR_for_cruise()
//{
//	this->reset_THR_and_pedals();
//	this->set_THRRIGHT_for_flightThrottle();
//	this->set_THRLEFT_for_nothing();
//}
//
//
//// set_BRKRIGHT_for_groundForward
//// set_BRKRIGHT_for_groundBackward
//// set_THRLEFT_for_verticalStrafe
//// set_THRLEFT_for_nothing
//// set_THRRIGHT_for_flightThrottle
//void Profile::set_BRKRIGHT_for_groundForward()
//{
//	MapAxis(m_pPedals, RUD::BRK_RIGHT, AllLayers, m_pVirtualJoy2, SC2::AxisGroundThrust, AxisDirection::Normal);
//	m_pPedals->setCustomCurve(RUD::BRK_RIGHT, {-1.0f+2.0f*fBRAKE_RIGHT_LDZ,0.0f,  1.0f-2.0f*fBRAKE_RIGHT_RDZ,1.0f});
//	m_pVirtualJoy2->setAxis(SC2::AxisGroundThrust, m_pPedals->axisValue(RUD::BRK_RIGHT));
//}
//
//void Profile::set_BRKRIGHT_for_groundBackward()
//{
//	MapAxis(m_pPedals, RUD::BRK_RIGHT, AllLayers, m_pVirtualJoy2, SC2::AxisGroundThrust, AxisDirection::Normal);
//	m_pPedals->setCustomCurve(RUD::BRK_RIGHT, {-1.0f+2.0f*fBRAKE_RIGHT_LDZ,0.0f,  1.0f-2.0f*fBRAKE_RIGHT_RDZ,-1.0f});
//	m_pVirtualJoy2->setAxis(SC2::AxisGroundThrust, m_pPedals->axisValue(RUD::BRK_RIGHT));
//}
//
//void Profile::set_THRLEFT_for_verticalStrafe()
//{
//	MapAxis(m_pThrottle, THR::THR_LEFT, AllLayers, m_pVirtualJoy1, SC1::AxisFlightStrafeVertical, AxisDirection::Reversed);
//	float reductionFactor = 0.2f;
//	m_pThrottle->setCustomCurve(THR::THR_LEFT, { -1.0f,-reductionFactor,  1.0f,reductionFactor });
//	m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeVertical, 0.0f);
//	m_pVirtualJoy1->setAxisTrim(SC1::AxisFlightStrafeVertical, m_pThrottle->axisValue(THR::THR_LEFT));
//}
//
//void Profile::set_THRLEFT_for_nothing()
//{
//	UnmapAxis(m_pThrottle, THR::THR_LEFT);
//	m_pVirtualJoy1->setAxis(SC1::AxisFlightStrafeVertical, 0.0f); // set vertical strafe axis at 0, otherwise they keep their last values
//}
//
//void Profile::set_THRRIGHT_for_flightThrottle()
//{
//	MapAxis(m_pThrottle, THR::THR_RIGHT, AllLayers, m_pVirtualJoy1, SC1::AxisFlightThrust, AxisDirection::Normal); // THR_RIGHT mapped to thrust
//	m_pVirtualJoy1->setAxis(SC1::AxisFlightThrust, m_pThrottle->axisValue(THR::THR_RIGHT)); // set axis at correct value, otherwise it is necessary to wait for a movement of the throttle
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// LED BRIGHTNESS DOWN ////////////////////////////////////////////////////////
//void Profile::ledBrightnessDown()
//{
//	if (m_iBrightness > 0)
//	{
//		m_iBrightness = lim<quint8>(m_iBrightness - 1, 0, 5);
//		m_pThrottle->setData("BRIGHTNESS", m_iBrightness);
//	}
//}
//
//// LED BRIGHTNESS UP //////////////////////////////////////////////////////////
//void Profile::ledBrightnessUp()
//{
//	if (m_iBrightness < 5)
//	{
//		m_iBrightness = lim<quint8>(m_iBrightness + 1, 0, 5);
//		m_pThrottle->setData("BRIGHTNESS", m_iBrightness);
//	}
//}
//
//// TOGGLE BACKLIT /////////////////////////////////////////////////////////////
//void Profile::toggleBacklit()
//{
//	m_bBacklit = !m_bBacklit;
//	m_pThrottle->setData("BACKLIT", m_bBacklit);
//}
