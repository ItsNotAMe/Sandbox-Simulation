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
	void OnUpdateThread(float timestep, int fromX, int fromY, int order);

	Particle& GetParticle(int x, int y);
	void SetParticle(int x, int y, Particle& particle);
	void SetParticle(int x, int y, Particle&& particle);
	void SetParticle(int x, int y, uint8_t id);
	void SwapParticles(int x1, int y1, int x2, int y2, bool splashLiquid = false);
	bool IsEmpty(int x, int y);
	bool InBounds(int x, int y);

	bool IsSolid(int x, int y);
	bool IsLiquid(int x, int y);
	bool IsGas(int x, int y);
	bool IsParticle(int x, int y, uint8_t id);
	bool IsParticle(int x, int y, std::vector<uint8_t>& ids);
	bool IsInWater(int x, int y, int* lx, int* ly);
	bool IsInLiquid(int x, int y, int* lx, int* ly);
	int IsFlamable(int x, int y); // returns flamability value (the lower the more flamable + 0 = no)

	//bool FindPlace(int x, int y, int* xTo, int* yTo, int inc);

	void InitParticle(int x, int y, float timestep);

	void MoveVelocity(int x, int y);

	void MoveSand(int x, int y);
	void MoveWater(int x, int y);
	void MoveSteam(int x, int y);
	void MoveWood(int x, int y);
	void MoveFire(int x, int y);
	void MoveSmoke(int x, int y);
	void MoveOil(int x, int y);
	void MoveGunpowder(int x, int y);
	void MoveSalt(int x, int y);

	bool MovePowder(int x, int y, uint8_t id, int fR, int sR, std::vector<uint8_t> cannotPassThrough);
	bool MoveLiquid(int x, int y, uint8_t id, int fR, int sR, std::vector<uint8_t> cannotPassThrough);
	bool MoveGas(int x, int y, uint8_t id, int fR, int sR, std::vector<uint8_t> cannotPassThrough);
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