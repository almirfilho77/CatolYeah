#ifndef __WINDOW_H__
#define __WINDOW_H__
#pragma once

#include "cypch.h"
#include "Events/Event.h"

namespace CatolYeah
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string &title = "CatolYeah Engine",
					unsigned int width = 1280,
					unsigned int height = 720)
			:	Title(title),
				Width(width),
				Height(height) {}
	};

	/*
	* @class	Window
	* @brief	Window abstraction class (platform agnostic interface)
	*/
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual bool OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual std::string GetGPUInfo() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}


#endif /* __WINDOW_H__ */
