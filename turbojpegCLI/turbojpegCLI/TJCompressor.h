#pragma once
#pragma warning( disable : 4635 )
#include "turbojpeg.h"
#pragma warning( default : 4635 )
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
		String^ NO_ASSOC_ERROR;
		tjhandle handle;
		array<Byte>^ srcBuf;
		int srcWidth;
		int srcHeight;
		int srcX;
		int srcY;
		int srcPitch;
		int srcStride;
		PixelFormat srcPixelFormat;
		SubsamplingOption subsamp;
		int jpegQuality;
		int compressedSize;
		bool isDisposed;
		!TJCompressor();
		void Initialize()
		{
			NO_ASSOC_ERROR = "No source image is associated with this instance";
			handle = 0;
			srcWidth = 0;
			srcHeight = 0;
			srcX = -1;
			srcY = -1;
			srcPitch = 0;
			srcStride = 0;
			srcPixelFormat = (PixelFormat)-1;
			subsamp = SubsamplingOption::SAMP_420;
			jpegQuality = 80;
			compressedSize = 0;
			isDisposed = false;
		}

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
