#ifndef __LAYER_STACK_H__
#define __LAYER_STACK_H__
#pragma once

#include "CatolYeah/Core/Core.h"
#include "CatolYeah/Core/Layer.h"

namespace CatolYeah
{
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }

	private:
		std::vector<Layer*> m_layers;
		unsigned int m_layerInsertIndex = 0;
	};

}//CatolYeah

#endif /* __LAYER_STACK_H__ */