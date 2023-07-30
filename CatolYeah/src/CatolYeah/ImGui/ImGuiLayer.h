#ifndef __IMGUI_LAYER_H__
#define __IMGUI_LAYER_H__
#pragma once

#include "CatolYeah/Layer.h"
#include "CatolYeah/Events/ApplicationEvent.h"
#include "CatolYeah/Events/KeyEvent.h"
#include "CatolYeah/Events/MouseEvent.h"

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
		bool m_OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool m_OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool m_OnMouseMovedEvent(MouseMovedEvent& e);
		bool m_OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool m_OnKeyPressedEvent(KeyPressedEvent& e);
		bool m_OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool m_OnKeyTypedEvent(KeyTypedEvent& e);
	private:
		float m_time = 0.0f;
	};

}//CatolYeah

#endif /* __IMGUI_LAYER_H__ */