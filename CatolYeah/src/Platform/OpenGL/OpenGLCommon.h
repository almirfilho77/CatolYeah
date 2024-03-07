#pragma once

#include "glad/glad.h"

#ifdef _DEBUG

#define GLCallVoid(x) GLClearError();\
    x;\
    CY_ASSERT(GLLogCall(#x, __FILE__, __LINE__), "OpenGL Error!");

#define GLCall(x) [&](){\
    GLClearError();\
    auto retval = x;\
    CY_ASSERT(GLLogCall(#x, __FILE__, __LINE__), "OpenGL Error!")\
    return retval;\
    }()
#else

#define GLCallVoid(x) x
#define GLCall(x) x

#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);