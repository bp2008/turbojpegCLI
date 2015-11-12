#pragma once
#pragma warning( disable : 4635 )
#include "turbojpeg.h"
#pragma warning( default : 4635 )
#include "TJScalingFactor.h"
#include "TJException.h"
#include "stringconvert.h"
using namespace System;
namespace turbojpegCLI
{
	public enum class SubsamplingOption
	{
		/// <summary>
		/// 4:4:4 chrominance subsampling (no chrominance subsampling).  The JPEG
		/// or YUV image will contain one chrominance component for every pixel in the
		/// source image.
		/// </summary>
		SAMP_444 = 0,
		/// <summary>
		/// 4:2:2 chrominance subsampling.  The JPEG or YUV image will contain one
		/// chrominance component for every 2x1 block of pixels in the source image.
		/// </summary>
		SAMP_422 = 1,
		/// <summary>
		/// 4:2:0 chrominance subsampling.  The JPEG or YUV image will contain one
		/// chrominance component for every 2x2 block of pixels in the source image.
		/// </summary>
		SAMP_420 = 2,
		/// <summary>
		/// Grayscale.  The JPEG or YUV image will contain no chrominance components.
		/// </summary>
		SAMP_GRAY = 3,
		/// <summary>
		/// 4:4:0 chrominance subsampling.  The JPEG or YUV image will contain one
		/// chrominance component for every 1x2 block of pixels in the source image.
		/// Note that 4:4:0 subsampling is not fully accelerated in libjpeg-turbo.
		/// </summary>
		SAMP_440 = 4,
		/// <summary>
		/// 4:1:1 chrominance subsampling.  The JPEG or YUV image will contain one
		/// chrominance component for every 4x1 block of pixels in the source image.
		/// JPEG images compressed with 4:1:1 subsampling will be almost exactly the
		/// same size as those compressed with 4:2:0 subsampling, and in the
		/// aggregate, both subsampling methods produce approximately the same
		/// perceptual quality.  However, 4:1:1 is better able to reproduce sharp
		/// horizontal features.  Note that 4:1:1 subsampling is not fully accelerated
		/// in libjpeg-turbo.
		/// </summary>
		SAMP_411 = 5
	};
	public enum class PixelFormat
	{
		/// <summary>
		/// RGB pixel format.  The red, green, and blue components in the image are
		/// stored in 3-byte pixels in the order R, G, B from lowest to highest byte
		/// address within each pixel.
		/// </summary>
		RGB = 0,
		/// <summary>
		/// BGR pixel format.  The red, green, and blue components in the image are
		/// stored in 3-byte pixels in the order B, G, R from lowest to highest byte
		/// address within each pixel.
		/// </summary>
		BGR = 1,
		/// <summary>
		/// RGBX pixel format.  The red, green, and blue components in the image are
		/// stored in 4-byte pixels in the order R, G, B from lowest to highest byte
		/// address within each pixel.  The X component is ignored when compressing
		/// and undefined when decompressing.
		/// </summary>
		RGBX = 2,
		/// <summary>
		/// BGRX pixel format.  The red, green, and blue components in the image are
		/// stored in 4-byte pixels in the order B, G, R from lowest to highest byte
		/// address within each pixel.  The X component is ignored when compressing
		/// and undefined when decompressing.
		/// </summary>
		BGRX = 3,
		/// <summary>
		/// XBGR pixel format.  The red, green, and blue components in the image are
		/// stored in 4-byte pixels in the order R, G, B from highest to lowest byte
		/// address within each pixel.  The X component is ignored when compressing
		/// and undefined when decompressing.
		/// </summary>
		XBGR = 4,
		/// <summary>
		/// XRGB pixel format.  The red, green, and blue components in the image are
		/// stored in 4-byte pixels in the order B, G, R from highest to lowest byte
		/// address within each pixel.  The X component is ignored when compressing
		/// and undefined when decompressing.
		/// </summary>
		XRGB = 5,
		/// <summary>
		/// Grayscale pixel format.  Each 1-byte pixel represents a luminance
		/// (brightness) level from 0 to 255.
		/// </summary>
		GRAY = 6,
		/// <summary>
		/// RGBA pixel format.  This is the same as {@link #PF_RGBX}, except that when
		/// decompressing, the X byte is guaranteed to be 0xFF, which can be
		/// interpreted as an opaque alpha channel.
		/// </summary>
		RGBA = 7,
		/// <summary>
		/// BGRA pixel format.  This is the same as {@link #PF_BGRX}, except that when
		/// decompressing, the X byte is guaranteed to be 0xFF, which can be
		/// interpreted as an opaque alpha channel.
		/// </summary>
		BGRA = 8,
		/// <summary>
		/// ABGR pixel format.  This is the same as {@link #PF_XBGR}, except that when
		/// decompressing, the X byte is guaranteed to be 0xFF, which can be
		/// interpreted as an opaque alpha channel.
		/// </summary>
		ABGR = 9,
		/// <summary>
		/// ARGB pixel format.  This is the same as {@link #PF_XRGB}, except that when
		/// decompressing, the X byte is guaranteed to be 0xFF, which can be
		/// interpreted as an opaque alpha channel.
		/// </summary>
		ARGB = 10,
		/// <summary>
		/// CMYK pixel format.  Unlike RGB, which is an additive color model used
		/// primarily for display, CMYK (Cyan/Magenta/Yellow/Key) is a subtractive
		/// color model used primarily for printing.  In the CMYK color model, the
		/// value of each color component typically corresponds to an amount of cyan,
		/// magenta, yellow, or black ink that is applied to a white background.  In
		/// order to convert between CMYK and RGB, it is necessary to use a color
		/// management system (CMS.)  A CMS will attempt to map colors within the
		/// printer's gamut to perceptually similar colors in the display's gamut and
		/// vice versa, but the mapping is typically not 1:1 or reversible, nor can it
		/// be defined with a simple formula.  Thus, such a conversion is out of scope
		/// for a codec library.  However, the TurboJPEG API allows for compressing
		/// CMYK pixels into a YCCK JPEG image (see {@link #CS_YCCK}) and
		/// decompressing YCCK JPEG images into CMYK pixels.
		/// </summary>
		CMYK = 11
	};
	public enum class Colorspace
	{
		/// <summary>
		/// RGB colorspace.  When compressing the JPEG image, the R, G, and B
		/// components in the source image are reordered into image planes, but no
		/// colorspace conversion or subsampling is performed.  RGB JPEG images can be
		/// decompressed to any of the extended RGB pixel formats or grayscale, but
		/// they cannot be decompressed to YUV images.
		/// </summary>
		RGB = 0,
		/// <summary>
		/// YCbCr colorspace.  YCbCr is not an absolute colorspace but rather a
		/// mathematical transformation of RGB designed solely for storage and
		/// transmission.  YCbCr images must be converted to RGB before they can
		/// actually be displayed.  In the YCbCr colorspace, the Y (luminance)
		/// component represents the black &amp; white portion of the original image, and
		/// the Cb and Cr (chrominance) components represent the color portion of the
		/// original image.  Originally, the analog equivalent of this transformation
		/// allowed the same signal to drive both black &amp; white and color televisions,
		/// but JPEG images use YCbCr primarily because it allows the color data to be
		/// optionally subsampled for the purposes of reducing bandwidth or disk
		/// space.  YCbCr is the most common JPEG colorspace, and YCbCr JPEG images
		/// can be compressed from and decompressed to any of the extended RGB pixel
		/// formats or grayscale, or they can be decompressed to YUV planar images.
		/// </summary>
		YCbCr = 1,
		/// <summary>
		/// Grayscale colorspace.  The JPEG image retains only the luminance data (Y
		/// component), and any color data from the source image is discarded.
		/// Grayscale JPEG images can be compressed from and decompressed to any of
		/// the extended RGB pixel formats or grayscale, or they can be decompressed
		/// to YUV planar images.
		/// </summary>
		GRAY = 2,
		/// <summary>
		/// CMYK colorspace.  When compressing the JPEG image, the C, M, Y, and K
		/// components in the source image are reordered into image planes, but no
		/// colorspace conversion or subsampling is performed.  CMYK JPEG images can
		/// only be decompressed to CMYK pixels.
		/// </summary>
		CMYK = 3,
		/// <summary>
		/// YCCK colorspace.  YCCK (AKA "YCbCrK") is not an absolute colorspace but
		/// rather a mathematical transformation of CMYK designed solely for storage
		/// and transmission.  It is to CMYK as YCbCr is to RGB.  CMYK pixels can be
		/// reversibly transformed into YCCK, and as with YCbCr, the chrominance
		/// components in the YCCK pixels can be subsampled without incurring major
		/// perceptual loss.  YCCK JPEG images can only be compressed from and
		/// decompressed to CMYK pixels.
		/// </summary>
		YCCK = 4
	};
	public enum class Flag
	{
		/// <summary>
		/// A flag specifying no flags are to be used. This is equivalent to the integer 0.
		/// </summary>
		NONE = 0,
		/// <summary>
		/// The uncompressed source/destination image is stored in bottom-up (Windows,
		/// OpenGL) order, not top-down (X11) order.
		/// </summary>
		BOTTOMUP = 2,
		/// <summary>
		/// When decompressing an image that was compressed using chrominance
		/// subsampling, use the fastest chrominance upsampling algorithm available in
		/// the underlying codec.  The default is to use smooth upsampling, which
		/// creates a smooth transition between neighboring chrominance components in
		/// order to reduce upsampling artifacts in the decompressed image.
		/// </summary>
		FASTUPSAMPLE = 256,
		/// <summary>
		/// Use the fastest DCT/IDCT algorithm available in the underlying codec.  The
		/// default if this flag is not specified is implementation-specific.  For
		/// example, the implementation of TurboJPEG for libjpeg[-turbo] uses the fast
		/// algorithm by default when compressing, because this has been shown to have
		/// only a very slight effect on accuracy, but it uses the accurate algorithm
		/// when decompressing, because this has been shown to have a larger effect.
		/// </summary>
		FASTDCT = 2048,
		/// <summary>
		/// Use the most accurate DCT/IDCT algorithm available in the underlying
		/// codec.  The default if this flag is not specified is
		/// implementation-specific.  For example, the implementation of TurboJPEG for
		/// libjpeg[-turbo] uses the fast algorithm by default when compressing,
		/// because this has been shown to have only a very slight effect on accuracy,
		/// but it uses the accurate algorithm when decompressing, because this has
		/// been shown to have a larger effect.
		/// </summary>
		ACCURATEDCT = 4096
	};
	public ref class TJ
	{
	public:
		/// <summary>
		/// The number of chrominance subsampling options
		/// </summary>
		static const int NUMSAMP = 6;

		/// <summary>
		/// The number of JPEG colorspaces
		/// </summary>
		static const int NUMCS = 5;

		/// <summary>
		/// The number of pixel formats
		/// </summary>
		static const int NUMPF = 12;

		/// <summary>
		/// Throws an exception if the specified SubsamplingOption is invalid.
		/// </summary>
		static void checkSubsampling(SubsamplingOption subsamp)
		{
			if (!isValidSubsampling(subsamp))
				throw gcnew ArgumentException("Invalid subsampling type");
		}

		/// <summary>
		/// Throws an exception if the specified PixelFormat is invalid.
		/// </summary>
		static void checkPixelFormat(PixelFormat pixelFormat)
		{
			if (!isValidPixelFormat(pixelFormat))
				throw gcnew ArgumentException("Invalid pixel format");
		}

		/// <summary>
		/// Returns false if the specified SubsamplingOption is invalid.
		/// </summary>
		static bool isValidSubsampling(SubsamplingOption subsamp)
		{
			if (NUMSAMP != TJ_NUMSAMP)
				throw gcnew Exception("Mismatch between .NET and C API");
			if ((int)subsamp < 0 || (int)subsamp >= NUMSAMP)
				return false;
			return true;
		}

		/// <summary>
		/// Returns false if the specified PixelFormat is invalid.
		/// </summary>
		static bool isValidPixelFormat(PixelFormat pixelFormat)
		{
			if (NUMPF != TJ_NUMPF)
				throw gcnew Exception("Mismatch between .NET and C API");
			if ((int)pixelFormat < 0 || (int)pixelFormat >= NUMPF)
				return false;
			return true;
		}

		/// <summary>
		/// Returns the MCU block width for the given level of chrominance
		/// subsampling.
		/// </summary>
		///
		/// <param name="subsamp">the level of chrominance subsampling (one of
		/// <code>SubsamplingOption</code> enum values)</param>
		///
		/// <returns>the MCU block width for the given level of chrominance
		/// subsampling.</returns>
		static int getMCUWidth(SubsamplingOption subsamp)
		{
			checkSubsampling(subsamp);
			return tjMCUWidth[(int)subsamp];
		}

		/// <summary>
		/// Returns the MCU block height for the given level of chrominance
		/// subsampling.
		/// </summary>
		///
		/// <param name="subsamp">the level of chrominance subsampling (one of
		/// <code>SAMP_*</code>)</param>
		///
		/// <returns>the MCU block height for the given level of chrominance
		/// subsampling.</returns>
		static int getMCUHeight(SubsamplingOption subsamp)
		{
			checkSubsampling(subsamp);
			return tjMCUHeight[(int)subsamp];
		}

		/// <summary>
		/// Returns the pixel size (in bytes) for the given pixel format.
		/// </summary>
		///
		/// <param name="pixelFormat">the pixel format (one of <code>PixelFormat</code> enum values)</param>
		///
		/// <returns>the pixel size (in bytes) for the given pixel format.</returns>
		static int getPixelSize(PixelFormat pixelFormat)
		{
			checkPixelFormat(pixelFormat);
			return tjPixelSize[(int)pixelFormat];
		}

		/// <summary>
		/// For the given pixel format, returns the number of bytes that the red
		/// component is offset from the start of the pixel.  For instance, if a pixel
		/// of format <code>TJ.PF_BGRX</code> is stored in <code>char pixel[]</code>,
		/// then the red component will be
		/// <code>pixel[TJ.getRedOffset(TJ.PF_BGRX)]</code>.
		/// </summary>
		///
		/// <param name="pixelFormat">the pixel format (one of <code>PixelFormat</code> enum values)</param>
		///
		/// <returns>the red offset for the given pixel format.</returns>
		static int getRedOffset(PixelFormat pixelFormat)
		{
			checkPixelFormat(pixelFormat);
			return tjRedOffset[(int)pixelFormat];
		}

		/// <summary>
		/// For the given pixel format, returns the number of bytes that the green
		/// component is offset from the start of the pixel.  For instance, if a pixel
		/// of format <code>TJ.PF_BGRX</code> is stored in <code>char pixel[]</code>,
		/// then the green component will be
		/// <code>pixel[TJ.getGreenOffset(TJ.PF_BGRX)]</code>.
		/// </summary>
		///
		/// <param name="pixelFormat">the pixel format (one of <code>PixelFormat</code> enum values)</param>
		///
		/// <returns>the green offset for the given pixel format.</returns>
		static int getGreenOffset(PixelFormat pixelFormat)
		{
			checkPixelFormat(pixelFormat);
			return tjGreenOffset[(int)pixelFormat];
		}

		/// <summary>
		/// For the given pixel format, returns the number of bytes that the blue
		/// component is offset from the start of the pixel.  For instance, if a pixel
		/// of format <code>TJ.PF_BGRX</code> is stored in <code>char pixel[]</code>,
		/// then the blue component will be
		/// <code>pixel[TJ.getBlueOffset(TJ.PF_BGRX)]</code>.
		/// </summary>
		///
		/// <param name="pixelFormat">the pixel format (one of <code>PixelFormat</code> enum values)</param>
		///
		/// <returns>the blue offset for the given pixel format.</returns>
		static int getBlueOffset(PixelFormat pixelFormat)
		{
			checkPixelFormat(pixelFormat);
			return tjBlueOffset[(int)pixelFormat];
		}

		/// <summary>
		/// Returns the maximum size of the buffer (in bytes) required to hold a JPEG
		/// image with the given width, height, and level of chrominance subsampling.
		/// </summary>
		///
		/// <param name="width">the width (in pixels) of the JPEG image</param>
		///
		/// <param name="height">the height (in pixels) of the JPEG image</param>
		///
		/// <param name="jpegSubsamp">the level of chrominance subsampling to be used when
		/// generating the JPEG image (one of SubsamplingOption enum values)</param>
		///
		/// <returns>the maximum size of the buffer (in bytes) required to hold a JPEG
		/// image with the given width, height, and level of chrominance subsampling.</returns>
		static int bufSize(int width, int height, SubsamplingOption jpegSubsamp)
		{
			unsigned long retval = tjBufSize(width, height, (int)jpegSubsamp);
			if (retval == -1)
				throw gcnew TJException(getSystemString(tjGetErrorStr()));
			return retval;
		}

		/// <summary>
		/// Returns a list of fractional scaling factors that the JPEG decompressor in
		/// this implementation of TurboJPEG supports.
		/// </summary>
		///
		/// <returns>a list of fractional scaling factors that the JPEG decompressor in
		/// this implementation of TurboJPEG supports.</returns>
		static array<TJScalingFactor^>^ getScalingFactors()
		{
			int n = 0;
			tjscalingfactor *sf;
			sf = tjGetScalingFactors(&n);
			if (sf == nullptr || n == 0)
				throw gcnew TJException(getSystemString(tjGetErrorStr()));

			array<TJScalingFactor^>^ sfManaged = gcnew array<TJScalingFactor^>(n);

			for (int i = 0; i < n; i++)
				sfManaged[i] = gcnew TJScalingFactor(sf[i].num, sf[i].denom);

			return sfManaged;
		}
	};
}