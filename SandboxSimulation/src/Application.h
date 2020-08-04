#pragma once

#include "Simulator/Simulator.h"

class Application
{
public:
	static void Init();

	static void OnUpdate(float timestep);
	static void Render();
private:
	static Simulator* m_Sim;
	static int m_ParticleSize;
	static int m_BrushSize;
	static int m_CurrentBlock;
	static bool m_SolidBrush;

	static Random m_Rand;
};