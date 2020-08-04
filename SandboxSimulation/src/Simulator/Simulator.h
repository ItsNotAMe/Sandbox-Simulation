#pragma once

#include "Particle.h"
#include "../Random.h"

struct Rect
{
	int Left = 0, Right = 0;
	int Bottom = 0, Top = 0;
};

class Simulator
{
public:
	Simulator() = delete;
	Simulator(const Simulator&) = default;
	Simulator(int width, int height, int particleSize);

	void OnUpdate(float timestep);
	void Render();

	void AddParticle(uint8_t id, int x, int y);
	void RemoveParticle(int x, int y);

	void SwapDebug() { m_Debug = !m_Debug; }
private:
	void OnUpdateThread(float timestep, int fromY, int fromX, int order);

	Particle& GetParticle(int x, int y);
	void SetParticle(int x, int y, Particle& particle);
	void SetParticle(int x, int y, Particle&& particle);
	void SwapParticles(int x1, int y1, int x2, int y2, bool splashWater = false);
	bool IsEmpty(int x, int y);
	bool InBounds(int x, int y);

	bool IsSolid(int x, int y);
	//bool IsLiquid(int x, int y);
	bool IsGas(int x, int y);
	bool IsParticle(int x, int y, uint8_t id);
	bool IsInLiquid(int x, int y, int* lx, int* ly);

	void MoveVelocity(int x, int y);

	void MoveSand(int x, int y);
	void MoveWater(int x, int y);
	void MoveSteam(int x, int y);
	void MoveWood(int x, int y);
	void MoveFire(int x, int y);
private:
	Particle* m_Particles;
	Rect* chunks;
	int m_ChunksX, m_ChunksY;
	int m_Width, m_Height;
	const int m_ParticleSize;
	const int m_ChunkSize = 64;

	Random rand;

	bool m_Debug = false;

	const float m_UpdatePerSecond = 60.0f;
	float m_Timer = 1 / m_UpdatePerSecond;
};