#pragma once

#include <windows.h>
#include <cstdint>

struct RenderData
{
	void* BufferMemory;
	int BufferWidth, BufferHeight;
	BITMAPINFO BitmapInfo;
};

class Renderer
{
public:
	static void Init(HDC& hdc);
	static void Resize(int width, int height);

	static int GetWidth() { return m_RenderData.BufferWidth; }
	static int GetHeight() { return m_RenderData.BufferHeight; }

	static void Render();

	static void DrawRect(int left, int bottom, int width, int height, uint32_t color);
	static void DrawOutline(int left, int bottom, int width, int height, uint32_t color);

	static void Clear(uint32_t color);
private:
	static HDC m_Hdc;
	static RenderData m_RenderData;
};