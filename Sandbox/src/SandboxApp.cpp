#include <CatolYeah.h>
#include <iostream>

class ExampleLayer : public CatolYeah::Layer
{
public:
	ExampleLayer()
		: Layer("Example") {}

	void OnUpdate() override
	{
		CY_INFO("ExampleLayer::OnUpdate!");
	}

	void OnEvent(CatolYeah::Event& event) override
	{
		CY_TRACE("{0}", event);
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