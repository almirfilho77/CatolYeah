#ifndef __GRAPHICS_CONTEXT_H__
#define __GRAPHICS_CONTEXT_H__
#pragma once

namespace CatolYeah
{

	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}//CatolYeah

#endif /* __GRAPHICS_CONTEXT_H__ */