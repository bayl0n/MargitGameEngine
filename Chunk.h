#pragma once
#include "FastNoiseLite.h"

namespace Margit {
	struct ChunkLayout {
		int x;
		int y;
		int z;
		int width;
		int height;
		int depth;
	};

	class Chunk
	{
	public:
		Chunk(ChunkLayout layout) : Layout(layout) {

		}

		/*
		remap
		-----
			Takes a value and remaps it from an old range to a new range, such that
			it is proportional to the old range.

			e.g.

			value = 5
			old range = 0 to 10
			new range = 15 to 20

			5 is halfway of 0 to 10

			so return halfway between 15 to 20

			output = 17.5
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

		void generateHeights(FastNoiseLite noise) {
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
	};
}

