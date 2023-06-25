#include <CatolYeah.h>
#include <iostream>

class Sandbox : public CatolYeah::Application
{
public:
	Sandbox()
	{
		CY_TRACE("Sandbox c'tor");
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