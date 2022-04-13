#pragma once

// Forward Declarations
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

// Wrap an OpenGL function with glCall to allow error checking for it in debug mode
#define ASSERT(x) if (!(x)) __debugbreak()
#ifdef JG_DEBUG
#define glCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define glCall(x) x
#endif
