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
	/// TurboJPEG decompressor
	/// </summary>
	public ref class TJDecompressor
	{
	private:
		String^ NO_ASSOC_ERROR = "No JPEG image is associated with this instance";
		tjhandle handle;
		array<Byte>^ jpegBuf;
		int jpegBufSize = 0;
		int jpegWidth = 0;
		int jpegHeight = 0;
		SubsamplingOption jpegSubsamp = (SubsamplingOption)-1;
		Colorspace jpegColorspace = (Colorspace)-1;
		bool isDisposed = false;
		!TJDecompressor();
	public:

		TJDecompressor();
		TJDecompressor(array<Byte>^ jpegImage);
		TJDecompressor(array<Byte>^ jpegImage, int imageSize);
		~TJDecompressor();

		void setSourceImage(array<Byte>^ jpegImage, int imageSize);

		int getWidth();
		int getHeight();
		SubsamplingOption getSubsamp();
		Colorspace getColorspace();
		array<Byte>^ getJPEGBuf();
		int getJPEGSize();

		int getScaledWidth(int desiredWidth, int desiredHeight);
		int getScaledHeight(int desiredWidth, int desiredHeight);

		void decompress(array<Byte>^ dstBuf, int x, int y, int desiredWidth, int pitch, int desiredHeight, PixelFormat pixelFormat, Flag flags);
		void decompress(array<Byte>^ dstBuf, PixelFormat pixelFormat, Flag flags);
		void decompress(array<Byte>^ dstBuf);

		array<Byte>^ decompress(int desiredWidth, int pitch, int desiredHeight, PixelFormat pixelFormat, Flag flags);
		array<Byte>^ decompress(PixelFormat pixelFormat, Flag flags);
		array<Byte>^ decompress();
	};
}
