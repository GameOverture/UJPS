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

	const uint AxisAimLeftRight								= vJoyDevice::ROTY;
	const uint AxisAimUpDown								= vJoyDevice::ROTX;
	const uint AxisOnFootLeftRight							= vJoyDevice::X;
	const uint AxisOnFootFwdBck								= vJoyDevice::Z;
	const uint AxisVehicleFwdBck							= vJoyDevice::SLIDER0;
	const uint AxisVehicleLeftRight							= vJoyDevice::SLIDER1;

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
	const uint X________									= vJoyDevice::DX13;
	const uint LookBehind									= vJoyDevice::DX14;

	const uint Spacebrake									= vJoyDevice::DX15;
	const uint MatchTargetVelocity							= vJoyDevice::DX16;
	const uint ToggleDecoupledMode							= vJoyDevice::DX17;
	const uint CycleIFCS									= vJoyDevice::DX18;
	const uint ToggleGForceSafety							= vJoyDevice::DX19;
	const uint ToggleCOMSTAB								= vJoyDevice::DX20;
	const uint ToggleESP									= vJoyDevice::DX21;
	const uint Afterburner									= vJoyDevice::DX22;
	const uint Boost										= vJoyDevice::DX23;
	const uint ToggleLandingGear							= vJoyDevice::DX24;
	const uint Autoland										= vJoyDevice::DX25;
	const uint ToggleQuantumTravelSystem					= vJoyDevice::DX26;
	const uint QuantumDrive									= vJoyDevice::DX27;

	const uint ResetAim										= vJoyDevice::DX28;
	const uint LookAhead									= vJoyDevice::DX29;
	const uint GimbalLock									= vJoyDevice::DX30;
	const uint ReticleFocus									= vJoyDevice::DX31;
	const uint CycleAllTargets								= vJoyDevice::DX32;
	const uint CycleAllTargetsBack							= vJoyDevice::DX33;
	const uint CycleFriendlyTarget							= vJoyDevice::DX34;
	const uint CycleFriendlyTargetBack						= vJoyDevice::DX35;
	const uint PinFocusedTarget								= vJoyDevice::DX36;
	const uint CyclePinnedTargets							= vJoyDevice::DX37;
	const uint CyclePinnedTargetsBack						= vJoyDevice::DX38;
	const uint CycleHostileTargets							= vJoyDevice::DX39;
	const uint CycleHostileTargetsBack						= vJoyDevice::DX40;
	const uint TargetNearestHostile							= vJoyDevice::DX41;
	const uint ToggleReticleMode							= vJoyDevice::DX42;
	const uint TargetFocus									= vJoyDevice::DX43;
	const uint ToggleMiningMode								= vJoyDevice::DX44;
	const uint ToggleScanningMode							= vJoyDevice::DX45;
	const uint ScanningRadarPing							= vJoyDevice::DX46;
	const uint ScanningIncreaseRadarAngle					= vJoyDevice::DX47;
	const uint ScanningDecreaseRadarAngle					= vJoyDevice::DX48;

	const uint FireWeaponGroup1								= vJoyDevice::DX49;
	const uint FireWeaponGroup2								= vJoyDevice::DX50;
}

namespace StarCitizenControls_vJoy2
{
	const uint CycleWeaponAmmo								= vJoyDevice::DX1;
	const uint CycleWeaponAmmoBack							= vJoyDevice::DX2;
	const uint AcquireMissileLock							= vJoyDevice::DX3;
	const uint LaunchMissile								= vJoyDevice::DX4;

	const uint LaunchCountermeasure							= vJoyDevice::DX5;
	const uint CycleCountermeasureAmmo						= vJoyDevice::DX6;
	const uint CycleCountermeasureAmmoBack					= vJoyDevice::DX7;
	const uint ShieldRaiseFront								= vJoyDevice::DX8;
	const uint ShieldRaiseBack								= vJoyDevice::DX9;
	const uint ShieldRaiseLeft								= vJoyDevice::DX10;
	const uint ShieldRaiseRight								= vJoyDevice::DX11;
	const uint ShieldRaiseTop								= vJoyDevice::DX12;
	const uint ShieldRaiseBottom							= vJoyDevice::DX13;
	const uint ShieldResetLevels							= vJoyDevice::DX14;

	const uint PowerPreset1									= vJoyDevice::DX15;
	const uint PowerPreset2									= vJoyDevice::DX16;
	const uint PowerPreset3									= vJoyDevice::DX17;
	const uint ResetPowerDistribution						= vJoyDevice::DX18;
	const uint IncreasePower								= vJoyDevice::DX19;
	const uint DecreasePower								= vJoyDevice::DX20;
	const uint IncreasePowerMax								= vJoyDevice::DX21;
	const uint DecreasePowerMin								= vJoyDevice::DX22;
	const uint TogglePowerPreset1							= vJoyDevice::DX23;
	const uint TogglePowerPreset2							= vJoyDevice::DX24;
	const uint TogglePowerPreset3							= vJoyDevice::DX25;
	const uint TogglePower									= vJoyDevice::DX26;

	const uint TogglePersonalIdentificationBroadcastSystem	= vJoyDevice::DX27;
	const uint CycleRadarRange								= vJoyDevice::DX28;

	const uint Scoreboard									= vJoyDevice::DX29;
	const uint Map											= vJoyDevice::DX30;

	const uint ToggleLights									= vJoyDevice::DX31;

	const uint LeanLeft										= vJoyDevice::DX32;
	const uint LeanRight									= vJoyDevice::DX33;

	const uint VehicleHorn									= vJoyDevice::DX34;
	const uint VehicleBrake									= vJoyDevice::DX35;

	const uint CameraIncreaseFov							= vJoyDevice::DX36;
	const uint CameraDecreaseFov							= vJoyDevice::DX37;
	const uint CameraIncreaseDof							= vJoyDevice::DX38;
	const uint CameraDecreaseDof							= vJoyDevice::DX39;

	const uint TrackIR_Precision							= vJoyDevice::DX40;
	const uint TrackIR_Center								= vJoyDevice::DX41;
	const uint TrackIR_Pause								= vJoyDevice::DX42;
}

#endif

