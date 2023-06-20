#ifndef __ENTRY_POINT_H__
#define __ENTRY_POINT_H__

#ifdef CY_PLATFORM_WINDOWS

extern CatolYeah::Application* CatolYeah::CreateApplication();

int main(int argc, char **argv)
{
	auto app = CatolYeah::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif

#endif /* __ENTRY_POINT_H__ */