using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TestTurbojpegCLI
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
			Benchmark.RunAllBenchmarks("testimg.jpg", 20);
			/*
			 *	These are example results.  In practice, the efficiency varies greatly depending on the format of the source image.
			 *
			 *	Benchmark Name           Time      % of Bitmap time
			 *	System.Drawing.Bitmap    3028ms    100.00 %
			 *	turbojpegCLI simple API  1279ms    42.24 %
			 *	turbojpegCLI optimized   1230ms    40.62 %
			 */
			Console.WriteLine("Done. Press Enter to close this program.");
			Console.ReadLine();
		}
	}
}
