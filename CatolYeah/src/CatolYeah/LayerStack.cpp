#include "cypch.h"
#include "LayerStack.h"

namespace CatolYeah
{
	LayerStack::LayerStack()
	{
		CY_CORE_TRACE("Layer Stack c'tor");
	}

	LayerStack::~LayerStack()
	{
		CY_CORE_TRACE("Layer Stack d'tor");
		for (Layer* layer : m_layers)
		{
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		// m_layerInsert will always be at the back of the layer portion of the stack, but always
		// in front of the overlays in the stack (rendered first)
		m_layers.emplace(m_layers.begin() + m_layerInsertIndex, layer);
		m_layerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		// Emplace it to the back of the stack so it gets rendered last
		m_layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(begin(), end(), layer);
		if (it != end())
		{
			m_layers.erase(it);
			m_layerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(begin(), end(), overlay);
		if (it != end())
		{
			m_layers.erase(it);
		}
	}

}//CatolYeah