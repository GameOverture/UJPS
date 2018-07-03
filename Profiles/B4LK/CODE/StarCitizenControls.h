#ifndef STAR_CITIZEN_CONTROLS
#define STAR_CITIZEN_CONTROLS

#include "WindowsKeys.h"
#include "vJoyDevice.h"

using uint = unsigned int;

namespace StarCitizenControls_vJoy1
{
	const uint AxisFlightPitch								= vJoyDevice::ROTX;
	const uint AxisFlightYaw								= vJoyDevice::ROTY;
	const uint AxisFlightRoll								= vJoyDevice::ROTZ;
	const uint AxisFlightStrafeLeftRight					= vJoyDevice::X;
	const uint AxisFlightStrafeUpDown						= vJoyDevice::Y;
	const uint AxisFlightStrafeFwdBck						= vJoyDevice::Z;
	const uint AxisFlightThrottle							= vJoyDevice::SLIDER0;
	const uint AxisFlightDynamicZoom						= vJoyDevice::SLIDER1;

	const uint Eject										= vJoyDevice::DX1;
	const uint ExitSeat										= vJoyDevice::DX2;
	const uint SelfDestruct									= vJoyDevice::DX3;
	const uint IncreaseCoolerRate							= vJoyDevice::DX4;
	const uint DecreaseCoolerRate							= vJoyDevice::DX5;
	const uint FlightSystemsReady							= vJoyDevice::DX6;
	const uint OpenAllDoors									= vJoyDevice::DX7;
	const uint CloseAllDoors								= vJoyDevice::DX8;
	const uint LockAllDoors									= vJoyDevice::DX9;
	const uint UnlockAllDoors								= vJoyDevice::DX10;

	const uint CycleCameraView								= vJoyDevice::DX11;
	const uint CycleCameraOrbitMode							= vJoyDevice::DX12;
	const uint LookBehind									= vJoyDevice::DX13;

	const uint Spacebrake									= vJoyDevice::DX14;
	const uint MatchTargetVelocity							= vJoyDevice::DX15;
	const uint ToggleDecoupledMode							= vJoyDevice::DX16;
	const uint CycleIFCS									= vJoyDevice::DX17;
	const uint ToggleGForceSafety							= vJoyDevice::DX18;
	const uint ToggleCOMSTAB								= vJoyDevice::DX19;
	const uint ToggleESP									= vJoyDevice::DX20;
	const uint Afterburner									= vJoyDevice::DX21;
	const uint Boost										= vJoyDevice::DX22;
	const uint ToggleLandingGear							= vJoyDevice::DX23;
	const uint Autoland										= vJoyDevice::DX24;
	const uint ToggleQuantumTravelSystem					= vJoyDevice::DX25;
	const uint QuantumDrive									= vJoyDevice::DX26;
	
	const uint ResetAim										= vJoyDevice::DX27;
	const uint LookAhead									= vJoyDevice::DX28;
	const uint GimbalLock									= vJoyDevice::DX29;
	const uint ReticleFocus									= vJoyDevice::DX30;
	const uint CycleAllTargets								= vJoyDevice::DX31;
	const uint CycleAllTargetsBack							= vJoyDevice::DX32;
	const uint CycleFriendlyTarget							= vJoyDevice::DX33;
	const uint CycleFriendlyTargetBack						= vJoyDevice::DX34;
	const uint PinFocusedTarget								= vJoyDevice::DX35;
	const uint CyclePinnedTargets							= vJoyDevice::DX36;
	const uint CyclePinnedTargetsBack						= vJoyDevice::DX37;
	const uint CycleHostileTargets							= vJoyDevice::DX38;
	const uint CycleHostileTargetsBack						= vJoyDevice::DX39;
	const uint TargetNearestHostile							= vJoyDevice::DX40;
	const uint ToggleReticleMode							= vJoyDevice::DX41;
	const uint TargetFocus									= vJoyDevice::DX42;
	const uint ToggleMiningMode								= vJoyDevice::DX43;
	const uint ToggleScanningMode							= vJoyDevice::DX44;
	const uint ScanningRadarPing							= vJoyDevice::DX45;
	const uint ScanningIncreaseRadarAngle					= vJoyDevice::DX46;
	const uint ScanningDecreaseRadarAngle					= vJoyDevice::DX47;
						
	const uint FireWeaponGroup1								= vJoyDevice::DX48;
	const uint FireWeaponGroup2								= vJoyDevice::DX49;
	const uint CycleWeaponAmmo								= vJoyDevice::DX50;
}

namespace StarCitizenControls_vJoy2
{
	const uint AxisAimLeftRight								= vJoyDevice::ROTY;
	const uint AxisAimUpDown								= vJoyDevice::ROTX;
	const uint AxisOnFootLeftRight							= vJoyDevice::X;
	const uint AxisOnFootFwdBck								= vJoyDevice::Z;
	const uint AxisVehicleFwdBck							= vJoyDevice::SLIDER0;
	const uint AxisVehicleLeftRight							= vJoyDevice::SLIDER1;
	const uint AxisMiningThrottle							= vJoyDevice::Y;

	const uint CycleWeaponAmmoBack							= vJoyDevice::DX1;
	const uint AcquireMissileLock							= vJoyDevice::DX2;
	const uint LaunchMissile								= vJoyDevice::DX3;
	
	const uint LaunchCountermeasure							= vJoyDevice::DX4;
	const uint CycleCountermeasureAmmo						= vJoyDevice::DX5;
	const uint CycleCountermeasureAmmoBack					= vJoyDevice::DX6;
	const uint ShieldRaiseFront								= vJoyDevice::DX7;
	const uint ShieldRaiseBack								= vJoyDevice::DX8;
	const uint ShieldRaiseLeft								= vJoyDevice::DX9;
	const uint ShieldRaiseRight								= vJoyDevice::DX10;
	const uint ShieldRaiseTop								= vJoyDevice::DX11;
	const uint ShieldRaiseBottom							= vJoyDevice::DX12;
	const uint ShieldResetLevels							= vJoyDevice::DX13;
	
	const uint PowerPreset1									= vJoyDevice::DX14;
	const uint PowerPreset2									= vJoyDevice::DX15;
	const uint PowerPreset3									= vJoyDevice::DX16;
	const uint ResetPowerDistribution						= vJoyDevice::DX17;
	const uint IncreasePower								= vJoyDevice::DX18;
	const uint DecreasePower								= vJoyDevice::DX19;
	const uint IncreasePowerMax								= vJoyDevice::DX20;
	const uint DecreasePowerMin								= vJoyDevice::DX21;
	const uint TogglePowerPreset1							= vJoyDevice::DX22;
	const uint TogglePowerPreset2							= vJoyDevice::DX23;
	const uint TogglePowerPreset3							= vJoyDevice::DX24;
	const uint TogglePower									= vJoyDevice::DX25;
	
	const uint TogglePersonalIdentificationBroadcastSystem	= vJoyDevice::DX26;
	const uint CycleRadarRange								= vJoyDevice::DX27;
	
	const uint Scoreboard									= vJoyDevice::DX28;
	const uint Map											= vJoyDevice::DX29;
	
	const uint ToggleLights									= vJoyDevice::DX30;

	const uint LeanLeft										= vJoyDevice::DX31;
	const uint LeanRight									= vJoyDevice::DX32;

	const uint VehicleHorn									= vJoyDevice::DX33;
	const uint VehicleBrake									= vJoyDevice::DX34;

	const uint CameraIncreaseFov							= vJoyDevice::DX35;
	const uint CameraDecreaseFov							= vJoyDevice::DX36;
	const uint CameraIncreaseDof							= vJoyDevice::DX37;
	const uint CameraDecreaseDof							= vJoyDevice::DX38;

	const uint MFDLeft										= vJoyDevice::DX39;
	const uint MFDRight										= vJoyDevice::DX40;
	const uint MFDUp										= vJoyDevice::DX41;
	const uint MFDDown										= vJoyDevice::DX42;

	const uint TrackIR_Precision							= vJoyDevice::DX43;
	const uint TrackIR_Center								= vJoyDevice::DX44;
	const uint TrackIR_Pause								= vJoyDevice::DX45;
}

#endif

