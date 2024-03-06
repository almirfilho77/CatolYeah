#ifndef __IMGUI_LAYER_H__
#define __IMGUI_LAYER_H__
#pragma once

#include "CatolYeah/Core/Layer.h"
#include "CatolYeah/Events/ApplicationEvent.h"
#include "CatolYeah/Events/KeyEvent.h"
#include "CatolYeah/Events/MouseEvent.h"

namespace CatolYeah {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_time = 0.0f;
	};

}//CatolYeah

#endif /* __IMGUI_LAYER_H__ */