#ifndef __CORE_H__
#define __CORE_H__

#include <memory>

#define BIT(x) (1 << x)

#define CY_BIND_EVENT_FN(fn)	std::bind(&fn, this, std::placeholders::_1)

namespace CatolYeah
{
	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T>
	using Ref = std::shared_ptr<T>;
}

#endif /* __CORE_H__ */