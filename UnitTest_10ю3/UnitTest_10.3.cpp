
#include "pch.h"
#include "CppUnitTest.h"
#include "../lab_10.3/lab_10.3.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest10ю3
{
	TEST_CLASS(UnitTest10ю3)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Assert::AreEqual(4, calc_n(1000));
		}
	};
}
