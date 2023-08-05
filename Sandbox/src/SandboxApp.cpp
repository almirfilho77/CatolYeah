#include <CatolYeah.h>
#include <iostream>

#include "imgui/imgui.h"

class ExampleLayer : public CatolYeah::Layer
{
public:
	ExampleLayer()
		: Layer("Example") {}

	void OnAttach() override
	{
		CY_TRACE("ExampleLayer::OnAttach!");
	}

	void OnUpdate() override
	{
		
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Text");
		ImGui::Text("This is a test!");
		ImGui::End();
	}

	void OnEvent(CatolYeah::Event& event) override
	{
		
	}
};

class Sandbox : public CatolYeah::Application
{
public:
	Sandbox()
	{
		CY_TRACE("Sandbox c'tor");
		PushLayer(new ExampleLayer());
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