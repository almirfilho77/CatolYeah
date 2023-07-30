#include "cypch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

#include "CatolYeah/Application.h"

namespace CatolYeah {

    static ImGuiKey s_GLFWKeyToImGuiKey(int key);
    static void s_UpdateKeyModifiers();

	ImGuiLayer::ImGuiLayer()
		:	Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		CY_CORE_TRACE("ImGuiLayer::OnAttach");
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		ImGui::StyleColorsDark();

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		float window_width = static_cast<float>(app.GetWindow().GetWidth());
		float window_height = static_cast<float>(app.GetWindow().GetHeight());
		//CY_CORE_DEBUG("Window dimensions: [width={0}] [height={1}]", window_width, window_height);
		io.DisplaySize = ImVec2(window_width, window_height);

		float time = (float)glfwGetTime();
		io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
		m_time = time;
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		CY_CORE_DEBUG("{0}", event);
		EventDispatcher dispatcher(event);
		/* Mouse Events */
		dispatcher.Dispatch<MouseButtonPressedEvent>(CY_BIND_EVENT_FN(ImGuiLayer::m_OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(CY_BIND_EVENT_FN(ImGuiLayer::m_OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(CY_BIND_EVENT_FN(ImGuiLayer::m_OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(CY_BIND_EVENT_FN(ImGuiLayer::m_OnMouseScrolledEvent));

		/* Key Events */
		dispatcher.Dispatch<KeyPressedEvent>(CY_BIND_EVENT_FN(ImGuiLayer::m_OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(CY_BIND_EVENT_FN(ImGuiLayer::m_OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(CY_BIND_EVENT_FN(ImGuiLayer::m_OnKeyTypedEvent));
	}

	bool ImGuiLayer::m_OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto button = e.GetMouseButton();
		if (button >= 0 && button < ImGuiMouseButton_COUNT)
			io.AddMouseButtonEvent(button, GLFW_PRESS);

		return EVENT_RETURN_PASS_ON;
	}

	bool ImGuiLayer::m_OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto button = e.GetMouseButton();
		if (button >= 0 && button < ImGuiMouseButton_COUNT)
			io.AddMouseButtonEvent(button, GLFW_RELEASE);

		return EVENT_RETURN_PASS_ON;
	}

	bool ImGuiLayer::m_OnMouseMovedEvent(MouseMovedEvent& e)
	{
        ImGuiIO& io = ImGui::GetIO();
        /*if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            int window_x, window_y;
            glfwGetWindowPos(window, &window_x, &window_y);
            x += window_x;
            y += window_y;
        }*/
        io.AddMousePosEvent(e.GetMouseX(), e.GetMouseY());

        return EVENT_RETURN_PASS_ON;
	}

	bool ImGuiLayer::m_OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseWheelEvent(e.GetXOffset(), e.GetYOffset());

		return EVENT_RETURN_PASS_ON;
	}

	bool ImGuiLayer::m_OnKeyPressedEvent(KeyPressedEvent& e)
	{
        s_UpdateKeyModifiers();
        ImGuiIO& io = ImGui::GetIO();
        ImGuiKey imgui_key = s_GLFWKeyToImGuiKey(e.GetKeyCode());
        io.AddKeyEvent(imgui_key, true);

        return EVENT_RETURN_PASS_ON;
	}

	bool ImGuiLayer::m_OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
        s_UpdateKeyModifiers();
        ImGuiIO& io = ImGui::GetIO();
        ImGuiKey imgui_key = s_GLFWKeyToImGuiKey(e.GetKeyCode());
        io.AddKeyEvent(imgui_key, false);

        return EVENT_RETURN_PASS_ON;
	}

    bool ImGuiLayer::m_OnKeyTypedEvent(KeyTypedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddInputCharacter(e.GetKeyCode());

        return EVENT_RETURN_PASS_ON;
    }

	ImGuiKey s_GLFWKeyToImGuiKey(int key)
    {
        switch (key)
        {
        case GLFW_KEY_TAB: return ImGuiKey_Tab;
        case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
        case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
        case GLFW_KEY_UP: return ImGuiKey_UpArrow;
        case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
        case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
        case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
        case GLFW_KEY_HOME: return ImGuiKey_Home;
        case GLFW_KEY_END: return ImGuiKey_End;
        case GLFW_KEY_INSERT: return ImGuiKey_Insert;
        case GLFW_KEY_DELETE: return ImGuiKey_Delete;
        case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
        case GLFW_KEY_SPACE: return ImGuiKey_Space;
        case GLFW_KEY_ENTER: return ImGuiKey_Enter;
        case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
        case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
        case GLFW_KEY_COMMA: return ImGuiKey_Comma;
        case GLFW_KEY_MINUS: return ImGuiKey_Minus;
        case GLFW_KEY_PERIOD: return ImGuiKey_Period;
        case GLFW_KEY_SLASH: return ImGuiKey_Slash;
        case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
        case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
        case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
        case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
        case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
        case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
        case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
        case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
        case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
        case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
        case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
        case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
        case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
        case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
        case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
        case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
        case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
        case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
        case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
        case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
        case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
        case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
        case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
        case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
        case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
        case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
        case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
        case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
        case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
        case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
        case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
        case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
        case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
        case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
        case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
        case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
        case GLFW_KEY_MENU: return ImGuiKey_Menu;
        case GLFW_KEY_0: return ImGuiKey_0;
        case GLFW_KEY_1: return ImGuiKey_1;
        case GLFW_KEY_2: return ImGuiKey_2;
        case GLFW_KEY_3: return ImGuiKey_3;
        case GLFW_KEY_4: return ImGuiKey_4;
        case GLFW_KEY_5: return ImGuiKey_5;
        case GLFW_KEY_6: return ImGuiKey_6;
        case GLFW_KEY_7: return ImGuiKey_7;
        case GLFW_KEY_8: return ImGuiKey_8;
        case GLFW_KEY_9: return ImGuiKey_9;
        case GLFW_KEY_A: return ImGuiKey_A;
        case GLFW_KEY_B: return ImGuiKey_B;
        case GLFW_KEY_C: return ImGuiKey_C;
        case GLFW_KEY_D: return ImGuiKey_D;
        case GLFW_KEY_E: return ImGuiKey_E;
        case GLFW_KEY_F: return ImGuiKey_F;
        case GLFW_KEY_G: return ImGuiKey_G;
        case GLFW_KEY_H: return ImGuiKey_H;
        case GLFW_KEY_I: return ImGuiKey_I;
        case GLFW_KEY_J: return ImGuiKey_J;
        case GLFW_KEY_K: return ImGuiKey_K;
        case GLFW_KEY_L: return ImGuiKey_L;
        case GLFW_KEY_M: return ImGuiKey_M;
        case GLFW_KEY_N: return ImGuiKey_N;
        case GLFW_KEY_O: return ImGuiKey_O;
        case GLFW_KEY_P: return ImGuiKey_P;
        case GLFW_KEY_Q: return ImGuiKey_Q;
        case GLFW_KEY_R: return ImGuiKey_R;
        case GLFW_KEY_S: return ImGuiKey_S;
        case GLFW_KEY_T: return ImGuiKey_T;
        case GLFW_KEY_U: return ImGuiKey_U;
        case GLFW_KEY_V: return ImGuiKey_V;
        case GLFW_KEY_W: return ImGuiKey_W;
        case GLFW_KEY_X: return ImGuiKey_X;
        case GLFW_KEY_Y: return ImGuiKey_Y;
        case GLFW_KEY_Z: return ImGuiKey_Z;
        case GLFW_KEY_F1: return ImGuiKey_F1;
        case GLFW_KEY_F2: return ImGuiKey_F2;
        case GLFW_KEY_F3: return ImGuiKey_F3;
        case GLFW_KEY_F4: return ImGuiKey_F4;
        case GLFW_KEY_F5: return ImGuiKey_F5;
        case GLFW_KEY_F6: return ImGuiKey_F6;
        case GLFW_KEY_F7: return ImGuiKey_F7;
        case GLFW_KEY_F8: return ImGuiKey_F8;
        case GLFW_KEY_F9: return ImGuiKey_F9;
        case GLFW_KEY_F10: return ImGuiKey_F10;
        case GLFW_KEY_F11: return ImGuiKey_F11;
        case GLFW_KEY_F12: return ImGuiKey_F12;
        default: return ImGuiKey_None;
        }
    }

    void s_UpdateKeyModifiers()
    {
        Application& app = Application::Get();
        bool is_ctrl_key_pressed = (app.GetWindow().GetKeyIsPressed(GLFW_KEY_LEFT_CONTROL)) || (app.GetWindow().GetKeyIsPressed(GLFW_KEY_RIGHT_CONTROL));
        bool is_shift_key_pressed = (app.GetWindow().GetKeyIsPressed(GLFW_KEY_LEFT_SHIFT)) || (app.GetWindow().GetKeyIsPressed(GLFW_KEY_RIGHT_SHIFT));
        bool is_alt_key_pressed = (app.GetWindow().GetKeyIsPressed(GLFW_KEY_LEFT_ALT)) || (app.GetWindow().GetKeyIsPressed(GLFW_KEY_RIGHT_ALT));
        bool is_super_key_pressed = (app.GetWindow().GetKeyIsPressed(GLFW_KEY_LEFT_SUPER)) || (app.GetWindow().GetKeyIsPressed(GLFW_KEY_RIGHT_SUPER));

        ImGuiIO& io = ImGui::GetIO();
        io.AddKeyEvent(ImGuiMod_Ctrl, is_ctrl_key_pressed);
        io.AddKeyEvent(ImGuiMod_Shift, is_shift_key_pressed);
        io.AddKeyEvent(ImGuiMod_Alt, is_alt_key_pressed);
        io.AddKeyEvent(ImGuiMod_Super, is_super_key_pressed);
    }
}//CatolYeah