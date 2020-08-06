#include "Application.h"

#include "Renderer.h"
#include "Input/Input.h"
#include "Util.h"

Simulator* Application::m_Sim = nullptr;
int Application::m_ParticleSize = 1;
int Application::m_BrushSize = 20 / m_ParticleSize, Application::m_CurrentBlock = 1;
bool Application::m_SolidBrush = true, Application::m_CircleBrush = true;
Random Application::m_Rand;

void Application::Init()
{
	if (!m_Sim)
		m_Sim = new Simulator(Renderer::GetWidth(), Renderer::GetHeight(), m_ParticleSize);
}

void Application::OnUpdate(float timestep)
{
	m_Sim->OnUpdate(timestep);

	if (Input::IsPressed(KEY_H)) // Debug
		m_Sim->SwapDebug();
	if (Input::IsPressed(KEY_S)) // Swap to solid brush
		m_SolidBrush = !m_SolidBrush;
	if (Input::IsPressed(KEY_C)) // Swap to circle brush
		m_CircleBrush = !m_CircleBrush;
	if (Input::IsPressed(KEY_A)) // Swap block
	{
		m_CurrentBlock--;
		m_CurrentBlock = Util::Clamp(m_CurrentBlock, 1.0f, 9.0f);
	}
	if (Input::IsPressed(KEY_D)) // Swap block
	{
		m_CurrentBlock++;
		m_CurrentBlock = Util::Clamp(m_CurrentBlock, 1.0f, 9.0f);
	}
	if (Input::IsPressed(KEY_LEFT_BRACKET)) // Change brush size
	{
		m_BrushSize--;
		if (m_BrushSize < 0)
			m_BrushSize = 0;
	}
	if (Input::IsPressed(KEY_RIGHT_BRACKET)) // Change brush size
		m_BrushSize++;
	if (Input::IsDown(MOUSE_BUTTON_LEFT)) // Add block
		for (int y = -m_BrushSize / 2; y <= m_BrushSize / 2 + m_BrushSize % 2; y++)
			for (int x = -m_BrushSize / 2; x <= m_BrushSize / 2 + m_BrushSize % 2; x++)
				if ((m_SolidBrush || m_Rand.Rand(0, 100) < 5) && (!m_CircleBrush || std::sqrt(x * x + y * y) <= m_BrushSize / 2))
					m_Sim->AddParticle(m_CurrentBlock, Input::GetMouseX() + x * m_ParticleSize, Input::GetMouseY() + y * m_ParticleSize);
	if (Input::IsDown(MOUSE_BUTTON_RIGHT)) // Remove block
		for (int y = m_BrushSize / 2 + m_BrushSize % 2; y >= -m_BrushSize / 2; y--)
			for (int x = -m_BrushSize / 2; x <= m_BrushSize / 2 + m_BrushSize % 2; x++)
				if (!m_CircleBrush || std::sqrt(x * x + y * y) <= m_BrushSize / 2)
					m_Sim->RemoveParticle(Input::GetMouseX() + x * m_ParticleSize, Input::GetMouseY() + y * m_ParticleSize);
}

void Application::Render()
{
	Renderer::Clear(0x333333);

	m_Sim->Render();
	
	if (!m_CircleBrush)
	{
		int brushX = (Input::GetMouseX() / m_ParticleSize - m_BrushSize / 2) * m_ParticleSize;
		int brushY = (Input::GetMouseY() / m_ParticleSize - m_BrushSize / 2) * m_ParticleSize;
		int brushSize = (m_BrushSize + 1) * m_ParticleSize;
		Renderer::DrawOutline(brushX, brushY, brushSize, brushSize, 0xddeedf);
	}
	else
	{
		for (int y = -m_BrushSize / 2; y <= m_BrushSize / 2 + m_BrushSize % 2; y++)
			for (int x = -m_BrushSize / 2; x <= m_BrushSize / 2 + m_BrushSize % 2; x++)
				if (std::sqrt(x * x + y * y) <= m_BrushSize / 2 && std::sqrt(x * x + y * y) >= m_BrushSize / 2 - 0.99)
					Renderer::DrawRect(Input::GetMouseX() / m_ParticleSize * m_ParticleSize + x * m_ParticleSize, Input::GetMouseY() / m_ParticleSize * m_ParticleSize + y * m_ParticleSize, m_ParticleSize, m_ParticleSize, 0xddeedf);
	}

	// draw selected particle:
	int size = 25;
	switch (m_CurrentBlock)
	{
		case MAT_ID_SAND: Renderer::DrawRect(Renderer::GetWidth() - size * 2, Renderer::GetHeight() - size * 2, size, size, MAT_COLOR_SAND); break;
		case MAT_ID_WATER: Renderer::DrawRect(Renderer::GetWidth() - size * 2, Renderer::GetHeight() - size * 2, size, size, MAT_COLOR_WATER); break;
		case MAT_ID_STEAM: Renderer::DrawRect(Renderer::GetWidth() - size * 2, Renderer::GetHeight() - size * 2, size, size, MAT_COLOR_STEAM); break;
		case MAT_ID_WOOD: Renderer::DrawRect(Renderer::GetWidth() - size * 2, Renderer::GetHeight() - size * 2, size, size, MAT_COLOR_WOOD); break;
		case MAT_ID_FIRE: Renderer::DrawRect(Renderer::GetWidth() - size * 2, Renderer::GetHeight() - size * 2, size, size, MAT_COLOR_FIRE); break;
		case MAT_ID_SMOKE: Renderer::DrawRect(Renderer::GetWidth() - size * 2, Renderer::GetHeight() - size * 2, size, size, MAT_COLOR_SMOKE); break;
		case MAT_ID_OIL: Renderer::DrawRect(Renderer::GetWidth() - size * 2, Renderer::GetHeight() - size * 2, size, size, MAT_COLOR_OIL); break;
		case MAT_ID_GUNPOWDER: Renderer::DrawRect(Renderer::GetWidth() - size * 2, Renderer::GetHeight() - size * 2, size, size, MAT_COLOR_GUNPOWDER); break;
		case MAT_ID_SALT: Renderer::DrawRect(Renderer::GetWidth() - size * 2, Renderer::GetHeight() - size * 2, size, size, MAT_COLOR_SALT); break;
		default:
			break;
	}
	Renderer::DrawOutline(Renderer::GetWidth() - size * 2, Renderer::GetHeight() - size * 2, size, size, 0xddeedf);

	Renderer::Render();
}