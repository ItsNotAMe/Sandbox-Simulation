#include "Simulator.h"

#include <stdlib.h>
#include <thread>
#include <vector>

#include "../Renderer.h"

Simulator::Simulator(int width, int height, int particleSize)
	: m_ParticleSize(particleSize)
{
	m_Width = width / particleSize;
	m_Height = height / particleSize;
	m_Particles = new Particle[m_Width * m_Height];
	m_ChunksX = m_Width / m_ChunkSize + (m_Width % m_ChunkSize > 0 ? 1 : 0);
	m_ChunksY = m_Height / m_ChunkSize + (m_Height % m_ChunkSize > 0 ? 1 : 0);
	chunks = new Rect[m_ChunksX * m_ChunksY];
}

void Simulator::OnUpdate(float timestep)
{
	m_Timer -= timestep;
	if (m_Timer > 0)
		return;
	m_Timer += 1 / m_UpdatePerSecond;

	/*float timestep1 = 0.016f;
	LARGE_INTEGER begin;
	QueryPerformanceCounter(&begin);
	float performanceFrequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performanceFrequency = (float)perf.QuadPart;
	}*/

	// check for pixels to update
	for (int chunkY = 0; chunkY < m_ChunksY; chunkY++)
	{
		for (int chunkX = 0; chunkX < m_ChunksX; chunkX++)
		{
			int range = 1;
			int none = -range - 1;
			int minY = none, maxY = none;
			int minX = none, maxX = none;
			for (int y = chunkY * m_ChunkSize; y < (chunkY + 1) * m_ChunkSize && y < m_Height; y++)
			{
				for (int x = chunkX * m_ChunkSize; x < (chunkX + 1) * m_ChunkSize && x < m_Width; x++)
				{
					if (GetParticle(x, y).HasBeenUpdatedThisFrame || GetParticle(x, y).IsUsingVelocity)
					{
						minX = min(minX, x - range);
						if (minX == none)
							minX = x - range;
						maxX = max(maxX, x + range);
						if (minY == none)
							minY = y - range;
						maxY = y + range;
					}
				}
			}
			chunks[chunkX + chunkY * m_ChunksX].Left = max(minX, 0);
			chunks[chunkX + chunkY * m_ChunksX].Right = min(maxX, m_Width - 1);
			chunks[chunkX + chunkY * m_ChunksX].Bottom = max(minY, 0);
			chunks[chunkX + chunkY * m_ChunksX].Top = min(maxY, m_Height - 1);
		}
	}

	/*LARGE_INTEGER end;
	QueryPerformanceCounter(&end);
	timestep1 = (float)(end.QuadPart - begin.QuadPart) / performanceFrequency;
	if (timestep1 > 0.016)
		begin = end;
	begin = end;*/

	for (int y = 0; y < m_Height; y++)
		for (int x = 0; x < m_Width; x++)
			GetParticle(x, y).HasBeenUpdatedThisFrame = false;

	int order = rand.Rand(0, 10) / 5;

	/*Simulator::OnUpdateThread(timestep, 0, 0, order);
	Simulator::OnUpdateThread(timestep, 0, 1, order);
	Simulator::OnUpdateThread(timestep, 1, 0, order);
	Simulator::OnUpdateThread(timestep, 1, 1, order);*/

	// TODO: fix the particles sometimes switching ids but not colors because of the threads
	std::thread t1(&Simulator::OnUpdateThread, this, timestep, 0, 0, order);
	std::thread t2(&Simulator::OnUpdateThread, this, timestep, 0, 1, order);
	std::thread t3(&Simulator::OnUpdateThread, this, timestep, 1, 0, order);
	//std::thread t4(&Simulator::OnUpdateThread, this, timestep, 1, 1, order);
	Simulator::OnUpdateThread(timestep, 1, 1, order);

	t1.join();
	t2.join();
	t3.join();
	//t4.join();
}

void Simulator::Render()
{
	for (int y = m_Height - 1; y >= 0; y--)
		for (int x = 0; x < m_Width; x++)
			if (!IsEmpty(x, y))
				Renderer::DrawRect(x * m_ParticleSize, y * m_ParticleSize, m_ParticleSize, m_ParticleSize, GetParticle(x, y).Color);
	if (m_Debug)
		for (int chunkY = 0; chunkY < m_ChunksY; chunkY++)
			for (int chunkX = 0; chunkX < m_ChunksX; chunkX++)
				if (chunks[chunkX + chunkY * m_ChunksX].Left != -1)
					Renderer::DrawOutline(chunks[chunkX + chunkY * m_ChunksX].Left * m_ParticleSize, chunks[chunkX + chunkY * m_ChunksX].Bottom * m_ParticleSize, (chunks[chunkX + chunkY * m_ChunksX].Right - chunks[chunkX + chunkY * m_ChunksX].Left + 1) * m_ParticleSize, (chunks[chunkX + chunkY * m_ChunksX].Top - chunks[chunkX + chunkY * m_ChunksX].Bottom + 1) * m_ParticleSize, 0xdd2233);
}

void Simulator::AddParticle(uint8_t id, int x, int y)
{
	x /= m_ParticleSize;
	y /= m_ParticleSize;
	if (!IsEmpty(x, y))
		return;
	Particle& particle = GetParticle(x, y);
	switch (id)
	{
		case MAT_ID_SAND:
			particle.Id = MAT_ID_SAND;
			particle.Color = MAT_COLOR_SAND;
			break;
		case MAT_ID_WATER:
			particle.Id = MAT_ID_WATER;
			particle.Color = MAT_COLOR_WATER;
			break;
		case MAT_ID_STEAM:
			particle.Id = MAT_ID_STEAM;
			particle.Color = MAT_COLOR_STEAM;
			break;
		case MAT_ID_WOOD:
			particle.Id = MAT_ID_WOOD;
			particle.Color = MAT_COLOR_WOOD;
			break;
		case MAT_ID_FIRE:
			particle.Id = MAT_ID_FIRE;
			particle.Color = MAT_COLOR_FIRE;
			break;
		default:
			break;
	}
	particle.LifeTime = 0;
	particle.Velocity = { 0, 0 };
	particle.HasBeenUpdatedThisFrame = true;
}

void Simulator::RemoveParticle(int x, int y)
{
	x /= m_ParticleSize;
	y /= m_ParticleSize;
	if (!InBounds(x, y))
		return;
	Particle& particle = GetParticle(x, y);
	particle.Id = MAT_ID_EMPTY;
	particle.LifeTime = 0;
	particle.Velocity = { 0, 0 };
	particle.HasBeenUpdatedThisFrame = true;
	for (int i = -1; i <= 1; i++)
	{
		int j = y;
		while (++j < m_Height && GetParticle(x + i, j).Id != MAT_ID_EMPTY)
			GetParticle(x + i, j).HasBeenUpdatedThisFrame = true;
	}
}

void Simulator::OnUpdateThread(float timestep, int fromY, int fromX, int order)
{
	for (int chunkY = fromY; chunkY < m_ChunksY; chunkY += 2)
	{
		for (int chunkX = order ? fromX : m_ChunksX - 1 - fromX; order ? chunkX < m_ChunksX : chunkX >= 0; order ? chunkX += 2 : chunkX -= 2)
		{
			int bottom = chunks[chunkX + chunkY * m_ChunksX].Bottom, top = chunks[chunkX + chunkY * m_ChunksX].Top;
			int left = chunks[chunkX + chunkY * m_ChunksX].Left, right = chunks[chunkX + chunkY * m_ChunksX].Right;
			if (left < 0)
				continue;
			for (int y = bottom; y <= top; y++)
			{
				for (int x = order ? left : right; order ? x <= right : x >= left; order ? x++ : x--)
				{
					Particle& particle = GetParticle(x, y);
					if (particle.HasBeenUpdatedThisFrame)
						continue;
					uint8_t id = particle.Id;
					particle.LifeTime += timestep;
					if (particle.IsUsingVelocity)
						MoveVelocity(x, y);
					else
					{
						switch (id)
						{
							case MAT_ID_SAND: MoveSand(x, y); break;
							case MAT_ID_WATER: MoveWater(x, y); break;
							case MAT_ID_STEAM: MoveSteam(x, y); break;
							case MAT_ID_WOOD: MoveWood(x, y); break;
							case MAT_ID_FIRE: MoveFire(x, y); break;
							default:
								break;
						}
					}
				}
			}
		}
	}
}

Particle& Simulator::GetParticle(int x, int y)
{
	return m_Particles[x + y * m_Width];
}

void Simulator::SetParticle(int x, int y, Particle& particle)
{
	m_Particles[x + y * m_Width] = particle;
}

void Simulator::SetParticle(int x, int y, Particle&& particle)
{
	m_Particles[x + y * m_Width] = particle;
}

void Simulator::SwapParticles(int x1, int y1, int x2, int y2, bool splashWater)
{
	Particle particle = GetParticle(x1, y1);
	Particle temp = GetParticle(x2, y2);
	particle.HasBeenUpdatedThisFrame = true;
	temp.HasBeenUpdatedThisFrame = true;
	if (splashWater && GetParticle(x2, y2).Id == MAT_ID_WATER)
	{
		int splashRate = max(4 / m_ParticleSize, 2);
		temp.IsUsingVelocity = true;
		temp.Velocity.y = splashRate;
		temp.Velocity.x = rand.Rand(0, 5) - 2;

		if (IsEmpty(x1 + 1, y1))
		{
			SetParticle(x1, y1, GetParticle(x1 + 1, y1));
			SetParticle(x1 + 1, y1, temp);
		}
		else if (IsEmpty(x1 - 1, y1))
		{
			SetParticle(x1, y1, GetParticle(x1 - 1, y1));
			SetParticle(x1 - 1, y1, temp);
		}
		else
			SetParticle(x1, y1, temp);
	}
	else
		SetParticle(x1, y1, temp);
	SetParticle(x2, y2, particle);
}

bool Simulator::IsEmpty(int x, int y)
{
	return InBounds(x, y) && GetParticle(x, y).Id == MAT_ID_EMPTY;
}

bool Simulator::InBounds(int x, int y)
{
	return (x >= 0 && x < m_Width && y >= 0 && y < m_Height);
}

bool Simulator::IsSolid(int x, int y)
{
	if (!InBounds(x, y))
		return true;
	uint8_t id = GetParticle(x, y).Id;
	return id == MAT_ID_SAND || id == MAT_ID_WOOD;
}

bool Simulator::IsGas(int x, int y)
{
	if (!InBounds(x, y))
		return true;
	uint8_t id = GetParticle(x, y).Id;
	return id == MAT_ID_STEAM || id == MAT_ID_FIRE;
}

bool Simulator::IsParticle(int x, int y, uint8_t id)
{
	return !InBounds(x, y) || GetParticle(x, y).Id == id;
}

bool Simulator::IsInLiquid(int x, int y, int* lx, int* ly)
{
	if (InBounds(x, y) && (IsParticle(x + 1, y + 1, MAT_ID_WATER) || IsParticle(x + 1, y + 1, MAT_ID_OIL)))
	{
		*lx = x; *ly = y;
		return true;
	}
	if (InBounds(x, y - 1) && (IsParticle(x + 1, y + 1, MAT_ID_WATER) || IsParticle(x + 1, y + 1, MAT_ID_OIL)))
	{
		*lx = x; *ly = y - 1;
		return true;
	}
	if (InBounds(x, y + 1) && (IsParticle(x + 1, y + 1, MAT_ID_WATER) || IsParticle(x + 1, y + 1, MAT_ID_OIL)))
	{
		*lx = x; *ly = y + 1;
		return true;
	}
	if (InBounds(x - 1, y) && (IsParticle(x + 1, y + 1, MAT_ID_WATER) || IsParticle(x + 1, y + 1, MAT_ID_OIL)))
	{
		*lx = x - 1; *ly = y;
		return true;
	}
	if (InBounds(x - 1, y - 1) && (IsParticle(x + 1, y + 1, MAT_ID_WATER) || IsParticle(x + 1, y + 1, MAT_ID_OIL)))
	{
		*lx = x - 1; *ly = y - 1;
		return true;
	}
	if (InBounds(x - 1, y + 1) && (IsParticle(x + 1, y + 1, MAT_ID_WATER) || IsParticle(x + 1, y + 1, MAT_ID_OIL)))
	{
		*lx = x - 1; *ly = y + 1;
		return true;
	}
	if (InBounds(x + 1, y) && (IsParticle(x + 1, y + 1, MAT_ID_WATER) || IsParticle(x + 1, y + 1, MAT_ID_OIL)))
	{
		*lx = x + 1; *ly = y;
		return true;
	}
	if (InBounds(x + 1, y - 1) && (IsParticle(x + 1, y + 1, MAT_ID_WATER) || IsParticle(x + 1, y + 1, MAT_ID_OIL)))
	{
		*lx = x + 1; *ly = y - 1;
		return true;
	}
	if (InBounds(x + 1, y + 1) && (IsParticle(x + 1, y + 1, MAT_ID_WATER) || IsParticle(x + 1, y + 1, MAT_ID_OIL)))
	{
		*lx = x + 1; *ly = y + 1;
		return true;
	}
	return false;
}

void Simulator::MoveVelocity(int x, int y)
{
	Particle& particle = GetParticle(x, y);
	particle.Velocity.y -= 0.5f / m_ParticleSize;
	if (particle.Velocity.y == 0)
		particle.Velocity.y -= 0.1f;
	particle.Velocity.y = max(particle.Velocity.y, min(-10 / m_ParticleSize, -1));
	particle.HasBeenUpdatedThisFrame = true;
	bool found = false;
	int newX, newY;
	for (float i = 0.1f; i <= 1.0f; i += 0.1f)
	{
		int nX = (int)(x + i * particle.Velocity.x);
		int nY = (int)(y + i * particle.Velocity.y);
		if (nX == x && nY == y)
			continue;
		if (!IsEmpty(nX, nY))
		{
			particle.IsUsingVelocity = false;
			if (particle.Velocity.y > 0)
				particle.Velocity.y = 0;
			break;
		}
		newX = nX;
		newY = nY;
		found = true;
	}
	if (found)
		SwapParticles(x, y, newX, newY);
}

void Simulator::MoveSand(int x, int y)
{
	int fallRate = max(9 / m_ParticleSize, 2);
	int spreadRate = 1;

	if (rand.Rand(0, 100) < 50)
		spreadRate = -spreadRate;

	int lx, ly;
	if (IsInLiquid(x, y, &lx, &ly))
		fallRate = 1;

	int sR = spreadRate;
	int fR = fallRate;

	Particle particle = GetParticle(x, y);
	int xTo = -1, yTo = -1;
	for (int j = 0; sR > 0 ? j <= sR : j >= sR; sR > 0 ? j++ : j--)
	{
		if (j != 0 && IsSolid(x + j, y))
			break;
		for (int i = 1; i <= fR; i++)
		{
			if (IsSolid(x + j, y - i))
				break;
			if (y - i < yTo || yTo == -1)
			{
				xTo = x + j;
				yTo = y - i;
			}
		}
		if (yTo != -1 && yTo == y - fR)
			break;
	}
	for (int j = 0; sR > 0 ? j <= sR : j >= sR; sR > 0 ? j++ : j--)
	{
		if (j != 0 && IsSolid(x - j, y))
			break;
		for (int i = 1; i <= fR; i++)
		{
			if (IsSolid(x - j, y - i))
				break;
			if (y - i < yTo || yTo == -1)
			{
				xTo = x - j;
				yTo = y - i;
			}
		}
		if (yTo != -1 && yTo == y - fR)
			break;
	}
	if (xTo != -1)
		SwapParticles(x, y, xTo, yTo, true);
}

void Simulator::MoveWater(int x, int y)
{
	int fallRate = max(10 / m_ParticleSize, 2);
	int spreadRate = max(10 / m_ParticleSize, 5);

	if (rand.Rand(0, 100) < 50)
		spreadRate = -spreadRate;

	int fR = fallRate;
	int sR = spreadRate;

	Particle particle = GetParticle(x, y);
	int xTo = -1, yTo = -1;
	for (int j = 0; sR > 0 ? j <= sR : j >= sR; sR > 0 ? j++ : j--)
	{
		if (j != 0 && (IsSolid(x + j, y) || IsParticle(x + j, y, MAT_ID_WATER) || IsGas(x + j, y)))
			break;
		for (int i = 0; i <= fR; i++)
		{
			if (i == 0 && j == 0)
				continue;
			if (IsSolid(x + j, y - i) || IsParticle(x + j, y - i, MAT_ID_WATER) || IsGas(x + j, y - i))
				break;
			if (y - i < yTo || yTo == -1)
			{
				xTo = x + j;
				yTo = y - i;
			}
		}
		if (yTo != -1 && yTo == y - fR)
			break;
	}
	for (int j = 0; sR > 0 ? j <= sR : j >= sR; sR > 0 ? j++ : j--)
	{
		if (j != 0 && (IsSolid(x - j, y) || IsParticle(x - j, y, MAT_ID_WATER) || IsGas(x - j, y)))
			break;
		for (int i = 0; i <= fR; i++)
		{
			if (i == 0 && j == 0)
				continue;
			if (IsSolid(x - j, y - i) || IsParticle(x - j, y - i, MAT_ID_WATER) || IsGas(x - j, y - i))
				break;
			if (y - i < yTo || yTo == -1)
			{
				xTo = x - j;
				yTo = y - i;
			}
		}
		if (yTo != -1 && yTo == y - fR)
			break;
	}
	if (xTo != -1)
		SwapParticles(x, y, xTo, yTo);
}

void Simulator::MoveSteam(int x, int y)
{
	//int fallRate = 2;
	int fallRate = max(2 / m_ParticleSize, 1);
	int spreadRate = 3;

	if (rand.Rand(0, 100) < 50)
		spreadRate = -spreadRate;

	int lx, ly;
	if (IsInLiquid(x, y, &lx, &ly))
		fallRate *= 2;

	int fR = fallRate;
	int sR = spreadRate;

	Particle particle = GetParticle(x, y);
	int xTo = -1, yTo = -1;
	for (int j = 0; sR > 0 ? j <= sR : j >= sR; sR > 0 ? j++ : j--)
	{
		if (j != 0 && (IsSolid(x + j, y) || IsParticle(x + j, y, MAT_ID_STEAM)))
			break;
		for (int i = 0; i <= fR; i++)
		{
			if (i == 0 && j == 0)
				continue;
			if (IsSolid(x + j, y + i) || IsParticle(x + j, y + i, MAT_ID_STEAM))
				break;
			if (y + i > yTo || yTo == -1)
			{
				xTo = x + j;
				yTo = y + i;
			}
		}
		if (yTo != -1 && yTo == y + fR)
			break;
	}
	for (int j = 0; sR > 0 ? j <= sR : j >= sR; sR > 0 ? j++ : j--)
	{
		if (j != 0 && (IsSolid(x - j, y) || IsParticle(x - j, y, MAT_ID_STEAM)))
			break;
		for (int i = 0; i <= fR; i++)
		{
			if (i == 0 && j == 0)
				continue;
			if (IsSolid(x - j, y + i) || IsParticle(x - j, y + i, MAT_ID_STEAM))
				break;
			if (y + i > yTo || yTo == -1)
			{
				xTo = x - j;
				yTo = y + i;
			}
		}
		if (yTo != -1 && yTo == y + fR)
			break;
	}
	if (xTo != -1)
		SwapParticles(x, y, xTo, yTo);
}

void Simulator::MoveWood(int x, int y)
{
	// does nothing
}

void Simulator::MoveFire(int x, int y)
{
	Particle& particle = GetParticle(x, y);
	particle.HasBeenUpdatedThisFrame = true;
	if (particle.LifeTime > 0.2f && rand.Rand(0, 100) == 0)
	{
			SetParticle(x, y, Particle());
			return;
	}
	int lx, ly;
	if (rand.Rand(0, 2) == 0 && IsInLiquid(x, y, &lx, &ly))
	{
		SetParticle(x, y, Particle());
		SetParticle(lx, ly, Particle());
		AddParticle(MAT_ID_STEAM, lx, ly);
		return;
	}

	// Kill fire underneath
	if (InBounds(x, y - 3) && IsParticle(x, y - 3, MAT_ID_FIRE) && rand.Rand(0, 100) == 0)
	{
		SetParticle(x, y - 3, particle);
		SetParticle(x, y, Particle());
		return;
	}

	// Chance to kick itself up ( to simulate flames )
	if (IsParticle(x, y - 1, MAT_ID_FIRE) && IsEmpty(x, y + 1))
	{
		if (rand.Rand(0, 10) == 0 && particle.LifeTime < 10.f && particle.LifeTime > 0.1f) {
			int r = rand.Rand(0, 2);
			int rh = rand.Rand(-10, 0);
			int spread = 3;
			for (int i = rh; i < 0; ++i) {
				for (int j = r ? -spread : spread; r ? j < spread : j > -spread; r ? ++j : --j) {
					int rx = j, ry = i;
					if (IsEmpty(x + rx, y + ry)) {
						SetParticle(x + rx, y + ry, particle);
						SetParticle(x, y, Particle());
						break;
					}
				}
			}
		}
		return;
	}
}
