#include <CatolYeah.h>
#include <CatolYeah/Core/EntryPoint.h>
#include <iostream>

#include "imgui/imgui.h"

#include "BlockBall.h"
#include "Sandbox2D.h"

class DebugLayer : public CatolYeah::Layer
{
public:
	bool isProfilerActive;
	char* profileBuffer;
public:
	DebugLayer()
		: Layer("DebugLayer"), isProfilerActive(false)
	{
		profileBuffer = new char[100];
	}

	~DebugLayer()
	{
		delete profileBuffer;
	}

	void OnAttach() override
	{
		CY_TRACE("DebugLayer::OnAttach!");
	}

	void ToggleProfillingOnOff(bool onOff, char *name)
	{
		if (onOff)
		{
			CY_PROFILING_BEGIN_SESSION(name, name);
		}
		else
		{
			CY_PROFILING_END_SESSION();
		}
	}

	void OnImGuiRender() override
	{
		std::string gpu_info = CatolYeah::Application::Get().GetWindow().GetGPUInfo();
		ImGui::Begin("GPU Info");
		ImGui::Text(gpu_info.c_str());
		ImGui::End();

		ImGui::Begin("Profilling");
		ImGui::InputText("Output filename", profileBuffer, 100);
		static std::string label = "Start profiling";
		if (ImGui::Button(label.c_str()))
		{
			isProfilerActive = !isProfilerActive;
			ToggleProfillingOnOff(isProfilerActive, profileBuffer);
			if (isProfilerActive) label = "Stop profiling";
			else label = "Start profiling";
		}
		ImGui::End();
	}
};

class Sandbox : public CatolYeah::Application
{
public:
	Sandbox()
	{
		CY_TRACE("Sandbox c'tor");
		PushLayer(new Sandbox2D());
		//PushLayer(new BlockBall::BlockBall());
		PushLayer(new DebugLayer());
	}

	~Sandbox()
	{
		CY_TRACE("Sandbox d'tor");
	}
};

CatolYeah::Application* CatolYeah::CreateApplication()
{
	return new Sandbox();
}