#ifndef J_CURVE
#define J_CURVE


#include "AbstractAxisCurve.h"
#include "realjoysticks-global.h"


class REALJOYSTICKS_EXPORT JCurve : public AbstractAxisCurve
{
	public:
		JCurve(float zoom);
		JCurve(const JCurve &other) = delete;
		JCurve(JCurve &&other) = delete;
		JCurve& operator=(const JCurve &other) = delete;
		JCurve& operator=(JCurve &&other) = delete;
		virtual ~JCurve() = default;
		
		virtual float run(float in) override final;
		
		
	private:
		float m_zoom;
};

#endif
