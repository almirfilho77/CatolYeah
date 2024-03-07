#ifndef __ASSERT_H__
#define __ASSERT_H__
#pragma once

#include "CatolYeah/Core/Logger.h"

/* Debug breakpoint */
#ifdef CY_ASSERT_ENABLED

#define CY_DEBUGBREAK			__debugbreak();
#define CY_ASSERT(x, message)\
	if ((x)) {\
	}\
	else {\
		CY_CORE_ERROR(message);\
		CY_DEBUGBREAK\
    }

#else

#define DEBUGBREAK
#define CY_ASSERT(x, message)

#endif

#endif /* __ASSERT_H__ */