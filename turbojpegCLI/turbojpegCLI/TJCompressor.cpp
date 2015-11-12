#include "TJCompressor.h"
namespace turbojpegCLI
{
	/// <summary>
	/// <para>Create a TurboJPEG compressor instance that can not be used until setSourceImage(...) is called.</para>
	/// <para>The default SubsamplingOption will be SubsamplingOption.SAMP_420.</para>
	/// <para>The default JPEG Quality will be 80 (range: 1 to 100)</para>
	/// </summary>
	TJCompressor::TJCompressor()
	{
	}

	/// <summary>
	/// Call this when finished with the TJDecompressor to free any native structures.
	/// If you use a C# using() block, you won't need to call this.
	/// </summary>
	TJCompressor::~TJCompressor()
	{
		// This method appears as "Dispose()" in C#.
		if (isDisposed)
			return;
		// We would dispose of managed data here, if we had any that needed disposing.

		this->!TJCompressor();
		isDisposed = true;
	}
	TJCompressor::!TJCompressor()
	{
		// This is the Finalizer, for disposing of unmanaged data.  Managed data should not be disposed here, because managed classes may have already been garbage collected by the time this runs.
		if (tjDestroy(handle) == -1)
			throw gcnew TJException(getSystemString(tjGetErrorStr()));
		handle = 0;
	}

	/// <summary>
	/// <para>Create a TurboJPEG compressor instance and associate the uncompressed
	/// source image stored in <code>srcImage</code> with the newly created
	/// instance.</para>
	/// <para>The image is assumed to have a standard RGB pixel format.</para>
	/// <para>The default SubsamplingOption will be SubsamplingOption.SAMP_420.</para>
	/// <para>The default JPEG Quality will be 80 (range: 1 to 100)</para>
	/// </summary>
	///
	/// <param name="srcImage">image buffer containing RGB, grayscale, or CMYK pixels to
	/// be compressed or encoded.  This buffer is not modified.</param>
	///
	/// <param name="width">width (in pixels) of the region in the source image from
	/// which the JPEG or YUV image should be compressed/encoded</param>
	///
	/// <param name="height">height (in pixels) of the region in the source image from
	/// which the JPEG or YUV image should be compressed/encoded</param>
	TJCompressor::TJCompressor(array<Byte>^ srcImage, int width, int height)
	{
		setSourceImage(srcImage, width, height);
	}

	/// <summary>
	/// <para>Create a TurboJPEG compressor instance and associate the uncompressed
	/// source image stored in <code>srcImage</code> with the newly created
	/// instance.</para>
	/// <para>The image is assumed to be unpadded.</para>
	/// <para>The default SubsamplingOption will be SubsamplingOption.SAMP_420.</para>
	/// <para>The default JPEG Quality will be 80 (range: 1 to 100)</para>
	/// </summary>
	///
	/// <param name="srcImage">image buffer containing RGB, grayscale, or CMYK pixels to
	/// be compressed or encoded.  This buffer is not modified.</param>
	///
	/// <param name="width">width (in pixels) of the region in the source image from
	/// which the JPEG or YUV image should be compressed/encoded</param>
	///
	/// <param name="height">height (in pixels) of the region in the source image from
	/// which the JPEG or YUV image should be compressed/encoded</param>
	///
	/// <param name="pixelFormat">pixel format of the source image (one of the PixelFormat enum values)</param>
	TJCompressor::TJCompressor(array<Byte>^ srcImage, int width, int height, PixelFormat pixelFormat)
	{
		setSourceImage(srcImage, width, height, pixelFormat);
	}

	/// <summary>
	/// <para>Create a TurboJPEG compressor instance and associate the uncompressed
	/// source image stored in <code>srcImage</code> with the newly created
	/// instance.</para>
	/// <para>The default SubsamplingOption will be SubsamplingOption.SAMP_420.</para>
	/// <para>The default JPEG Quality will be 80 (range: 1 to 100)</para>
	/// </summary>
	///
	/// <param name="srcImage">image buffer containing RGB, grayscale, or CMYK pixels to
	/// be compressed or encoded.  This buffer is not modified.</param>
	///
	/// <param name="x">x offset (in pixels) of the region in the source image from which
	/// the JPEG or YUV image should be compressed/encoded</param>
	///
	/// <param name="y">y offset (in pixels) of the region in the source image from which
	/// the JPEG or YUV image should be compressed/encoded</param>
	///
	/// <param name="width">width (in pixels) of the region in the source image from
	/// which the JPEG or YUV image should be compressed/encoded</param>
	///
	/// <param name="pitch">bytes per line of the source image.  Normally, this should be
	/// <code>width * TJ.getPixelSize(pixelFormat)</code> if the source image is
	/// unpadded, but you can use this parameter to, for instance, specify that
	/// the scanlines in the source image are padded to a 4-byte boundary or to
	/// compress/encode a JPEG or YUV image from a region of a larger source
	/// image.  You can also be clever and use this parameter to skip lines, etc.
	/// Setting this parameter to 0 is the equivalent of setting it to
	/// <code>width * TJ.getPixelSize(pixelFormat)</code>.</param>
	///
	/// <param name="height">height (in pixels) of the region in the source image from
	/// which the JPEG or YUV image should be compressed/encoded</param>
	///
	/// <param name="pixelFormat">pixel format of the source image (one of the PixelFormat enum values)</param>
	TJCompressor::TJCompressor(array<Byte>^ srcImage, int x, int y, int width, int pitch, int height, PixelFormat pixelFormat)
	{
		setSourceImage(srcImage, x, y, width, pitch, height, pixelFormat);
	}


	/// <summary>
	/// <para>Associate an uncompressed RGB image with this compressor instance.</para>
	/// <para>The image is assumed to have a standard RGB pixel format.</para>
	/// </summary>
	///
	/// <param name="srcImage">image buffer containing RGB, grayscale, or CMYK pixels to
	/// be compressed or encoded.  This buffer is not modified.</param>
	///
	/// <param name="width">width (in pixels) of the region in the source image from
	/// which the JPEG or YUV image should be compressed/encoded</param>
	///
	/// <param name="height">height (in pixels) of the region in the source image from
	/// which the JPEG or YUV image should be compressed/encoded</param>
	void TJCompressor::setSourceImage(array<Byte>^ srcImage, int width, int height)
	{
		setSourceImage(srcImage, 0, 0, width, 0, height, PixelFormat::RGB);
	}

	/// <summary>
	/// <para>Associate an uncompressed RGB, grayscale, or CMYK source image with this
	/// compressor instance.</para>
	/// <para>The image is assumed to be unpadded.</para>
	/// </summary>
	///
	/// <param name="srcImage">image buffer containing RGB, grayscale, or CMYK pixels to
	/// be compressed or encoded.  This buffer is not modified.</param>
	///
	/// <param name="width">width (in pixels) of the region in the source image from
	/// which the JPEG or YUV image should be compressed/encoded</param>
	///
	/// <param name="height">height (in pixels) of the region in the source image from
	/// which the JPEG or YUV image should be compressed/encoded</param>
	///
	/// <param name="pixelFormat">pixel format of the source image (one of the PixelFormat enum values)</param>
	void TJCompressor::setSourceImage(array<Byte>^ srcImage, int width, int height, PixelFormat pixelFormat)
	{
		setSourceImage(srcImage, 0, 0, width, 0, height, pixelFormat);
	}

	/// <summary>
	/// Associate an uncompressed RGB, grayscale, or CMYK source image with this
	/// compressor instance.
	/// </summary>
	///
	/// <param name="srcImage">image buffer containing RGB, grayscale, or CMYK pixels to
	/// be compressed or encoded.  This buffer is not modified.</param>
	///
	/// <param name="x">x offset (in pixels) of the region in the source image from which
	/// the JPEG or YUV image should be compressed/encoded</param>
	///
	/// <param name="y">y offset (in pixels) of the region in the source image from which
	/// the JPEG or YUV image should be compressed/encoded</param>
	///
	/// <param name="width">width (in pixels) of the region in the source image from
	/// which the JPEG or YUV image should be compressed/encoded</param>
	///
	/// <param name="pitch">bytes per line of the source image.  Normally, this should be
	/// <code>width * TJ.getPixelSize(pixelFormat)</code> if the source image is
	/// unpadded, but you can use this parameter to, for instance, specify that
	/// the scanlines in the source image are padded to a 4-byte boundary or to
	/// compress/encode a JPEG or YUV image from a region of a larger source
	/// image.  You can also be clever and use this parameter to skip lines, etc.
	/// Setting this parameter to 0 is the equivalent of setting it to
	/// <code>width * TJ.getPixelSize(pixelFormat)</code>.</param>
	///
	/// <param name="height">height (in pixels) of the region in the source image from
	/// which the JPEG or YUV image should be compressed/encoded</param>
	///
	/// <param name="pixelFormat">pixel format of the source image (one of the PixelFormat enum values)</param>
	void TJCompressor::setSourceImage(array<Byte>^ srcImage, int x, int y, int width, int pitch, int height, PixelFormat pixelFormat)
	{
		if (handle == 0)
		{
			handle = tjInitCompress();
			if (handle == nullptr)
				throw gcnew TJException(getSystemString(tjGetErrorStr()));
		}
		TJ::checkPixelFormat(pixelFormat);
		if (srcImage == nullptr || x < 0 || y < 0 || width < 1 || height < 1 || pitch < 0)
			throw gcnew ArgumentException("Invalid argument in setSourceImage()");
		srcBuf = srcImage;
		srcWidth = width;
		if (pitch == 0)
			srcPitch = width * TJ::getPixelSize(pixelFormat);
		else
			srcPitch = pitch;
		srcHeight = height;
		srcPixelFormat = pixelFormat;
		srcX = x;
		srcY = y;
	}

	/// <summary>
	/// Set the level of chrominance subsampling for subsequent compress/encode
	/// operations.  When pixels are converted from RGB to YCbCr (see
	/// Colorspace.YCbCr) or from CMYK to YCCK (see Colorspace.YCCK) as part
	/// of the JPEG compression process, some of the Cb and Cr (chrominance)
	/// components can be discarded or averaged together to produce a smaller
	/// image with little perceptible loss of image clarity (the human eye is more
	/// sensitive to small changes in brightness than to small changes in color.)
	/// This is called "chrominance subsampling".
	/// </summary>
	///
	/// <param name="newSubsamp">the level of chrominance subsampling to use in
	/// subsequent compress/encode oeprations (one of the
	/// SubsamplingOption enum values)</param>
	void TJCompressor::setSubsamp(SubsamplingOption newSubsamp)
	{
		TJ::checkSubsampling(newSubsamp);
		subsamp = newSubsamp;
	}

	/// <summary>
	/// <para>Get the level of chrominance subsampling being used for subsequent compress/encode
	/// operations.</para>
	/// <para>Default value if unset: SubsamplingOption.SAMP_420</para>
	/// </summary>
	/// <returns>the level of chrominance subsampling being used for subsequent compress/encode
	/// operations. Default value if unset: SubsamplingOption.SAMP_420</returns>
	SubsamplingOption TJCompressor::getSubsamp()
	{
		return subsamp;
	}

	/// <summary>
	/// Set the JPEG image quality level for subsequent compress operations.
	/// </summary>
	///
	/// <param name="quality">the new JPEG image quality level (1 to 100, 1 = worst,
	/// 100 = best)</param>
	void TJCompressor::setJPEGQuality(int quality)
	{
		if (quality < 1 || quality > 100)
			throw gcnew ArgumentException("Invalid argument in setJPEGQuality()");
		jpegQuality = quality;
	}

	/// <summary>
	/// Get the JPEG image quality level being used for subsequent compress operations. (1 to 100, 1 = worst,
	/// 100 = best)  Default value if unset: 80
	/// </summary>
	///
	/// <returns>the JPEG image quality level (1 to 100, 1 = worst,
	/// 100 = best)</returns>
	int TJCompressor::getJPEGQuality()
	{
		return jpegQuality;
	}

	/// <summary>
	/// Compress the uncompressed source image associated with this compressor
	/// instance and output a JPEG image to the given destination buffer.
	/// </summary>
	///
	/// <param name="dstBuf">buffer that will receive the JPEG image.  Use
	/// TJ.bufSize() to determine the maximum size for this buffer based on
	/// the source image's width and height and the desired level of chrominance
	/// subsampling. Although it is unlikely, libjpeg-turbo may replace this 
	/// buffer so it must be passed by reference.</param>
	///
	/// <param name="flags">the bitwise OR of one or more of the Flag enum values</param>
	void TJCompressor::compress(array<Byte>^ %dstBuf, Flag flags)
	{
		if (dstBuf == nullptr || (int)flags < 0)
			throw gcnew Exception("Invalid argument in compress()");
		if (srcBuf == nullptr)
			throw gcnew Exception(NO_ASSOC_ERROR);
		if (jpegQuality < 0)
			throw gcnew Exception("JPEG Quality not set");
		TJ::checkSubsampling(subsamp);
		TJ::checkPixelFormat(srcPixelFormat);
		if (srcWidth < 1 || srcHeight < 1 || srcPitch < 0)
			throw gcnew Exception("Invalid width, height, or pitch.");

		int actualPitch = (srcPitch == 0) ? srcWidth * tjPixelSize[(int)srcPixelFormat] : srcPitch;
		int arraySize = (srcY + srcHeight - 1) * actualPitch + (srcX + srcWidth) * tjPixelSize[(int)srcPixelFormat];
		if (srcBuf->Length < arraySize)
			throw gcnew Exception("Source buffer is not large enough");

		unsigned long jpegSize = TJ::bufSize(srcWidth, srcHeight, subsamp);
		if (dstBuf->Length < (int)jpegSize)
			throw gcnew Exception("Destination buffer is not large enough");

		pin_ptr<Byte> pinnedInput = &srcBuf[0];
		pin_ptr<Byte> pinnedOutput = &dstBuf[0];
		unsigned char* outputBuf = pinnedOutput;
		// I am not sure what this is for, but it does not seem to be needed.
		//if (ProcessSystemProperties() < 0)
		//	throw gcnew Exception("Setting system properties failed");

		if (tjCompress2(handle, &pinnedInput[srcY * actualPitch + srcX * tjPixelSize[(int)srcPixelFormat]], srcWidth, srcPitch, srcHeight, (int)srcPixelFormat, &outputBuf, &jpegSize, (int)subsamp, jpegQuality, (int)flags | TJFLAG_NOREALLOC)
			== -1)
			throw gcnew TJException("tjCompress2 failed");
		// outputBuf may be pointing at a new char* now.  If it is, we need to make sure dstBuf gets updated.
		if (outputBuf != pinnedOutput)
		{
			dstBuf = gcnew array<Byte>((int)jpegSize);
			System::Runtime::InteropServices::Marshal::Copy((IntPtr)outputBuf, dstBuf, 0, (int)jpegSize);
		}
		compressedSize = jpegSize;
	}

	/// <summary>
	/// Compress the uncompressed source image associated with this compressor
	/// instance and return a buffer containing a JPEG image. The length of this 
	/// buffer will not be equal to the size of the JPEG image. Use 
	/// getCompressedSize() to obtain the size of the JPEG image.
	/// </summary>
	///
	/// <param name="flags">the bitwise OR of one or more of the Flag enum values</param>
	///
	/// <returns>a buffer containing a JPEG image.  The length of this buffer will
	/// not be equal to the size of the JPEG image.  Use getCompressedSize() to
	/// obtain the size of the JPEG image.</returns>
	array<Byte>^ TJCompressor::compress(Flag flags)
	{
		checkSourceImage();
		array<Byte>^ buf = gcnew array<Byte>(TJ::bufSize(srcWidth, srcHeight, subsamp));
		compress(buf, flags);
		return buf;
	}

	/// <summary>
	/// Compress the uncompressed source image associated with this compressor
	/// instance and return a buffer containing a JPEG image. The length of this 
	/// buffer will not be equal to the size of the JPEG image. Use 
	/// getCompressedSize() to obtain the size of the JPEG image.
	/// </summary>
	/// <returns>a buffer containing a JPEG image.  The length of this buffer will
	/// not be equal to the size of the JPEG image.  Use getCompressedSize() to
	/// obtain the size of the JPEG image.</returns>
	array<Byte>^ TJCompressor::compress()
	{
		return compress(Flag::NONE);
	}

	/// <summary>
	/// Compress the uncompressed source image associated with this compressor
	/// instance and return a buffer containing a JPEG image. This method copies
	/// the compressed data to a new array of the appropriate size, so you do not
	/// have to call getCompressedSize() or deal with passing around the actual
	/// compressed length separately from the byte array.  Consequently, this method
	/// is a little less efficient than the other compress methods, and it creates
	/// more garbage for the garbage collector.
	/// </summary>
	///
	/// <param name="flags">the bitwise OR of one or more of the Flag enum values</param>
	///
	/// <returns>a buffer containing a JPEG image. The length of this buffer will be 
	/// equal to the size of the JPEG image.</returns>
	array<Byte>^ TJCompressor::compressToExactSize(Flag flags)
	{
		checkSourceImage();
		array<Byte>^ buf = gcnew array<Byte>(TJ::bufSize(srcWidth, srcHeight, subsamp));
		compress(buf, flags);
		if (buf->Length == compressedSize)
			return buf;
		array<Byte>^ exactSizeBuf = gcnew array<Byte>(compressedSize);
		Array::Copy(buf, exactSizeBuf, compressedSize);
		return exactSizeBuf;
	}

	/// <summary>
	/// Compress the uncompressed source image associated with this compressor
	/// instance and return a buffer containing a JPEG image. This method copies
	/// the compressed data to a new array of the appropriate size, so you do not
	/// have to call getCompressedSize() or deal with passing around the actual
	/// compressed length separately from the byte array.  Consequently, this method
	/// is a little less efficient than the other compress methods, and it creates
	/// more garbage for the garbage collector.
	/// </summary>
	/// <returns>a buffer containing a JPEG image. The length of this buffer will be 
	/// equal to the size of the JPEG image.</returns>
	array<Byte>^ TJCompressor::compressToExactSize()
	{
		return compressToExactSize(Flag::NONE);
	}


	/// <summary>
	/// Returns the size of the image (in bytes) generated by the most recent
	/// compress operation.
	/// </summary>
	///
	/// <returns>the size of the image (in bytes) generated by the most recent
	/// compress operation.</returns>
	int TJCompressor::getCompressedSize()
	{
		return compressedSize;
	}

	void TJCompressor::checkSourceImage()
	{
		if (srcWidth < 1 || srcHeight < 1)
			throw gcnew Exception(NO_ASSOC_ERROR);
		TJ::checkSubsampling(subsamp);
	}
}