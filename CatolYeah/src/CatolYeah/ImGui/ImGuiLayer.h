#ifndef __IMGUI_LAYER_H__
#define __IMGUI_LAYER_H__
#pragma once

#include "CatolYeah/Layer.h"

namespace CatolYeah {

	class CATOLYEAH_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;

	private:
		float m_time = 0.0f;
	};

}//CatolYeah

#endif /* __IMGUI_LAYER_H__ */