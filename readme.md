# Introduction
This is another Json library created for a project that couldn' use a more recent version than VS2013. Based on standard library it uses c++ 11.
It uses the standard library but was created to be used as a dll in Windows environment.
The purpose of this project is to have an intuitive json parser developend in modern c++ that doesn't throw exception and optimizes memory usage.
# Installation
<u><b>Note:</b></u> The library was developped for a windows usage but has no other dependency to windows library than `__declspec` defined in ``JsonIO.h``.
1. Compile JsonIO project that will produce JsonIO.lib and JsonIO.dll
1. In your c++ project, ensure to include all required header files:
    - IJsonContainer.h
    - IJsonSerializable.h
    - IJsonValue.h
    - JArray.h
    - JObject.h
    - JPrimitive.h
    - Json.h
    - JsonIO.h
    - JsonIOHelper.h
    - JsonItem.h
    - JsonValue.h
    - JUndefined.h
1. In your c++ project, ensure to link to ``JsonIO.lib``
1. In your c++ code, just include `Json.h` file that includes all you need. You might want to use namespace json to simplify the code
1. At runtime, your application will need ``JsonIO.dll``

# Usage
The interface `IJsonSerializable` is dedicated to define `readObject` and `writeObject` method which is implemented by the model classes to map each property to a common object: `JObject` which is a dictionary mapping keys to `JsonValue` object.
A `JsonValue` can be one of `JArray`, `Jobject` or a `JPrimitive` which supports boolean, short, int, float, double, std::string and const char* primitives.
## IJsonSerializable
```cpp
#include "Json.h"

using namespace json;


class Item : public JsonSerializable
{
public:
    Item() {}
    Item(const std::string& name, int id) : m_name(name), m_id(id) {}
    void toObject(JObject& object) override
    {
        object["name"] = m_name;
        object["id"] = m_id;
    }
    void fromObject(const JObject& object) override
    {
        m_name = object["name"].getString("default");
        m_id = object["id"].getInt(-1);
    }
    std::string getName() const { return m_name; }
    int getId() const { return m_id; }
private:
    std::string m_name;
    int m_id{ 0 };
};

class Items : public JsonSerializable
{
public:
    Items()
    {
    }
    void toObject(JObject& object) override
    {
        JArray l_devices;
        for (auto& d : m_items)
        {
            JObject o;
            d.toObject(o);
            l_devices.push_back(o);
        }
        object["devices"] = l_devices;
    }

    void fromObject(const JObject& object) override
    {
        m_items.clear();
        const JArray& items = object["devices"].getArray();
        for (const auto& v : items)
        {
            const JObject& o = v.getObject();
            Item it;
            it.fromObject(o);
            add(it);
        }
    }

    void add(const Item& d)
    {
        m_items.push_back(d);
    }
    void add(Item&& d)
    {
        m_items.push_back(d);
    }
    Item& operator[](int index)
    {
        return m_items[index];
    }
    std::string to_string()
    {
        return serialize(false);
    }
private:
    std::vector<Item> m_items;
};

int main()
{
    Items d;
    d.add({ "device_1", 1 });
    d.add({ "device_2" , 2 });
    d.serialize(std::cout, true);
    Items d2;
    std::string djson = d.serialize(false);
    d2.deserialize(djson);
    d2.add({ "device_3", 3 });
    std::cout << d2.to_string() << std::endl;
    std::cout << "items[1]: " << d2[1].getName() << std::endl;

    // Expected results
    /*
    {
        "devices":
        [
            {
               "id": 1,
               "name": "device_1"
            },
            {
                "id": 2,
                "name": "device_2"
            }
        ]
    }
    {"devices": [{"id": 1, "name": "device_1"}, {"id": 2, "name": "device_2"}, {"id": 3, "name": "device_3"}]}
    items[1]: device_2
    */

}

```

## JsonObject
A `JObject` can be initialized with a json string or filled using operator []; The type of JsonValue is inferred from the primitive type or can be a `JObject` or a `JArray`.
Note that a copy is used when adding an object or an array. They should be filled to be added to the parent object.
A `JObject` can be written to or read from a string or a stream.
If one tries to access an index that does not exist or find a property that does not exist, a reference to an Undefined value is returned which will always return the default value. This is to avoid controls at runtime.
getInt on an object or an array will return the size of the object (number of properties) or array (number of items).

<u><b>Note:</b></u>The Undefined value will be in an error state and cannot be assigned. If you try to set a JsonValue in another object or array, the new value will be an Undefined value.

An object property can be accessed using an index but user should be aware that the index order depends on the alphabetical order of properties.
An array item can be accessed with a string if it can be parsed as a valid index of the array.

```cpp
#include "Json.h"
#include <iostream>

using namespace json;


int main()
{
    JObject o;
    o["isOk"] = true;
    o["value"] = 3500;
    o["item"] = JObject{ R"({"id": 1, "name": "item"})" };
    o["price"] = 35.78;
    JArray a;
    JArray a2{ 10, "20", o["item"], nullptr };
    for (int i = 1; i < 4; i++)
    {
        a << JObject{ {"id", i} };
    }
    o["ids"] = a;
    o["values"] = a2;
    std::cout << o.getString() << std::endl;
    o.write(std::cout, true);
    std::cout << "ids[0]: " << o["ids"][0].getString() << std::endl;
    std::cout << "o[0][\"0\"]: " << (std::string)o[0]["0"] << std::endl;
    std::cout << "isOk: " << o["isOk"].getBool() << std::endl;
    std::cout << "values: " << o["values"].getString() << std::endl;
    std::cout << "values[1]: " << (int)o["values"][1] << std::endl;
    std::cout << "ids[2].id: " << o.find("ids[2].id").getInt() << std::endl;
    std::cout << "price: " << o["price"].getDouble() << std::endl;
    std::cout << "test default: " << o.find("path.does.not.exist").getString("not found") << std::endl;

    // Expected results
    /*

        ids[0]: {"id", 1}
        o[0]["0"]: {"id", 1}
        isOk: true
        values: [10, "20", {"id": 1, "name": "item"}, null]
        values[1]: 20
        ids[2].id: 3
        price: 35.78
        test default: not found
    */
}

```

# Architecture
![Alt Image Text](./resources/general_architecture.png)
Your code will use ``JsonValue`` instances or reference to contain any primitive, a ``JObject`` or a ``JArray``.
You have too keep in mind that a ``JsonValue`` contains a unique pointer to any ``IJsonValue``. Affecting a ``JObject`` or a ``JArray`` to a ``JsonValue`` copy the content to that value even if you pass a reference.
When you set a ``JsonValue`` to any other type than ``JObject`` or ``JArray`` it will be stored as a ``JPrimitive<T>`` of the object.
The ``JPrimitive<T>`` describes specializations for the following types:
- bool
- short
- int
- float
- double
- const char*
- std::string
- nullptr_t

Here bellow the availables methods on JsonIO interfaces:

![Alt Image Text](./resources/interfaces_details.png)