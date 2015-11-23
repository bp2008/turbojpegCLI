#pragma once
#pragma warning( disable : 4635 )
#include "turbojpeg.h"
#pragma warning( default : 4635 )
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
		String^ NO_ASSOC_ERROR;
		tjhandle handle;
		array<Byte>^ jpegBuf;
		int jpegBufSize;
		int jpegWidth;
		int jpegHeight;
		SubsamplingOption jpegSubsamp;
		Colorspace jpegColorspace;
		bool isDisposed;
		!TJDecompressor();

		void Initialize()
		{
			NO_ASSOC_ERROR = "No JPEG image is associated with this instance";
			handle = 0;
			jpegBufSize = 0;
			jpegWidth = 0;
			jpegHeight = 0;
			jpegSubsamp = (SubsamplingOption)-1;
			jpegColorspace = (Colorspace)-1;
			isDisposed = false;
		}
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
