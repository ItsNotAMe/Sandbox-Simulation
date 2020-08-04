#include "Renderer.h"

#include "Util.h"

HDC Renderer::m_Hdc;
RenderData Renderer::m_RenderData;

void Renderer::Init(HDC& hdc)
{
	m_Hdc = hdc;
}

void Renderer::Resize(int width, int height)
{
	m_RenderData.BufferWidth = width;
	m_RenderData.BufferHeight = height;

	int bufferSize = m_RenderData.BufferWidth * m_RenderData.BufferHeight * sizeof(uint32_t);

	if (m_RenderData.BufferMemory)
		VirtualFree(m_RenderData.BufferMemory, 0, MEM_RELEASE);
	m_RenderData.BufferMemory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	m_RenderData.BitmapInfo.bmiHeader.biSize = sizeof(m_RenderData.BitmapInfo.bmiHeader);
	m_RenderData.BitmapInfo.bmiHeader.biWidth = m_RenderData.BufferWidth;
	m_RenderData.BitmapInfo.bmiHeader.biHeight = m_RenderData.BufferHeight;
	m_RenderData.BitmapInfo.bmiHeader.biPlanes = 1;
	m_RenderData.BitmapInfo.bmiHeader.biBitCount = 32;
	m_RenderData.BitmapInfo.bmiHeader.biCompression = BI_RGB;
}

void Renderer::Render()
{
	StretchDIBits(m_Hdc, 0, 0, m_RenderData.BufferWidth, m_RenderData.BufferHeight, 0, 0, m_RenderData.BufferWidth, m_RenderData.BufferHeight, m_RenderData.BufferMemory, &m_RenderData.BitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}

void Renderer::DrawRect(int left, int bottom, int width, int height, uint32_t color)
{
	int right = left + width;
	int top = bottom + height;
	if (right <= 0 || left >= m_RenderData.BufferWidth || top <= 0 || bottom >= m_RenderData.BufferHeight)
		return;
	left = Util::Clamp(left, 0.0f, m_RenderData.BufferWidth);
	right = Util::Clamp(right, 0.0f, m_RenderData.BufferWidth);
	bottom = Util::Clamp(bottom, 0.0f, m_RenderData.BufferHeight);
	top = Util::Clamp(top, 0.0f, m_RenderData.BufferHeight);
	unsigned int* pixel = (unsigned int*)m_RenderData.BufferMemory;
	for (int y = bottom; y < top; y++)
		for (int x = left; x < right; x++)
			pixel[x + y * m_RenderData.BufferWidth] = color;
}

void Renderer::DrawOutline(int left, int bottom, int width, int height, uint32_t color)
{
	int right = left + width;
	int top = bottom + height;
	if (right <= 0 || left >= m_RenderData.BufferWidth || top <= 0 || bottom >= m_RenderData.BufferHeight)
		return;
	left = Util::Clamp(left, 0.0f, m_RenderData.BufferWidth);
	right = Util::Clamp(right, 0.0f, m_RenderData.BufferWidth);
	bottom = Util::Clamp(bottom, 0.0f, m_RenderData.BufferHeight);
	top = Util::Clamp(top, 0.0f, m_RenderData.BufferHeight);
	unsigned int* pixel = (unsigned int*)m_RenderData.BufferMemory;
	for (int y = bottom; y < top; y++)
	{
		pixel[left + y * m_RenderData.BufferWidth] = color;
		pixel[right - 1 + y * m_RenderData.BufferWidth] = color;
	}
	for (int x = left; x < right; x++)
	{
		pixel[x + bottom * m_RenderData.BufferWidth] = color;
		pixel[x + (top - 1) * m_RenderData.BufferWidth] = color;
	}
}

void Renderer::Clear(uint32_t color)
{
	unsigned int* pixel = (unsigned int*)m_RenderData.BufferMemory;
	for (int y = 0; y < m_RenderData.BufferHeight; y++)
		for (int x = 0; x < m_RenderData.BufferWidth; x++)
			pixel[x + y * m_RenderData.BufferWidth] = color;
}
