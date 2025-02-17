#pragma once
#include "FastNoiseLite.h"
#include <vector>

namespace Margit {
	struct ChunkLayout {
		int x;
		int y;
		int z;
		int width;
		int depth;
		int height;
	};

	class Chunk
	{
	public:
		Chunk(ChunkLayout layout) : Layout(layout) {
			for (int chunkWidth = 0; chunkWidth < Layout.width; chunkWidth++) {
				for (int chunkDepth = 0; chunkDepth < Layout.depth; chunkDepth++) {
					for (int chunkHeight = 0; chunkHeight < Layout.height; chunkHeight++) {
						ChunkData[chunkWidth][chunkDepth][chunkHeight] =
					}
				}
			}
		}

		/*
		remap
		-----
			Takes a value and remaps it from an old range to a new range, such that
			it is proportional to the old range.
		*/
		static float remap(
			float value,
			float oldStart,
			float oldEnd,
			float newStart,
			float newEnd
		) {
			return (value - oldStart) / (oldEnd - oldStart) * (newEnd - newStart) + newStart;
		}

		void generateHeights(FastNoiseLite noise) const {
			for (int chunkWidth = 0; chunkWidth < Layout.width; chunkWidth++) {
				for (int chunkDepth = 0; chunkDepth < Layout.height; chunkDepth++) {
					float noiseValue = noise.GetNoise((float)chunkWidth, (float)chunkDepth);
					float chunkPillarHeight = remap(noiseValue, -1.0f, 1.0f, 0, 15);

					for (int chunkHeight = 0; chunkHeight < chunkPillarHeight; chunkHeight++) {
						// Chunk logic
					}
				}
			}
		}

	protected:
		ChunkLayout Layout;
		std::vector<std::vector<std::vector<float>>> ChunkData;
	};
}

