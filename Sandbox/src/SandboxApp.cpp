#include <CatolYeah.h>
#include <iostream>

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
		PushOverlay(new CatolYeah::ImGuiLayer());
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