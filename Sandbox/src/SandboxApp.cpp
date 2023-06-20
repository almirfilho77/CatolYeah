#include <CatolYeah.h>
#include <iostream>

class Sandbox : public CatolYeah::Application
{
public:
	Sandbox()
	{
		std::cout << "Sandbox C'tor\n";
	}

	~Sandbox()
	{
		std::cout << "Sandbox D'tor\n";
	}
};

CatolYeah::Application* CatolYeah::CreateApplication()
{
	return new Sandbox();
}