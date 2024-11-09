#include <CatolYeah.h>
#include <CatolYeah/Core/EntryPoint.h>
#include <iostream>

#include "imgui/imgui.h"

#include "BlockBall.h"
#include "Sandbox2D.h"

class DebugLayer : public CatolYeah::Layer
{
public:
	DebugLayer()
		: Layer("DebugLayer") {}

	void OnAttach() override
	{
		CY_TRACE("DebugLayer::OnAttach!");
	}

	void OnImGuiRender() override
	{
		std::string gpu_info = CatolYeah::Application::Get().GetWindow().GetGPUInfo();
		ImGui::Begin("GPU Info");
		ImGui::Text(gpu_info.c_str());
		ImGui::End();
	}
};

class Sandbox : public CatolYeah::Application
{
public:
	Sandbox()
	{
		CY_PROFILING_BEGIN_SESSION("Initialization", "results-Initialization.json");
		CY_TRACE("Sandbox c'tor");
		//PushLayer(new Sandbox2D());
		PushLayer(new BlockBall::BlockBall());
		PushLayer(new DebugLayer());
		CY_PROFILING_END_SESSION();
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