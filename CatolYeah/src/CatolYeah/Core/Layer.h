#ifndef __LAYER_H__
#define __LAYER_H__
#pragma once

#include "CatolYeah/Core/Core.h"
#include "CatolYeah/Events/Event.h"
#include "CatolYeah/Core/Timestep.h"

namespace CatolYeah 
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer", bool isImGuiLayer = false);
		virtual ~Layer();

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnImGuiRender();
		virtual void OnUpdate(Timestep ts);
		virtual void OnEvent(Event& event);
		virtual bool IsImGuiLayer() { return t_isImGuiLayer; }

#ifdef CY_CONFIG_DEBUG
		inline const std::string& GetName() const { return t_name; }
#endif
	protected:
		std::string t_name;
		bool t_isImGuiLayer = false;
	};

}//CatolYeah

#endif /* __LAYER_H__ */