#pragma once
#pragma warning( disable : 4635 )
#include "turbojpeg.h"
#pragma warning( default : 4635 )
#include "TJException.h"
#include "TJ.h"
using namespace System;
namespace turbojpegCLI
{
	/// <summary>
	/// TurboJPEG compressor
	/// </summary>
	public ref class TJCompressor
	{
	private:
		String^ NO_ASSOC_ERROR = "No source image is associated with this instance";
		tjhandle handle;
		array<Byte>^ srcBuf;
		int srcWidth = 0;
		int srcHeight = 0;
		int srcX = -1;
		int srcY = -1;
		int srcPitch = 0;
		int srcStride = 0;
		PixelFormat srcPixelFormat = (PixelFormat)-1;
		SubsamplingOption subsamp = SubsamplingOption::SAMP_420;
		int jpegQuality = 80;
		int compressedSize = 0;
		bool isDisposed = false;
		!TJCompressor();

		void checkSourceImage();
	public:

		TJCompressor();
		TJCompressor(array<Byte>^ srcImage, int width, int height);
		TJCompressor(array<Byte>^ srcImage, int width, int height, PixelFormat pixelFormat);
		TJCompressor(array<Byte>^ srcImage, int x, int y, int width, int pitch, int height, PixelFormat pixelFormat);
		~TJCompressor();

		void setSourceImage(array<Byte>^ srcImage, int width, int height);
		void setSourceImage(array<Byte>^ srcImage, int width, int height, PixelFormat pixelFormat);
		void setSourceImage(array<Byte>^ srcImage, int x, int y, int width, int pitch, int height, PixelFormat pixelFormat);

		void setSubsamp(SubsamplingOption newSubsamp);
		SubsamplingOption getSubsamp();
		void setJPEGQuality(int quality);
		int getJPEGQuality();

		void compress(array<Byte>^ %dstBuf, Flag flags);
		array<Byte>^ compress(Flag flags);
		array<Byte>^ compressToExactSize(Flag flags);
		array<Byte>^ compress();
		array<Byte>^ compressToExactSize();

		int getCompressedSize();
	};
}
