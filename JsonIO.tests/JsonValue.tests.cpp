#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "Json.h"

using namespace json;

namespace JsonIOtests
{
	TEST_CLASS(JsonValuetests)
	{
		TEST_METHOD(UndefinedValue)
		{
			JsonValue v;
			Assert::IsTrue(v.getType() == E_JsonType::Undefined);
			Assert::IsTrue(v.isUndefined());
			JObject o;
			JsonValue& vRef = o["test"];
			Assert::IsTrue(vRef.isUndefined());

			JArray a;
			a << JsonValue{};
			Assert::IsTrue(a[0].isUndefined());
		}
		TEST_METHOD(TypeInitialization)
		{
			JsonValue v;
			E_JsonType types[] = { E_JsonType::Array, E_JsonType::Bool, E_JsonType::Short, E_JsonType::Int, E_JsonType::Float, E_JsonType::Double, E_JsonType::String, E_JsonType::Null, E_JsonType::Object, E_JsonType::Undefined};
			for (E_JsonType t : types)
			{
				v = JsonValue{ t };
				Assert::AreEqual(toString(t), toString(v.getType()));
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

		TEST_METHOD(TestShortEquality)
		{
			unsigned short s = 1;
			JsonValue v = (short)1;
			JsonValue v2 = s;
			Assert::AreEqual((short)1, (short)v2);
			Assert::AreEqual((std::string)v, (std::string)v2);
			Assert::IsTrue(v == v2);
		}

		TEST_METHOD(TestFloatEquality)
		{
			float f = 1;
			JsonValue v = (float)1;
			JsonValue v2 = f;
			Assert::AreEqual((float)1, (float)v2);
			Assert::AreEqual((std::string)v, (std::string)v2);
			Assert::IsTrue(v == v2);
		}

		TEST_METHOD(TestDoubleEquality)
		{
			double d = 1;
			JsonValue v = (double)1;
			JsonValue v2 = d;
			Assert::AreEqual((double)1, (double)v2);
			Assert::AreEqual((std::string)v, (std::string)v2);
			Assert::IsTrue(v == v2);
		}

		TEST_METHOD(TestStringEquality)
		{
			std::string s{ "Test" };
			JsonValue v = "Test";
			JsonValue v2 = s;
			Assert::AreEqual(std::string{"Test"}, (std::string)v2);
			Assert::IsTrue(v == v2);
		}

		TEST_METHOD(TestNullEquality)
		{
			JsonValue v = nullptr;
			JsonValue v2 = nullptr;
			Assert::AreEqual(JsonValue{nullptr}.getString(), v2.getString());
			Assert::IsTrue(v == v2);
		}

		TEST_METHOD(TestJObjectEquality)
		{
			JsonValue v = JObject{ { "id", 1 }, { "value","test" } };
			JsonValue v2 = JObject{ { "id", 1 }, { "value","test" } };
			Assert::IsTrue(v == JObject{R"({"id": 1, "value": "test"})"});
			Assert::IsTrue(v == v2);
		}

		TEST_METHOD(TestJArrayEquality)
		{
			JObject o{ { "id", 1 }, {"value","test"} };
			JsonValue v = JArray{ 1, "deux", nullptr, o };
			JsonValue v2 = JArray( R"([1, "deux", null, {"id": 1, "value": "test"}])" );
			JArray a;
			a << 1 << "deux" << nullptr << JObject{ { "id", 1 }, {"value","test"} };
			Assert::IsTrue(v == a);
			Assert::IsTrue(v == v2);
		}

		TEST_METHOD(ErrorHandling)
		{
			JsonValue v = 1;
			JObject o;
			JArray a;
			JsonValue& vError = v[1];
			JObject& vOError = v.getObject();
			JArray& vAError = v.getArray();

			Assert::IsTrue(vError.getType() == E_JsonType::Error, L"vError doit etre en Error");
			Assert::IsTrue(vOError.getType() == E_JsonType::Error, L"vOError doit etre en Error");
			Assert::IsTrue(vAError.getType() == E_JsonType::Error, L"vAError doit etre en Error");

			vError = 1;
			vOError = JObject{ {"id", 1} };
			vAError = JArray{ 1,2,3 };

			Assert::AreEqual(-1, vError.getInt(-1));
			Assert::IsTrue(vError.getType() == E_JsonType::Error, L"vError doit etre en Error");
			Assert::IsTrue(vOError.isEmpty());
			Assert::IsFalse(vOError == JObject{ {"id", 1 } });
			Assert::IsTrue(vOError.getType() == E_JsonType::Error, L"vOError doit etre en Error");
			Assert::IsTrue(vAError.isEmpty());
			Assert::IsFalse(vAError == JArray{ 1,2,3 });
			Assert::IsTrue(vAError.getType() == E_JsonType::Error, L"vAError doit etre en Error");

			JsonValue vCopyError = vError;
			JObject vOCopyError = vOError;
			JArray vACopyError = vAError;
			Assert::IsTrue(vCopyError.getType() == E_JsonType::Undefined, L"vCopyError doit etre Undefined");
			Assert::IsTrue(vOCopyError.getType() == E_JsonType::Object, L"vOCopyError doit etre Object");
			Assert::IsTrue(vACopyError.getType() == E_JsonType::Array, L"vACopyError doit etre Array");
			o["v"] = vError;
			o["vO"] = vOError;
			o["vA"] = vAError;
			Assert::AreEqual((std::size_t)3, o.size());
			Assert::IsTrue(o["v"].getType() == E_JsonType::Undefined, L"vCopyError doit etre Undefined");
			Assert::IsTrue(o["vO"].getType() == E_JsonType::Object, L"vOCopyError doit etre Object");
			Assert::IsTrue(o["vA"].getType() == E_JsonType::Array, L"vACopyError doit etre Array");
			o["v"] = 1;
			o["vO"]["test"] = 1;
			o["vA"].getArray() << 1;
			Assert::AreEqual(1, o["v"].getInt());
			Assert::AreEqual(1, o["vO"]["test"].getInt());
			Assert::AreEqual(1, o["vA"][0].getInt());
		}

		TEST_METHOD(CopyHandling)
		{
			JsonValue v = 1;
			JsonValue v2 = v;
			Assert::IsTrue(v == v2);
			Assert::AreEqual(1, v2.getInt());
			v = JObject{ {"id", 1} };
			v2 = v;
			Assert::IsTrue(v == v2);
			Assert::AreEqual(JObject{ {"id", 1} }.getString(), v2.getString());
			JsonValue& v2Ref = v2;
			v2["test"] = "test";
			Assert::AreEqual(2, v2.getInt());
			Assert::AreEqual(1, v.getInt());
			Assert::AreEqual(2, v2.getInt());

			v = JArray{ 1,2,3 };
			v2 = v;

			Assert::AreEqual(JArray{ 1,2,3 }.getString(), v.getArray().getString());
			v2.getArray() << 4;
			Assert::AreEqual(4, v2.getInt());
			Assert::AreEqual(3, v.getInt());
			Assert::AreEqual(4, v2.getInt());

			JObject o;
			o["a"] = v2;
			JObject o2;
			o2["a"] = v2Ref;
			v2.getArray() << 5;
			Assert::AreEqual(4, o["a"].getInt());
			Assert::AreEqual(4, o2["a"].getInt());
			Assert::AreEqual(5, v2Ref.getInt());
		}
	};
}