#ifndef __INPUT_H__
#define __INPUT_H__
#pragma once

#include "CatolYeah/Core/Core.h"

namespace CatolYeah
{
	class Input
	{
	public:
		/* Keyboard related */
		inline static bool IsKeyPressed(int keycode) { return s_instance->IsKeyPressedImpl(keycode); }
		
		/* Mouse related */
		inline static bool IsMouseButtonPressed(int button) { return s_instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_instance->GetMouseYImpl(); }

	protected:
		/* Keyboard related */
		virtual bool IsKeyPressedImpl(int keycode) = 0;

		/* Mouse related */
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() const = 0;
		virtual float GetMouseXImpl() const = 0;
		virtual float GetMouseYImpl() const = 0;

	private:
		static Input* s_instance;
	};
}//CatolYeah


#endif /* __INPUT_H__ */