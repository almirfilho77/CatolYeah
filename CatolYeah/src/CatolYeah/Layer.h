#ifndef __LAYER_H__
#define __LAYER_H__
#pragma once

#include "CatolYeah/Core.h"
#include "CatolYeah/Events/Event.h"

namespace CatolYeah 
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnImGuiRender();
		virtual void OnUpdate();
		virtual void OnEvent(Event& event);

#ifdef CY_CONFIG_DEBUG
		inline const std::string& GetName() const { return t_name; }
#endif
	protected:
		std::string t_name;
	};

}//CatolYeah

#endif /* __LAYER_H__ */