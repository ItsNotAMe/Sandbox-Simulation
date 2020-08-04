#include "Application.h"

#include "Renderer.h"
#include "Input/Input.h"
#include "Util.h"

Simulator* Application::m_Sim = nullptr;
int Application::m_ParticleSize = 20;
int Application::m_BrushSize = 20 / m_ParticleSize, Application::m_CurrentBlock = 1;
bool Application::m_SolidBrush = true;
Random Application::m_Rand;

void Application::Init()
{
	if (!m_Sim)
		m_Sim = new Simulator(Renderer::GetWidth(), Renderer::GetHeight(), m_ParticleSize);
}

void Application::OnUpdate(float timestep)
{
	if (Input::IsPressed(KEY_H)) // Debug
		m_Sim->SwapDebug();
	if (Input::IsPressed(KEY_S)) // Swap to solid brush
		m_SolidBrush = !m_SolidBrush;
	if (Input::IsPressed(KEY_A)) // Swap block
	{
		m_CurrentBlock--;
		m_CurrentBlock = Util::Clamp(m_CurrentBlock, 1.0f, 5.0f);
	}
	if (Input::IsPressed(KEY_D)) // Swap block
	{
		m_CurrentBlock++;
		m_CurrentBlock = Util::Clamp(m_CurrentBlock, 1.0f, 5.0f);
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
				if (m_SolidBrush || m_Rand.Rand(0, 100) < 5)
					m_Sim->AddParticle(m_CurrentBlock, Input::GetMouseX() + x * m_ParticleSize, Input::GetMouseY() + y * m_ParticleSize);
	if (Input::IsDown(MOUSE_BUTTON_RIGHT)) // Remove block
		for (int y = m_BrushSize / 2 + m_BrushSize % 2; y >= -m_BrushSize / 2; y--)
			for (int x = -m_BrushSize / 2; x <= m_BrushSize / 2 + m_BrushSize % 2; x++)
				m_Sim->RemoveParticle(Input::GetMouseX() + x * m_ParticleSize, Input::GetMouseY() + y * m_ParticleSize);

	m_Sim->OnUpdate(timestep);
}

void Application::Render()
{
	Renderer::Clear(0x333333);

	m_Sim->Render();
	
	int brushX = (Input::GetMouseX() / m_ParticleSize - m_BrushSize / 2) * m_ParticleSize;
	int brushY = (Input::GetMouseY() / m_ParticleSize - m_BrushSize / 2) * m_ParticleSize;
	int brushSize = (m_BrushSize + 1) * m_ParticleSize;
	Renderer::DrawOutline(brushX, brushY, brushSize, brushSize, 0xddeedf);

	Renderer::Render();
}