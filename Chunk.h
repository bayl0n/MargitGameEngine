#pragma once
#include "FastNoiseLite.h"

namespace Margit {
	struct ChunkLayout {
		FastNoiseLite::NoiseType noiseType;
		float noiseFrequency;
		int width;
		int height;
		int depth;
	};
	class Chunk
	{
	public:
		Chunk(ChunkLayout layout) : Layout(layout) {

		}

	protected:
		ChunkLayout Layout;
	};
}

