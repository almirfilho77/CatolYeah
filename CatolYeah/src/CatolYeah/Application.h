#ifndef __APPLICATION_H__
#define __APPLICATION_H__
#pragma once

#include "Core.h"

namespace CatolYeah {

	class CATOLYEAH_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();

}

#endif /* __APPLICATION_H__ */