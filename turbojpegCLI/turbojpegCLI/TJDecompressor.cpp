#include "TJDecompressor.h"
#include "TJException.h"

namespace turbojpegCLI
{
	/// <summary>
	/// Constructs a TJDecompressor which is responsible for decompressing a jpeg image.
	/// When using the parameterless constructor, you must call setSourceImage().
	/// </summary>
	TJDecompressor::TJDecompressor()
	{
		Initialize();
		handle = tjInitDecompress();
		if (handle == nullptr)
			throw gcnew TJException(getSystemString(tjGetErrorStr()));
	}
	/// <summary>
	/// Constructs a TJDecompressor which is responsible for decompressing a jpeg image.
	/// </summary>
	/// <param name="jpegImage">A byte array containing compressed jpeg image data.</param>
	TJDecompressor::TJDecompressor(array<Byte>^ jpegImage)
	{
		Initialize();
		handle = tjInitDecompress();
		if (handle == nullptr)
			throw gcnew TJException(getSystemString(tjGetErrorStr()));
		setSourceImage(jpegImage, jpegImage->Length);
	}
	/// <summary>
	/// Constructs a TJDecompressor which is responsible for decompressing a jpeg image.
	/// </summary>
	/// <param name="jpegImage">A byte array containing compressed jpeg image data.</param>
	/// <param name="imageSize">The length of the image data in the array.</param>
	TJDecompressor::TJDecompressor(array<Byte>^ jpegImage, int imageSize)
	{
		Initialize();
		handle = tjInitDecompress();
		if (handle == nullptr)
			throw gcnew TJException(getSystemString(tjGetErrorStr()));
		setSourceImage(jpegImage, imageSize);
	}

	/// <summary>
	/// Call this when finished with the TJDecompressor to free any native structures.
	/// If you use a C# using() block, you won't need to call this.
	/// </summary>
	TJDecompressor::~TJDecompressor()
	{
		// This method appears as "Dispose()" in C#.
		if (isDisposed)
			return;
		// We would dispose of managed data here, if we had any that needed disposing.

		this->!TJDecompressor();
		isDisposed = true;
	}
	TJDecompressor::!TJDecompressor()
	{
		// This is the Finalizer, for disposing of unmanaged data.  Managed data should not be disposed here, because managed classes may have already been garbage collected by the time this runs.
		if (handle != 0 && tjDestroy(handle) == -1)
			throw gcnew TJException(getSystemString(tjGetErrorStr()));
		handle = 0;
	}

	/// <summary>
	/// Associate the JPEG image of length <code>imageSize</code> bytes stored in
	/// <code>jpegImage</code> with this decompressor instance. This image will
	/// be used as the source image for subsequent decompress operations.
	/// </summary>
	/// <param name="jpegImage">A byte array containing compressed jpeg image data.</param>
	/// <param name="imageSize">The length of the image data in the array.</param>
	void TJDecompressor::setSourceImage(array<Byte>^ jpegImage, int imageSize)
	{
		if (jpegImage == nullptr || imageSize < 1)
			throw gcnew ArgumentException("Invalid argument in setSourceImage()");
		jpegBuf = jpegImage;
		jpegBufSize = imageSize;

		if (jpegImage->Length < imageSize)
			throw gcnew Exception("Source buffer is not large enough");

		// We must pin each item we are passing a pointer to.
		pin_ptr<Byte> pinnedJpegImage = &jpegImage[0]; // Pin the array so it won't move due to garbage collection

		pin_ptr<int> w = &jpegWidth, h = &jpegHeight;
		pin_ptr<SubsamplingOption> s = &jpegSubsamp;
		pin_ptr<Colorspace> c = &jpegColorspace;

		if (tjDecompressHeader3(handle, pinnedJpegImage, (unsigned long)imageSize, w, h, (int*)s, (int*)c) == -1)
			throw gcnew TJException("tjDecompressHeader3 failed");
	}
	/// <summary>
	/// Gets the width in pixels of the last image assigned to this instance. You may call this any time after setting the source image.
	/// </summary>
	int TJDecompressor::getWidth()
	{
		if (jpegWidth < 1)
			throw gcnew Exception(NO_ASSOC_ERROR);
		return jpegWidth;
	}
	/// <summary>
	/// Gets the height in pixels of the last image assigned to this instance. You may call this any time after setting the source image.
	/// </summary>
	int TJDecompressor::getHeight()
	{
		if (jpegHeight < 1)
			throw gcnew Exception(NO_ASSOC_ERROR);
		return jpegHeight;
	}
	/// <summary>
	/// Gets the subsampling mode of the last image assigned to this instance. You may call this any time after setting the source image.
	/// </summary>
	SubsamplingOption TJDecompressor::getSubsamp()
	{
		if ((int)jpegSubsamp < 0)
			throw gcnew Exception(NO_ASSOC_ERROR);
		TJ::checkSubsampling(jpegSubsamp);
		return jpegSubsamp;
	}
	/// <summary>
	/// Gets the Colorspace of the last image assigned to this instance. You may call this any time after setting the source image.
	/// </summary>
	Colorspace TJDecompressor::getColorspace()
	{
		if ((int)jpegColorspace < 0)
			throw gcnew Exception(NO_ASSOC_ERROR);
		if ((int)jpegColorspace >= TJ::NUMCS)
			throw gcnew Exception("JPEG header information is invalid");
		return jpegColorspace;
	}
	/// <summary>
	/// Gets the byte array last assigned to this instance. You may call this any time after setting the source image.
	/// </summary>
	array<Byte>^ TJDecompressor::getJPEGBuf()
	{
		if (jpegBuf == nullptr)
			throw gcnew Exception(NO_ASSOC_ERROR);
		return jpegBuf;
	}
	/// <summary>
	/// Gets the image size in bytes last assigned to this instance. You may call this any time after setting the source image.
	/// </summary>
	int TJDecompressor::getJPEGSize()
	{
		if (jpegBufSize < 1)
			throw gcnew Exception(NO_ASSOC_ERROR);
		return jpegBufSize;
	}
	/// <summary>
	/// Returns the width of the largest scaled-down image that the TurboJPEG
	/// decompressor can generate without exceeding the desired image width and
	/// height.
	/// </summary>
	///
	/// <param name="desiredWidth">desired width (in pixels) of the decompressed image.
	/// Setting this to 0 is the same as setting it to the width of the JPEG image
	/// (in other words, the width will not be considered when determining the
	/// scaled image size.)</param>
	///
	/// <param name="desiredHeight">desired height (in pixels) of the decompressed image.
	/// Setting this to 0 is the same as setting it to the height of the JPEG
	/// image (in other words, the height will not be considered when determining
	/// the scaled image size.)</param>
	///
	/// <returns>the width of the largest scaled-down image that the TurboJPEG
	/// decompressor can generate without exceeding the desired image width and
	/// height.</returns>
	int TJDecompressor::getScaledWidth(int desiredWidth, int desiredHeight)
	{
		if (jpegWidth < 1 || jpegHeight < 1)
			throw gcnew Exception(NO_ASSOC_ERROR);
		if (desiredWidth < 0 || desiredHeight < 0)
			throw gcnew ArgumentException("Invalid argument in getScaledWidth()");
		array<TJScalingFactor^>^ sf = TJ::getScalingFactors();
		if (desiredWidth == 0)
			desiredWidth = jpegWidth;
		if (desiredHeight == 0)
			desiredHeight = jpegHeight;
		int scaledWidth = jpegWidth, scaledHeight = jpegHeight;
		for (int i = 0; i < sf->Length; i++)
		{
			scaledWidth = sf[i]->getScaled(jpegWidth);
			scaledHeight = sf[i]->getScaled(jpegHeight);
			if (scaledWidth <= desiredWidth && scaledHeight <= desiredHeight)
				break;
		}
		if (scaledWidth > desiredWidth || scaledHeight > desiredHeight)
			throw gcnew ArgumentException("Could not scale down to desired image dimensions");
		return scaledWidth;
	}

	/// <summary>
	/// Returns the height of the largest scaled-down image that the TurboJPEG
	/// decompressor can generate without exceeding the desired image width and
	/// height.
	/// </summary>
	///
	/// <param name="desiredWidth">desired width (in pixels) of the decompressed image.
	/// Setting this to 0 is the same as setting it to the width of the JPEG image
	/// (in other words, the width will not be considered when determining the
	/// scaled image size.)</param>
	///
	/// <param name="desiredHeight">desired height (in pixels) of the decompressed image.
	/// Setting this to 0 is the same as setting it to the height of the JPEG
	/// image (in other words, the height will not be considered when determining
	/// the scaled image size.)</param>
	///
	/// <returns>the height of the largest scaled-down image that the TurboJPEG
	/// decompressor can generate without exceeding the desired image width and
	/// height.</returns>
	int TJDecompressor::getScaledHeight(int desiredWidth, int desiredHeight)
	{
		if (jpegWidth < 1 || jpegHeight < 1)
			throw gcnew Exception(NO_ASSOC_ERROR);
		if (desiredWidth < 0 || desiredHeight < 0)
			throw gcnew ArgumentException("Invalid argument in getScaledHeight()");
		array<TJScalingFactor^>^ sf = TJ::getScalingFactors();
		if (desiredWidth == 0)
			desiredWidth = jpegWidth;
		if (desiredHeight == 0)
			desiredHeight = jpegHeight;
		int scaledWidth = jpegWidth, scaledHeight = jpegHeight;
		for (int i = 0; i < sf->Length; i++)
		{
			scaledWidth = sf[i]->getScaled(jpegWidth);
			scaledHeight = sf[i]->getScaled(jpegHeight);
			if (scaledWidth <= desiredWidth && scaledHeight <= desiredHeight)
				break;
		}
		if (scaledWidth > desiredWidth || scaledHeight > desiredHeight)
			throw gcnew ArgumentException("Could not scale down to desired image dimensions");
		return scaledHeight;
	}

	/// <summary>
	/// Decompress the JPEG source image or decode the YUV source image associated
	/// with this decompressor instance and output a grayscale, RGB, or CMYK image
	/// to the given destination buffer.
	/// (NOTE: YUV source images are not currently supported by this wrapper)
	/// </summary>
	///
	/// <param name="dstBuf">buffer that will receive the decompressed/decoded image.
	/// If the source image is a JPEG image, then this buffer should normally be
	/// <code>pitch * scaledHeight</code> bytes in size, where
	/// <code>scaledHeight</code> can be determined by calling <code>
	/// scalingFactor.{@link TJScalingFactor#getScaled getScaled}(jpegHeight)
	/// </code> with one of the scaling factors returned from {@link
	/// TJ#getScalingFactors} or by calling {@link #getScaledHeight}.  If the
	/// source image is a YUV image, then this buffer should normally be
	/// <code>pitch * height</code> bytes in size, where <code>height</code> is
	/// the height of the YUV image.  However, the buffer may also be larger than
	/// the dimensions of the source image, in which case the <code>x</code>,
	/// <code>y</code>, and <code>pitch</code> parameters can be used to specify
	/// the region into which the source image should be decompressed/decoded.</param>
	///
	/// <param name="x">x offset (in pixels) of the region in the destination image into
	/// which the source image should be decompressed/decoded. Usually you want this to be 0.</param>
	///
	/// <param name="y">y offset (in pixels) of the region in the destination image into
	/// which the source image should be decompressed/decoded. Usually you want this to be 0.</param>
	///
	/// <param name="desiredWidth">If the source image is a JPEG image, then this
	/// specifies the desired width (in pixels) of the decompressed image (or
	/// image region.)  If the desired destination image dimensions are different
	/// than the source image dimensions, then TurboJPEG will use scaling in the
	/// JPEG decompressor to generate the largest possible image that will fit
	/// within the desired dimensions.  Setting this to 0 is the same as setting
	/// it to the width of the JPEG image (in other words, the width will not be
	/// considered when determining the scaled image size.)  This parameter is
	/// ignored if the source image is a YUV image.</param>
	///
	/// <param name="pitch">bytes per line of the destination image.  Normally, this
	/// should be set to <code>scaledWidth * TJ.getPixelSize(pixelFormat)</code> if
	/// the destination image is unpadded, but you can use this to, for instance,
	/// pad each line of the destination image to a 4-byte boundary or to
	/// decompress/decode the source image into a region of a larger image.  NOTE:
	/// if the source image is a JPEG image, then <code>scaledWidth</code> can be
	/// determined by calling <code>TJScalingFactor.getScaled(jpegWidth)</code> 
	/// or by calling this.getScaledWidth().  If the source image is a
	/// YUV image, then <code>scaledWidth</code> is the width of the YUV image.
	/// Setting this parameter to 0 is the equivalent of setting it to
	/// <code>scaledWidth * TJ.getPixelSize(pixelFormat)</code>.</param>
	///
	/// <param name="desiredHeight">If the source image is a JPEG image, then this
	/// specifies the desired height (in pixels) of the decompressed image (or
	/// image region.)  If the desired destination image dimensions are different
	/// than the source image dimensions, then TurboJPEG will use scaling in the
	/// JPEG decompressor to generate the largest possible image that will fit
	/// within the desired dimensions.  Setting this to 0 is the same as setting
	/// it to the height of the JPEG image (in other words, the height will not be
	/// considered when determining the scaled image size.)  This parameter is
	/// ignored if the source image is a YUV image.</param>
	///
	/// <param name="pixelFormat">pixel format of the decompressed/decoded image (one of
	/// the turbojpegCLI.PixelFormat enum values)</param>
	///
	/// <param name="flags">the bitwise OR of one or more of
	/// the turbojpegCLI.Flag enum values</param>
	void TJDecompressor::decompress(array<Byte>^ dstBuf, int x, int y, int desiredWidth, int pitch, int desiredHeight, PixelFormat pixelFormat, Flag flags)
	{
		if (jpegBuf == nullptr)
			throw gcnew Exception(NO_ASSOC_ERROR);
		TJ::checkPixelFormat(pixelFormat);
		if (dstBuf == nullptr || x < 0 || y < 0 || pitch < 0 || (int)flags < 0)
			throw gcnew Exception("Invalid argument in decompress()");

		if (jpegBuf->Length < jpegBufSize)
			throw gcnew Exception("Source buffer is not large enough");

		int actualPitch = (pitch == 0) ? desiredWidth * tjPixelSize[(int)pixelFormat] : pitch;
		int arraySize = (y + desiredHeight - 1) * actualPitch + (x + desiredWidth) * tjPixelSize[(int)pixelFormat];

		if (dstBuf->Length < arraySize)
			throw gcnew Exception("Destination buffer is not large enough");

		pin_ptr<Byte> pinnedInput = &jpegBuf[0];
		pin_ptr<Byte> pinnedOutput = &dstBuf[0];

		if (tjDecompress2(handle, pinnedInput, (unsigned long)jpegBufSize, &pinnedOutput[y*actualPitch + x*tjPixelSize[(int)pixelFormat]], desiredWidth, pitch, desiredHeight, (int)pixelFormat, (int)flags) == -1)
			throw gcnew TJException("tjDecompress2 failed");
	}
	/// <summary>
	/// Decompress the JPEG source image or decode the YUV source image associated
	/// with this decompressor instance and output a grayscale, RGB, or CMYK image
	/// to the given destination buffer. 
	/// The image is decompressed to its native resolution.
	/// (NOTE: YUV source images are not currently supported by this wrapper)
	/// </summary>
	///
	/// <param name="dstBuf">buffer that will receive the decompressed/decoded image.
	/// If the source image is a JPEG image, then this buffer should normally be
	/// <code>pitch * scaledHeight</code> bytes in size, where
	/// <code>scaledHeight</code> can be determined by calling <code>
	/// scalingFactor.{@link TJScalingFactor#getScaled getScaled}(jpegHeight)
	/// </code> with one of the scaling factors returned from {@link
	/// TJ#getScalingFactors} or by calling {@link #getScaledHeight}.  If the
	/// source image is a YUV image, then this buffer should normally be
	/// <code>pitch * height</code> bytes in size, where <code>height</code> is
	/// the height of the YUV image.  However, the buffer may also be larger than
	/// the dimensions of the source image, in which case the <code>x</code>,
	/// <code>y</code>, and <code>pitch</code> parameters can be used to specify
	/// the region into which the source image should be decompressed/decoded.</param>
	///
	/// <param name="pixelFormat">pixel format of the decompressed/decoded image (one of
	/// the turbojpegCLI.PixelFormat enum values)</param>
	///
	/// <param name="flags">the bitwise OR of one or more of
	/// the turbojpegCLI.Flag enum values</param>
	void TJDecompressor::decompress(array<Byte>^ dstBuf, PixelFormat pixelFormat, Flag flags)
	{
		return decompress(dstBuf, 0, 0, jpegWidth, 0, jpegHeight, pixelFormat, flags);
	}
	/// <summary>
	/// Decompress the JPEG source image or decode the YUV source image associated
	/// with this decompressor instance and output a grayscale, RGB, or CMYK image
	/// to the given destination buffer. 
	/// The image is decompressed to its native resolution using the standard 
	/// RGB format.
	/// (NOTE: YUV source images are not currently supported by this wrapper)
	/// </summary>
	///
	/// <param name="dstBuf">buffer that will receive the decompressed/decoded image.
	/// If the source image is a JPEG image, then this buffer should normally be
	/// <code>pitch * scaledHeight</code> bytes in size, where
	/// <code>scaledHeight</code> can be determined by calling <code>
	/// scalingFactor.{@link TJScalingFactor#getScaled getScaled}(jpegHeight)
	/// </code> with one of the scaling factors returned from {@link
	/// TJ#getScalingFactors} or by calling {@link #getScaledHeight}.  If the
	/// source image is a YUV image, then this buffer should normally be
	/// <code>pitch * height</code> bytes in size, where <code>height</code> is
	/// the height of the YUV image.  However, the buffer may also be larger than
	/// the dimensions of the source image, in which case the <code>x</code>,
	/// <code>y</code>, and <code>pitch</code> parameters can be used to specify
	/// the region into which the source image should be decompressed/decoded.</param>
	void TJDecompressor::decompress(array<Byte>^ dstBuf)
	{
		return decompress(dstBuf, 0, 0, jpegWidth, 0, jpegHeight, PixelFormat::RGB, Flag::NONE);
	}

	/// <summary>
	/// Decompress the JPEG source image associated with this decompressor
	/// instance and return a new buffer containing the decompressed image.
	/// </summary>
	///
	/// <param name="desiredWidth">If the source image is a JPEG image, then this
	/// specifies the desired width (in pixels) of the decompressed image (or
	/// image region.)  If the desired destination image dimensions are different
	/// than the source image dimensions, then TurboJPEG will use scaling in the
	/// JPEG decompressor to generate the largest possible image that will fit
	/// within the desired dimensions.  Setting this to 0 is the same as setting
	/// it to the width of the JPEG image (in other words, the width will not be
	/// considered when determining the scaled image size.)  This parameter is
	/// ignored if the source image is a YUV image.</param>
	///
	/// <param name="pitch">bytes per line of the destination image.  Normally, this
	/// should be set to <code>scaledWidth * TJ.getPixelSize(pixelFormat)</code> if
	/// the destination image is unpadded, but you can use this to, for instance,
	/// pad each line of the destination image to a 4-byte boundary or to
	/// decompress/decode the source image into a region of a larger image.  NOTE:
	/// if the source image is a JPEG image, then <code>scaledWidth</code> can be
	/// determined by calling <code>TJScalingFactor.getScaled(jpegWidth)</code> 
	/// or by calling this.getScaledWidth().  If the source image is a
	/// YUV image, then <code>scaledWidth</code> is the width of the YUV image.
	/// Setting this parameter to 0 is the equivalent of setting it to
	/// <code>scaledWidth * TJ.getPixelSize(pixelFormat)</code>.</param>
	///
	/// <param name="desiredHeight">If the source image is a JPEG image, then this
	/// specifies the desired height (in pixels) of the decompressed image (or
	/// image region.)  If the desired destination image dimensions are different
	/// than the source image dimensions, then TurboJPEG will use scaling in the
	/// JPEG decompressor to generate the largest possible image that will fit
	/// within the desired dimensions.  Setting this to 0 is the same as setting
	/// it to the height of the JPEG image (in other words, the height will not be
	/// considered when determining the scaled image size.)  This parameter is
	/// ignored if the source image is a YUV image.</param>
	///
	/// <param name="pixelFormat">pixel format of the decompressed/decoded image (one of
	/// the turbojpegCLI.PixelFormat enum values)</param>
	///
	/// <param name="flags">the bitwise OR of one or more of
	/// the turbojpegCLI.Flag enum values</param>
	array<Byte>^ TJDecompressor::decompress(int desiredWidth, int pitch, int desiredHeight, PixelFormat pixelFormat, Flag flags)
	{
		if (jpegBuf == nullptr)
			throw gcnew Exception(NO_ASSOC_ERROR);
		TJ::checkPixelFormat(pixelFormat);
		if (pitch < 0 || desiredWidth < 0 || desiredHeight < 0 || (int)flags < 0)
			throw gcnew Exception("Invalid argument in decompress()");

		if (jpegBuf->Length < jpegBufSize)
			throw gcnew Exception("Source buffer is not large enough");

		int actualPitch = (pitch == 0) ? desiredWidth * tjPixelSize[(int)pixelFormat] : pitch;
		int arraySize = (desiredHeight - 1) * actualPitch + (desiredWidth) * tjPixelSize[(int)pixelFormat];

		array<Byte>^ dstBuf = gcnew array<Byte>(arraySize);

		decompress(dstBuf, 0, 0, desiredWidth, pitch, desiredHeight, pixelFormat, flags);

		return dstBuf;
	}

	/// <summary>
	/// Decompress the JPEG source image associated with this decompressor
	/// instance and return a new buffer containing the decompressed image.
	/// The image is decompressed to its native resolution.
	/// </summary>
	/// <param name="pixelFormat">pixel format of the decompressed/decoded image (one of
	/// the turbojpegCLI.PixelFormat enum values)</param>
	///
	/// <param name="flags">the bitwise OR of one or more of
	/// the turbojpegCLI.Flag enum values</param>
	array<Byte>^ TJDecompressor::decompress(PixelFormat pixelFormat, Flag flags)
	{
		return decompress(jpegWidth, jpegWidth * tjPixelSize[(int)pixelFormat], jpegHeight, pixelFormat, flags);
	}

	/// <summary>
	/// Decompress the JPEG source image associated with this decompressor
	/// instance and return a new buffer containing the decompressed image.
	/// The image is decompressed to its native resolution using the standard 
	/// RGB format.
	/// </summary>
	array<Byte>^ TJDecompressor::decompress()
	{
		return decompress(jpegWidth, jpegWidth * tjPixelSize[(int)PixelFormat::RGB], jpegHeight, PixelFormat::RGB, Flag::NONE);
	}
}