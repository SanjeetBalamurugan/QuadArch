#include "pch.h"
#include "RenderCommand.h"

inline void QuadArch::RenderCommand::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r,g,b,a);
}

inline void QuadArch::RenderCommand::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}
