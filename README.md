# turbojpegCLI

turbojpegCLI is a thin wrapper which makes the libjpeg-turbo library usable from all .NET languages, including C# and VB.

The real work is done by the underlying library, libjpeg-turbo, which provides very fast JPEG encoding and decoding methods.  You can find out more about the excellent libjpeg-turbo library here: http://www.libjpeg-turbo.org/

Windows binaries for libjpeg-turbo 1.4.2 are included in the binary distributions of turbojpegCLI.

## How fast is it?

Based on my own crude tests on an i7-3770k CPU, libjpeg-turbo wrapped by turbojpegCLI (this project) can encode and decode JPEG images in **40% to 75%** of the time required for the System.Drawing.Bitmap class to do the same.  Performance varies depending on the images and the formats involved.

## Implementation details

turbojpegCLI is written in C++/CLI, a language which bridges the gap between native and managed .NET code without using P/Invoke.  The resulting library is usable from any .NET framework language, including C# and VB (Visual Basic).

The project is built with Visual Studio 2013 and that means there are run-time library requirements : https://www.microsoft.com/en-us/download/details.aspx?id=40784

## Compatibility

I have only tested this on Windows.  I do not know if it is compatible with Linux and/or Mono, but feel free to try.

## What functionality is wrapped?

turbojpegCLI exposes only JPEG encoding and decoding, to and from byte arrays.  This wrapper does not use System.Drawing.Bitmap.  Libjpeg-turbo also includes methods for image transformations and YUVImage encode/decode, but I did not wrap these.

Most of the syntax and design was derived from the Java wrapper that is included in the official libjpeg-turbo source.  To save time, most of the method documentation comments were copied directly from the Java wrapper and minimally converted to XML format.  As such, some of the documentation refers to YUV images which are not supported in this library.

## An alternative wrapper

In case this wrapper doesn't float your boat, there is another completely different wrapper on NuGet, written and published by someone else entirely.  I have not tried it and can't speak for its performance or ease of use, but here it is:  https://www.nuget.org/packages/TurboJpegWrapper/
