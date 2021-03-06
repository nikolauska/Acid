﻿//
// MIT License
//
// Copyright(c) 2017 Jordan Peck
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#pragma once

#include <cstdint>
#include <memory>
#include "Engine/Exports.hpp"

namespace acid
{
	enum NoiseType
	{
		TYPE_VALUE = 0,
		TYPE_VALUEFRACTAL = 1,
		TYPE_PERLIN = 2,
		TYPE_PERLINFRACTAL = 3,
		TYPE_SIMPLEX = 4,
		TYPE_SIMPLEXFRACTAL = 5,
		TYPE_CELLULAR = 6,
		TYPE_WHITENOISE = 7,
		TYPE_CUBIC = 8,
		TYPE_CUBICFRACTAL = 9
	};

	enum NoiseInterp
	{
		INTERP_LINEAR = 0,
		INTERP_HERMITE = 1,
		INTERP_QUINTIC = 2
	};

	enum NoiseFractal
	{
		FRACTAL_FBM = 0,
		FRACTAL_BILLOW = 1,
		FRACTAL_RIGIDMULTI = 2
	};

	enum NoiseCellularFunc
	{
		CELLULAR_EUCLIDEAN = 0,
		CELLULAR_MANHATTAN = 1,
		CELLULAR_NATURAL = 2
	};

	enum NoiseCellularReturn
	{
		CELLULAR_CELLVALUE = 0,
		CELLULAR_NOISELOOKUP = 1,
		CELLULAR_DISTANCE = 2,
		CELLULAR_DISTANCE2 = 3,
		CELLULAR_DISTANCE2ADD = 4,
		CELLULAR_DISTANCE2SUB = 5,
		CELLULAR_DISTANCE2MUL = 6,
		CELLULAR_DISTANCE2DIV = 7
	};

	class ACID_EXPORT Noise
	{
	private:
		int32_t m_seed;
		std::unique_ptr<uint8_t[]> m_perm;
		std::unique_ptr<uint8_t[]> m_perm12;

		float m_frequency;
		NoiseInterp m_interp;
		NoiseType m_noiseType;

		int32_t m_octaves;
		float m_lacunarity;
		float m_gain;
		NoiseFractal m_fractalType;
		float m_fractalBounding;

		NoiseCellularFunc m_cellularDistanceFunction;
		NoiseCellularReturn m_cellularReturnType;
		std::unique_ptr<Noise> m_cellularNoiseLookup;
		int32_t m_cellularDistanceIndex0;
		int32_t m_cellularDistanceIndex1;
		float m_cellularJitter;

		float m_gradientPerturbAmp;
	public:
		Noise(const int32_t &seed);

		~Noise();

		// Returns seed used for all noise types
		int32_t GetSeed() const { return m_seed; }

		// Sets seed used for all noise types
		// Default: 1337
		void SetSeed(const int32_t &seed);

		// Returns frequency used for all noise types
		float GetFrequency() const { return m_frequency; }

		// Sets frequency for all noise types
		// Default: 0.01
		void SetFrequency(const float &frequency) { m_frequency = frequency; }

		// Returns interpolation method used for supported noise types
		NoiseInterp GetInterp() const { return m_interp; }

		// Changes the interpolation method used to smooth between noise values
		// Possible interpolation methods (lowest to highest quality) :
		// - Linear
		// - Hermite
		// - Quintic
		// Used in Value, Perlin Noise and Position Warping
		// Default: Quintic
		void SetInterp(const NoiseInterp &interp) { m_interp = interp; }

		// Returns the noise type used by GetNoise
		NoiseType GetNoiseType() const { return m_noiseType; }

		// Sets noise return type of GetNoise(...)
		// Default: Simplex
		void SetNoiseType(const NoiseType &noiseType) { m_noiseType = noiseType; }

		// Returns octave count for all fractal noise types
		int32_t GetFractalOctaves() const { return m_octaves; }

		// Sets octave count for all fractal noise types
		// Default: 3
		void SetFractalOctaves(const int32_t &octaves);

		// Returns octave lacunarity for all fractal noise types
		float GetFractalLacunarity() const { return m_lacunarity; }

		// Sets octave lacunarity for all fractal noise types
		// Default: 2.0
		void SetFractalLacunarity(const float &lacunarity) { m_lacunarity = lacunarity; }

		// Returns octave gain for all fractal noise types
		float GetFractalGain() const { return m_gain; }

		// Sets octave gain for all fractal noise types
		// Default: 0.5
		void SetFractalGain(const float &gain);

		// Returns method for combining octaves in all fractal noise types
		NoiseFractal GetFractalType() const { return m_fractalType; }

		// Sets method for combining octaves in all fractal noise types
		// Default: FBM
		void SetFractalType(const NoiseFractal &fractalType) { m_fractalType = fractalType; }

		// Returns the distance function used in cellular noise calculations
		NoiseCellularFunc GetCellularDistanceFunction() const { return m_cellularDistanceFunction; }

		// Sets distance function used in cellular noise calculations
		// Default: Euclidean
		void SetCellularDistanceFunction(const NoiseCellularFunc &cellularDistanceFunction) { m_cellularDistanceFunction = cellularDistanceFunction; }

		// Returns the return type from cellular noise calculations
		NoiseCellularReturn GetCellularReturnType() const { return m_cellularReturnType; }

		// Sets return type from cellular noise calculations
		// Note: NoiseLookup requires another FastNoise object be set with SetCellularNoiseLookup() to function
		// Default: CellValue
		void SetCellularReturnType(const NoiseCellularReturn &cellularReturnType) { m_cellularReturnType = cellularReturnType; }

		// Returns the noise used to calculate a cell value if the cellular return type is NoiseLookup
		std::unique_ptr<Noise> const &GetCellularNoiseLookup() const { return m_cellularNoiseLookup; }

		// Noise used to calculate a cell value if cellular return type is NoiseLookup
		// The lookup value is acquired through GetNoise() so ensure you SetNoiseType() on the noise lookup, value, Perlin or simplex is recommended
		void SetCellularNoiseLookup(Noise *noise) { m_cellularNoiseLookup.reset(noise); }

		// Returns the 2 distance indices used for distance2 return types
		void GetCellularDistance2Indices(int32_t &cellularDistanceIndex0, int32_t &cellularDistanceIndex1) const;

		// Sets the 2 distance indices used for distance2 return types
		// Default: 0, 1
		// Note: index0 should be lower than index1
		// Both indices must be >= 0, index1 must be < 4
		void SetCellularDistance2Indices(const int32_t &cellularDistanceIndex0, const int32_t &cellularDistanceIndex1);

		// Returns the maximum distance a cellular point32_t can move from its grid position
		float GetCellularJitter() const { return m_cellularJitter; }

		// Sets the maximum distance a cellular point32_t can move from its grid position
		// Setting this high will make artifacts more common
		// Default: 0.45
		void SetCellularJitter(const float &cellularJitter) { m_cellularJitter = cellularJitter; }

		// Returns the maximum warp distance from original location when using GradientPerturb{Fractal}(...)
		float GetGradientPerturbAmp() const { return m_gradientPerturbAmp; }

		// Sets the maximum warp distance from original location when using GradientPerturb{Fractal}(...)
		// Default: 1.0
		void SetGradientPerturbAmp(const float &gradientPerturbAmp) { m_gradientPerturbAmp = gradientPerturbAmp; }

		//2D
		float GetValue(float x, float y) const;

		float GetValueFractal(float x, float y) const;

		float GetPerlin(float x, float y) const;

		float GetPerlinFractal(float x, float y) const;

		float GetSimplex(float x, float y) const;

		float GetSimplexFractal(float x, float y) const;

		float GetCellular(float x, float y) const;

		float GetWhiteNoise(float x, float y) const;

		float GetWhiteNoiseInt(int32_t x, int32_t y) const;

		float GetCubic(float x, float y) const;

		float GetCubicFractal(float x, float y) const;

		float GetNoise(float x, float y) const;

		void GradientPerturb(float &x, float &y) const;

		void GradientPerturbFractal(float &x, float &y) const;

		//3D
		float GetValue(float x, float y, float z) const;

		float GetValueFractal(float x, float y, float z) const;

		float GetPerlin(float x, float y, float z) const;

		float GetPerlinFractal(float x, float y, float z) const;

		float GetSimplex(float x, float y, float z) const;

		float GetSimplexFractal(float x, float y, float z) const;

		float GetCellular(float x, float y, float z) const;

		float GetWhiteNoise(float x, float y, float z) const;

		float GetWhiteNoiseInt(int32_t x, int32_t y, int32_t z) const;

		float GetCubic(float x, float y, float z) const;

		float GetCubicFractal(float x, float y, float z) const;

		float GetNoise(float x, float y, float z) const;

		void GradientPerturb(float &x, float &y, float &z) const;

		void GradientPerturbFractal(float &x, float &y, float &z) const;

		//4D
		float GetSimplex(float x, float y, float z, float w) const;

		float GetWhiteNoise(float x, float y, float z, float w) const;

		float GetWhiteNoiseInt(int32_t x, int32_t y, int32_t z, int32_t w) const;

	private:
		void CalculateFractalBounding();

		// Helpers
		static int32_t FastFloor(const float &f);

		static int32_t FastRound(const float &f);

		static float Lerp(const float &a, const float &b, const float &t);

		static float InterpHermite(const float &t);

		static float InterpQuintic(const float &t);

		static float CubicLerp(const float &a, const float &b, const float &c, const float &d, const float &t);

		uint8_t Index2d12(const uint8_t &offset, const int32_t &x, const int32_t &y) const;

		uint8_t Index3d12(const uint8_t &offset, const int32_t &x, const int32_t &y, const int32_t &z) const;

		uint8_t Index4d32(const uint8_t &offset, const int32_t &x, const int32_t &y, const int32_t &z, const int32_t &w) const;

		uint8_t Index2d256(const uint8_t &offset, const int32_t &x, const int32_t &y) const;

		uint8_t Index3d256(const uint8_t &offset, const int32_t &x, const int32_t &y, const int32_t &z) const;

		uint8_t Index4d256(const uint8_t &offset, const int32_t &x, const int32_t &y, const int32_t &z, const int32_t &w) const;

		static float ValueCoord2d(const int32_t &seed, const int32_t &x, const int32_t &y);

		static float ValueCoord3d(const int32_t &seed, const int32_t &x, const int32_t &y, const int32_t &z);

		static float ValueCoord4d(const int32_t &seed, const int32_t &x, const int32_t &y, const int32_t &z, const int32_t &w);

		float ValueCoord2dFast(const uint8_t &offset, const int32_t &x, const int32_t &y) const;

		float ValueCoord3dFast(const uint8_t &offset, const int32_t &x, const int32_t &y, const int32_t &z) const;

		float GradCoord2d(const uint8_t &offset, const int32_t &x, const int32_t &y, const float &xd, const float &yd) const;

		float GradCoord3d(const uint8_t &offset, const int32_t &x, const int32_t &y, const int32_t &z, const float &xd, const float &yd, const float &zd) const;

		float GradCoord4d(const uint8_t &offset, const int32_t &x, const int32_t &y, const int32_t &z, const int32_t &w, const float &xd, const float &yd, const float &zd, const float &wd) const;

		// 2D
		float SingleValueFractalFbm(float x, float y) const;

		float SingleValueFractalBillow(float x, float y) const;

		float SingleValueFractalRigidMulti(float x, float y) const;

		float SingleValue(const uint8_t &offset, const float &x, const float &y) const;

		float SinglePerlinFractalFbm(float x, float y) const;

		float SinglePerlinFractalBillow(float x, float y) const;

		float SinglePerlinFractalRigidMulti(float x, float y) const;

		float SinglePerlin(const uint8_t &offset, const float &x, const float &y) const;

		float SingleSimplexFractalFbm(float x, float y) const;

		float SingleSimplexFractalBillow(float x, float y) const;

		float SingleSimplexFractalRigidMulti(float x, float y) const;

		float singleSimplexFractalBlend(float x, float y) const;

		float SingleSimplex(const uint8_t &offset, const float &x, const float &y) const;

		float SingleCubicFractalFbm(float x, float y) const;

		float SingleCubicFractalBillow(float x, float y) const;

		float SingleCubicFractalRigidMulti(float x, float y) const;

		float SingleCubic(const uint8_t &offset, const float &x, const float &y) const;

		float SingleCellular(const float &x, const float &y) const;

		float SingleCellular2Edge(const float &x, const float &y) const;

		void SingleGradientPerturb(const uint8_t &offset, const float &warpAmp, const float &frequency, float x, float y) const;

		//3D
		float SingleValueFractalFbm(float x, float y, float z) const;

		float SingleValueFractalBillow(float x, float y, float z) const;

		float SingleValueFractalRigidMulti(float x, float y, float z) const;

		float SingleValue(const uint8_t &offset, const float &x, const float &y, const float &z) const;

		float SinglePerlinFractalFbm(float x, float y, float z) const;

		float SinglePerlinFractalBillow(float x, float y, float z) const;

		float SinglePerlinFractalRigidMulti(float x, float y, float z) const;

		float SinglePerlin(const uint8_t &offset, const float &x, const float &y, const float &z) const;

		float SingleSimplexFractalFbm(float x, float y, float z) const;

		float SingleSimplexFractalBillow(float x, float y, float z) const;

		float SingleSimplexFractalRigidMulti(float x, float y, float z) const;

		float SingleSimplex(const uint8_t &offset, const float &x, const float &y, const float &z) const;

		float SingleCubicFractalFbm(float x, float y, float z) const;

		float SingleCubicFractalBillow(float x, float y, float z) const;

		float SingleCubicFractalRigidMulti(float x, float y, float z) const;

		float SingleCubic(const uint8_t &offset, const float &x, const float &y, const float &z) const;

		float SingleCellular(const float &x, const float &y, const float &z) const;

		float SingleCellular2Edge(const float &x, const float &y, const float &z) const;

		void SingleGradientPerturb(const uint8_t &offset, const float &warpAmp, const float &frequency, float x, float y, float z) const;

		//4D
		float SingleSimplex(const uint8_t &offset, const float &x, const float &y, const float &z, const float &w) const;
	};
}
