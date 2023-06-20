#ifndef __CORE_H__
#define __CORE_H__

#ifdef CY_PLATFORM_WINDOWS
	#ifdef CY_BUILD_DLL
		#define CATOLYEAH_API __declspec(dllexport)
	#else
		#define CATOLYEAH_API __declspec(dllimport)
	#endif
#else
	#error CatolYeah only supports windows!
#endif

#endif /* __CORE_H__ */