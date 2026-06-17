#pragma once
#include "Core.h"

struct WindowSpecs {
	unsigned int width;
	unsigned int height;

	std::string title;
};

class QuadAPI Window
{
public:
	Window();
	~Window();

	void Init();
	void Update();
	void Destroy();

private:
	WindowSpecs* m_Specs = nullptr;
};

