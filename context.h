#ifndef CONTEXT_H
#define CONTEXT_H

#include <iostream>
#include <variant>
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <iterator>
#include <map>
#include <algorithm>
#include <type_traits>


namespace context {

class TObject;
class TArray;
class TValue;

using TNamedValue = std::pair<std::string, TValue>;
using TObjectPtr = std::shared_ptr<TObject>;
using TArrayPtr  = std::shared_ptr<TArray>;

//! associative container name-value
class TObject {
public:
  using TStorageType = std::vector<TNamedValue>;
  using iterator = typename TStorageType::iterator;
  using const_iterator  = typename TStorageType::const_iterator;

  TObject()                     = default;
  TObject(const TObject &other) = default;
  TObject(TObject &&other)      = default;
  TObject &operator=(const TObject &rhs) = default;
  TObject &operator=(TObject &&rhs) = default;

  size_t size() const {
    return values_.size();
  }
  bool empty() const {
    return values_.empty();
  }

  iterator find(const std::string &s);
  const_iterator find(const std::string &s) const;

  template <class T>
  std::pair<iterator, bool> insert(std::string key, const T &v);

  TValue operator[](const std::string &key) const;
  TValue &operator[](const std::string &key);
  TValue at(const std::string &key) const;
  TValue &at(const std::string &key);

  iterator begin() {
    return values_.begin();
  }
  iterator end() {
    return values_.end();
  }
  const_iterator begin() const {
    return values_.begin();
  }
  const_iterator end() const {
    return values_.end();
  }

private:
  TStorageType values_;
  std::map<std::string, size_t> index_map_;

};

//! vector-like values container
class TArray {
public:
  using TStorageType = std::vector<TValue>;
  using iterator = typename TStorageType::iterator;
  using const_iterator = typename TStorageType::const_iterator;

  TArray()                    = default;
  TArray(const TArray &other) = default;
  TArray(TArray &&other)      = default;
  TArray &operator=(TArray &&rhs) = default;
  TArray &operator=(const TArray &rhs) = default;

  template <class In>
  TArray(In first, In last) {
    values_.insert(values_.end(), first, last);
  }

  iterator begin() {
    return values_.begin();
  }
  iterator end() {
    return values_.end();
  }
  const_iterator begin() const {
    return values_.begin();
  }
  const_iterator end() const {
    return values_.end();
  }

  size_t size() const {
    return values_.size();
  }
  bool empty() const {
    return values_.empty();
  }

  TValue  operator[](std::size_t n) const;
  TValue &operator[](std::size_t n);
  TValue  at(std::size_t n) const;
  TValue &at(std::size_t n);

  template <class T>
  void push_back(const T &v) {
    values_.emplace_back(v);
  }

private:
  TStorageType values_;

};

//! value storage
//! value can be NULL, scalar (int, bool double, string) or array or object
class TValue {
public:
  explicit TValue(const TArray &a);
  explicit TValue(const TObject &o);

  TValue(TArray &&a);
  TValue(TObject &&o);

  TValue(const TValue &other)
    : value_(other.value_), type_(other.type_)
  {	}
  TValue(TValue &&other)
    : value_(std::move(other.value_)), type_(other.type_)
  {	}

  TValue()
    : value_(tNull()), type_(Null)
  { }
  TValue(const std::nullptr_t &)
    : value_(tNull()), type_(Null)
  { }
  TValue(int v)
    : value_(v), type_(Int)
  { }
  TValue(double v)
    : value_(v), type_(Double)
  { }
  TValue(bool b)
    : value_(b), type_(Bool)
  { }
  TValue(std::string s)
    : value_(std::move(s)), type_(String)
  { }
  TValue(const char *s)
    : value_(std::string(s)), type_(String) {
  }

  const std::string &as_string() const {
    switch (type_) {
      case tType::String:
        return std::get<std::string>(value_);
      default:
        throw std::exception();
    }
  }
  std::string &as_string() {
    switch (type_) {
      case tType::String:
        return std::get<std::string>(value_);
      default:
        throw std::exception();
    }
  }

  const TArray &as_array() const {
    if (type_ != tType::Array)
      throw std::exception();
    return *std::get<TArrayPtr>(value_);
  }
  TArray &as_array() {
    if (type_ != tType::Array)
      throw std::exception();
    return *std::get<TArrayPtr>(value_);
  }

  const TObject &as_object() const {
    if (type_ != tType::Object)
      throw std::exception();
    return *std::get<TObjectPtr>(value_);
  }
  TObject &as_object() {
    if (type_ != tType::Object)
      throw std::exception();
    return *std::get<TObjectPtr>(value_);
  }

  int as_int() {
    if (type_ != tType::Int)
      throw std::exception();
    return std::get<int>(value_);
  }
  double as_double() {
    if (type_ != tType::Double)
      throw std::exception();
    return std::get<double>(value_);
  }
  int as_bool() {
    if (type_ != tType::Bool)
      throw std::exception();
    return std::get<bool>(value_);
  }

private:
  explicit TValue(TObjectPtr o)
    : value_(std::move(o)), type_(TValue::tType::Object)
  { }
  explicit TValue(TArrayPtr a)
    : value_(std::move(a)), type_(TValue::tType::Array)
  { }

public:
  TValue &operator=(const TValue &rhs);
  TValue &operator=(TValue &&rhs);

  enum tType {
    Null,
    Int,
    Double,
    Bool,
    String,
    Array,
    Object // == Map
  };

  tType Type() const {
    return type_;
  }

private:
  struct tNull {};

  std::variant<tNull, int, double, bool, std::string, TArrayPtr, TObjectPtr> value_;
  tType type_;
};

template <class T>
std::pair<TObject::iterator, bool> TObject::insert(std::string key, const T &v)
{
  // if key exists, DO NOT replace value
  auto it = find(key);
  if (it != values_.end())
    return std::make_pair(it, false);

  // push back new value, add index to index map
  auto n = values_.emplace(it, std::move(key), TValue(v));
  index_map_.emplace(n->first, values_.size() - 1);
  return std::make_pair(n, true);
}

}

#endif // CONTEXT_H
