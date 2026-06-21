#include "pch.h"
#include "RenderCommand.h"

inline void QuadArch::RenderCommand::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r,g,b,a);
}

inline void QuadArch::RenderCommand::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline void QuadArch::RenderCommand::Blend(bool enable)
{
	if (enable)
		glEnable(GL_BLEND);
	else
	{
		glDisable(GL_BLEND);
	}
}
