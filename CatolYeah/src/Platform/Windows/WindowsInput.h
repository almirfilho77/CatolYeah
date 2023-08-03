#ifndef __WINDOWS_INPUT_H__
#define __WINDOWS_INPUT_H__
#pragma once

#include "CatolYeah/Input.h"

namespace CatolYeah
{

	class WindowsInput : public Input
	{
	protected:
		/* Keyboard related */
		virtual bool IsKeyPressedImpl(int keycode) override;

		/* Mouse related */
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() const override;
		virtual float GetMouseXImpl() const override;
		virtual float GetMouseYImpl() const override;
	};

}//CatolYeah

#endif /* __WINDOWS_INPUT_H__ */