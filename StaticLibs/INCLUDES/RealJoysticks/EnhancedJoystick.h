#ifndef ENHANCED_JOYSTICK
#define ENHANCED_JOYSTICK


#include "AbstractRealJoystick.h"
#include "AbsoluteOrRelative.h"
#include "realjoysticks-global.h"
#include <array>
class AbstractAxisCurve;


class REALJOYSTICKS_EXPORT EnhancedJoystick : public AbstractRealJoystick
{
	public:
		EnhancedJoystick(AbstractRealJoystick *j, bool bOwn = true);
		EnhancedJoystick(const EnhancedJoystick &other) = delete;
		EnhancedJoystick(EnhancedJoystick &&other) = delete;
		EnhancedJoystick& operator=(const EnhancedJoystick &other) = delete;
		EnhancedJoystick& operator=(EnhancedJoystick &&other) = delete;
		virtual ~EnhancedJoystick();
		
		// reimplemented functions
		virtual uint id() const override;
		virtual QString description() const override;
		virtual QString hardwareId() const override;
		virtual void readGameController() override;
		virtual QVector<JoystickChange> changes() override;
		
		virtual uint buttonsCount() const override;
		virtual bool buttonPressed(uint button) const override;
		virtual QString buttonName(uint button) const override;
		virtual QStringList buttonsNames() const override;
		
		virtual uint axesCount() const override;
		virtual float axisValue(uint axis) const override;
		virtual QString axisName(uint axis) const override;
		virtual QStringList axesNames() const override;
		
		virtual uint povsCount() const override;
		virtual float povValue(uint pov) const override;
		virtual QString povName(uint pov) const override;
		virtual QStringList povsNames() const override;
		
		virtual void setData(const QString &str, QVariant v) override;
		virtual void flush() override;
		
		
		// decoration functions
		void setButtonLocked(uint button, bool locked);
		void setAxisLocked(uint axis, bool locked);
		void setAxisTrim(uint axis, float trim, AbsoluteOrRelative aor = AbsoluteOrRelative::Absolute);
		void setAxisInverted(uint axis, bool bInverted);
		float axisRawValue(uint axis) const;
		void setPovLocked(uint pov, bool locked);
		
		void setSCurve(uint axis, float lowerDZ, float centerDZ, float upperDZ, float curve, float zoom);
		void setJCurve(uint axis, float zoom);
		void setCustomCurve(uint axis, const std::vector<float> &points);
		void setCurve(uint axis, AbstractAxisCurve *curve);
		void removeCurve(uint axis);
		
		
	private:
		void updateAxis(uint axis);
		
		AbstractRealJoystick *m_j;
		bool m_bOwn;
		
		std::array<bool,128> m_buttonsLocked;
		std::array<bool,128> m_buttonsValuesBeforeLock;
		
		std::array<bool,8> m_axesLocked;
		std::array<float,8> m_axesTrim;
		std::array<float,8> m_axesInversionMod;
		std::array<float,8> m_axesValuesBeforeLock;
		std::array<AbstractAxisCurve*,8> m_axesCurves;
		std::vector<uint> m_axesToUpdate;
		
		std::array<bool,4> m_povsLocked;
		std::array<float,4> m_povsValuesBeforeLock;
};

#endif
