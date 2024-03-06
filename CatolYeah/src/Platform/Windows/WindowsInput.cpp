#include "cypch.h"
#include "WindowsInput.h"

#include "GLFW/glfw3.h"
#include "CatolYeah/Core/Application.h"

namespace CatolYeah
{
	Input* Input::s_instance = new WindowsInput();

	bool CatolYeah::WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto glfw_window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto key_state = glfwGetKey(glfw_window, keycode);
		return key_state == GLFW_PRESS || key_state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto glfw_window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto button_state = glfwGetMouseButton(glfw_window, button);
		return button_state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl() const
	{
		auto glfw_window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(glfw_window, &xpos, &ypos);
		return { static_cast<float>(xpos), static_cast<float>(ypos) };
	}
	
	float WindowsInput::GetMouseXImpl() const
	{
		auto [xpos, ypos] = GetMousePositionImpl();	//C++17 allows us to name the pair instead of doing std::get<0>(pair)
		return xpos;
	}

	float WindowsInput::GetMouseYImpl() const
	{
		auto [xpos, ypos] = GetMousePositionImpl();
		return ypos;
	}


}//CatolYeah
