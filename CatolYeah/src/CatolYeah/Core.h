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

/* Debug breakpoint */
#ifdef CY_CONFIG_DEBUG
#define DEBUGBREAK			__debugbreak();
#else
#define DEBUGBREAK
#endif/* Debug breakpoint */

#define BIT(x) (x << 1)

#define CY_BIND_EVENT_FN(fn)	std::bind(&fn, this, std::placeholders::_1)

#endif /* __CORE_H__ */