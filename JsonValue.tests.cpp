#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "Json.h"

using namespace json;

namespace JsonIOtests
{
	TEST_CLASS(JsonValuetests)
	{
		TEST_METHOD(TypeInitialization)
		{
			JsonValue v;
			E_JsonType types[] = { E_JsonType::Array, E_JsonType::Bool, E_JsonType::Short, E_JsonType::Int, E_JsonType::Float, E_JsonType::Double, E_JsonType::String, E_JsonType::Null, E_JsonType::Object, E_JsonType::Undefined , E_JsonType::Error};
			for (E_JsonType t : types)
			{
				v = JsonValue{ t };
				Assert::IsTrue(v.getType() == t);
			}
		}

		TEST_METHOD(TestIntEquality)
		{
			JsonValue v = 1;
			JsonValue v2 = 1;
			Assert::AreEqual(1, (int)v);
			Assert::IsTrue(v == v2);
		}

		TEST_METHOD(TestBoolEquality)
		{
			JsonValue v = true;
			JsonValue v2 = true;
			Assert::AreEqual(true, (bool)v);
			Assert::IsTrue(v == v2);
		}

		TEST_METHOD(TestJObjectEquality)
		{
			JsonValue v = JObject{ { "id", 1 }, { "value","test" } };
			JsonValue v2 = JObject{ { "id", 1 }, { "value","test" } };
			Assert::IsTrue(v == JObject{R"({"id": 1, "value": "test"})"});
			Assert::IsTrue(v == v2);
		}
	};
}