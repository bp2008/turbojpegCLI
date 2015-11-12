#pragma once
using namespace System;

namespace turbojpegCLI
{
	public ref class TJException : public Exception
	{
	public:
		TJException() : Exception()
		{
		}
		TJException(String^ msg) : Exception(msg)
		{
		}
	};
}
