#ifndef __LAYER_STACK_H__
#define __LAYER_STACK_H__
#pragma once

#include "CatolYeah/Core.h"
#include "CatolYeah/Layer.h"

namespace CatolYeah
{
	class CATOLYEAH_API LayerStack
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
		std::vector<Layer*>::iterator m_layerInsert;
	};

}//CatolYeah

#endif /* __LAYER_STACK_H__ */