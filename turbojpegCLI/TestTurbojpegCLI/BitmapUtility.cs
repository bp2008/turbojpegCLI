using System;
using System.Collections.Generic;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;

namespace TestTurbojpegCLI
{
	public static class BitmapUtility
	{
		public static byte[] GetJpegBytes(System.Drawing.Image img, long quality = 80)
		{
			using (MemoryStream ms = new MemoryStream())
			{
				SaveJpeg(ms, img, quality);
				return ms.ToArray();
			}
		}
		/// <summary>
		/// Saves the image into the specified stream using the specified jpeg quality level.
		/// </summary>
		/// <param name="stream">The stream to save the image in.</param>
		/// <param name="img">The image to save.</param>
		/// <param name="quality">(Optional) Jpeg compression quality, from 0 (low quality) to 100 (very high quality).</param>
		private static void SaveJpeg(System.IO.Stream stream, System.Drawing.Image img, long quality = 80)
		{
			if (quality < 0)
				quality = 0;
			if (quality > 100)
				quality = 100;
			EncoderParameters encoderParams = new EncoderParameters(1);
			EncoderParameter qualityParam = new EncoderParameter(System.Drawing.Imaging.Encoder.Quality, quality);
			ImageCodecInfo jpegCodec = GetEncoderInfo("image/jpeg");
			if (jpegCodec == null)
				return;

			encoderParams.Param[0] = qualityParam;
			img.Save(stream, jpegCodec, encoderParams);
		}
		private static ImageCodecInfo GetEncoderInfo(string mimeType)
		{
			ImageCodecInfo[] encoders = ImageCodecInfo.GetImageEncoders();

			for (int i = 0; i < encoders.Length; i++)
				if (encoders[i].MimeType == mimeType)
					return encoders[i];
			return null;
		}
	}
}
