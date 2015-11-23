#pragma once
using namespace System;

namespace turbojpegCLI
{
	public ref class TJScalingFactor
	{
		/// <summary>
		/// Numerator
		/// </summary>
		int num;
		/// <summary>
		/// Denominator
		/// </summary>
		int denom;
	public:
		TJScalingFactor(int num, int denom)
		{
			if (num < 1 || denom < 1)
				throw gcnew ArgumentException("Numerator and denominator must be >= 1");
			this->num = num;
			this->denom = denom;
		}
		/// <summary>
		/// Returns numerator
		/// </summary>
		int getNum()
		{
			return num;
		}
		/// <summary>
		/// Returns denominator
		/// </summary>
		int getDenom()
		{
			return denom;
		}
		/// <summary>
		/// Returns the scaled value of <code>dimension</code>.  This function
		/// performs the integer equivalent of
		/// <code>ceil(dimension * scalingFactor)</code>.
		/// </summary>
		///
		/// <returns>the scaled value of <code>dimension</code>.</returns>
		int getScaled(int dimension)
		{
			return (dimension * num + denom - 1) / denom;
		}

		/// <summary>
		/// Returns true or false, depending on whether this instance and
		/// <code>other</code> have the same numerator and denominator.
		/// </summary>
		///
		/// <returns>true or false, depending on whether this instance and
		/// <code>other</code> have the same numerator and denominator.</returns>
		bool equals(TJScalingFactor other)
		{
			return this->num == other.num && this->denom == other.denom;
		}

		/// <summary>
		/// Returns true or false, depending on whether this instance is equal to
		/// 1/1.
		/// </summary>
		///
		/// <returns>true or false, depending on whether this instance is equal to
		/// 1/1.</returns>
		bool isOne()
		{
			return num == 1 && denom == 1;
		}
	};
}
