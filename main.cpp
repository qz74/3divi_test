/* json example

{                            // object "", value is object type
  1,                         // object "", value is int type
  4242,                      // object "", value is int type
  "blabla",                  // object "", value is string type
  "arr" : [4.3, 2.2, 1.1],   // object "arr", value is array type, array values are double type
  true,                      // object "", value is bool type
  false,                     // object "", value is bool type
  {                          // object "", value is object type
    "a" : 123,               // object "a", value is int type
    "b" : "321",             // object "b", value is double type
    "c" : {                  // object "c", value is object type
      555,                   // object "", value is int type
      "vavava"               // object "", value is string type
    },
    [true, false] // object "", value is array type, array values are bool type
  }
}

*/

#include <iostream>
#include <string>
#include <utility>

#include "context.h"
#include "json/json.h"

Json::Value ToJson(context::TObject obj);
Json::Value ToJson(context::TArray arr);
Json::Value ToJson(context::TValue v);

Json::Value ToJson(context::TObject obj)
{
  Json::Value root;
  for (auto &ov: obj)
    root[ov.first] = ToJson(ov.second);

  return root;
}

Json::Value ToJson(context::TArray arr)
{
  Json::Value root;
  for (auto &av: arr)
    root.append(ToJson(av));

  return root;
}

Json::Value ToJson(context::TValue v)
{
  Json::Value root;
  switch (v.Type()) {
    case context::TValue::tType::Null:
      break;
    case context::TValue::tType::Bool:
      root = v.as_bool();
      break;
    case context::TValue::tType::Int:
      root = v.as_int();
      break;
    case context::TValue::tType::Double:
      root = v.as_double();
      break;
    case context::TValue::tType::String:
      root = v.as_string();
      break;
    case context::TValue::tType::Array: {
      for (auto &av: v.as_array())
        root.append(ToJson(av));
      break;
    }
    case context::TValue::tType::Object: {
      for (auto &ov: v.as_object())
        root[ov.first] = ToJson(ov.second);
      break;
    }
  }

  return root;
}

using namespace context;

int main()
{
  TObject root;
  root.insert("null", nullptr);
  root.insert("int", 123);
  root.insert("double", 333.125);
  root.insert("bool", true);
  root.insert("string", "blablabla");

  TArray my_array;
  root.insert("array1", my_array); // empty array
  my_array.push_back(11);
  my_array.push_back(22);
  my_array.push_back(33);
  root.insert("array2", my_array);

  TObject map_obj;
  root.insert("map1", map_obj); // empty map
  map_obj.insert("first", 1.1);
  map_obj.insert("second", "2.2");
  map_obj.insert("third", "33");
  map_obj.insert("fourth", false);
  map_obj.insert("fifth", my_array);
  root.insert("map2", map_obj);

  Json::Value jsonobj = ToJson(root);

  std::cout << "JSON:" << std::endl;
  std::cout << jsonobj << std::endl;

  auto it = root.find("double");
  std::cout << "'double' is " << ((it == root.end())?("not found!"):("found!")) << std::endl;
  it = root.find("third");
  std::cout << "'third' is " << ((it == root.end())?("not found!"):("found!")) << std::endl;

  return 0;
}
