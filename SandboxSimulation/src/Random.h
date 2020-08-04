#pragma once

#include <chrono>
#include <random>

class Random
{
public:
	Random()
	{
        //std::random_device rd;
        seed = rd() ^ (
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
                ).count() +
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()
                ).count());

       gen = std::mt19937(seed);
	}

    int Rand(int min, int max)
    {
        std::uniform_int_distribution<unsigned> distrib(min, max - 1);
        return distrib(gen);
    }
private:
	std::random_device rd;
	std::mt19937::result_type seed;
	std::mt19937 gen;
};