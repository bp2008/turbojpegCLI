using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using turbojpegCLI;

namespace TestTurbojpegCLI
{
	public static class Benchmark
	{
		const int jpegQuality = 80;
		private static int numIterations = 30;
		private static string inputFilePath;
		private static double bitmapTime = 1;
		private static List<Action> benchmarks = new List<Action>();
		static Benchmark()
		{
			try
			{
				benchmarks.Add(benchBitmap); // bitmap needs to run first to establish baseline timing.
				benchmarks.Add(benchTJSimpleAPI);
				benchmarks.Add(benchTJOptimized);
			}
			catch (Exception ex)
			{
				Console.ForegroundColor = ConsoleColor.Red;
				Console.WriteLine(ex.ToString());
				Console.ResetColor();
			}
		}

		public static void RunAllBenchmarks(string jpegFilePath, int iterations)
		{
			inputFilePath = jpegFilePath;
			numIterations = iterations;
			Console.WriteLine("Beginning benchmarks on " + jpegFilePath + ".");
			Console.WriteLine();
			Console.WriteLine(("Benchmark Name").PadRight(25, ' ') + ("Time").ToString().PadRight(10, ' ') + "% of Bitmap time");
			foreach (Action action in benchmarks)
			{
				System.GC.Collect();
				action();
			}
			Console.WriteLine();
			Console.WriteLine("All benchmarks complete.");
			System.GC.Collect();
		}
		private static void PrintBenchmarkResult(string benchmarkName, long timeMs)
		{
			Console.WriteLine(benchmarkName.PadRight(25, ' ') + (timeMs + "ms").ToString().PadRight(10, ' ') + (timeMs / bitmapTime).ToString("P"));
		}

		private static void benchBitmap()
		{
			byte[] data = File.ReadAllBytes(inputFilePath);
			byte[] recompressed = new byte[0];
			System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
			sw.Start();

			using (MemoryStream ms = new MemoryStream(data))
			{
				for (int i = 0; i < numIterations; i++)
				{
					using (Image decompressed = Image.FromStream(ms))
					{
						recompressed = BitmapUtility.GetJpegBytes(decompressed, jpegQuality);
					}
				}
			}
			sw.Stop();
			bitmapTime = (double)sw.ElapsedMilliseconds;
			PrintBenchmarkResult("System.Drawing.Bitmap", sw.ElapsedMilliseconds);
			File.WriteAllBytes("out-bitmap.jpg", recompressed);
		}

		private static void benchTJSimpleAPI()
		{
			byte[] data = File.ReadAllBytes(inputFilePath);
			byte[] rawImg = null;
			byte[] recompressed = null;
			System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
			sw.Start();
			using (TJDecompressor decomp = new TJDecompressor(data))
			{
				using (TJCompressor comp = new TJCompressor())
				{
					comp.setJPEGQuality(jpegQuality);
					for (int i = 0; i < numIterations; i++)
					{
						rawImg = decomp.decompress();
						comp.setSourceImage(rawImg, decomp.getWidth(), decomp.getHeight());
						recompressed = comp.compressToExactSize();
					}
				}
			}
			sw.Stop();
			PrintBenchmarkResult("turbojpegCLI simple API", sw.ElapsedMilliseconds);
			File.WriteAllBytes("out-libjpeg-turbo.jpg", recompressed);
		}

		/// <summary>
		/// This API usage is optimized for multiple runs of the same image size, by reusing the byte arrays instead of allocating new ones for every operation.
		/// </summary>
		private static void benchTJOptimized()
		{
			byte[] data = File.ReadAllBytes(inputFilePath);
			byte[] recompressed;
			int recompressedSize = 0;
			System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
			sw.Start();
			using (TJDecompressor decomp = new TJDecompressor(data))
			{
				// Optimize by pre-allocating the buffers and re-using them.
				byte[] rawImg = new byte[decomp.getWidth() * decomp.getHeight() * TJ.getPixelSize(PixelFormat.RGB)];
				recompressed = new byte[TJ.bufSize(decomp.getWidth(), decomp.getHeight(), decomp.getSubsamp())];
				using (TJCompressor comp = new TJCompressor(rawImg, decomp.getWidth(), decomp.getHeight()))
				{
					comp.setJPEGQuality(jpegQuality);
					for (int i = 0; i < numIterations; i++)
					{
						decomp.decompress(rawImg);
						comp.compress(ref recompressed, Flag.NONE);
					}
					recompressedSize = comp.getCompressedSize();
				}
			}
			sw.Stop();
			PrintBenchmarkResult("turbojpegCLI optimized", sw.ElapsedMilliseconds);
			using (FileStream fs = new FileStream("out-libjpeg-turbo-optimized.jpg", FileMode.Create))
			{
				fs.Write(recompressed, 0, recompressedSize);
			}
		}
	}
}
