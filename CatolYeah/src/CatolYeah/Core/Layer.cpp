#include "cypch.h"
#include "Layer.h"

namespace CatolYeah
{

	Layer::Layer(const std::string& name, bool isImGuiLayer)
		:	t_name(name), t_isImGuiLayer(isImGuiLayer)
	{

	}

	Layer::~Layer()
	{

	}

	void Layer::OnAttach()
	{

	}

	void Layer::OnDetach()
	{

	}

	void Layer::OnImGuiRender()
	{

	}

	void Layer::OnUpdate(Timestep ts)
	{

	}

	void Layer::OnEvent(Event& event)
	{

	}

}//CatolYeah