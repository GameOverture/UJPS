#ifndef LAYERS_COMBO
#define LAYERS_COMBO


#include "Layer.h"
#include "ujpscore-global.h"
#include <vector>
#include <QString>


class UJPSCORE_EXPORT LayersCombo
{
	public:
		LayersCombo();
		LayersCombo(const std::vector<Layer> layers);
		LayersCombo(QString str);
		
		bool contains(Layer la) const;
		QString toString() const;
		
		
	private:
		std::vector<Layer> m_layers;
};


#endif

