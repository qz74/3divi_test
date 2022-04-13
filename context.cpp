#include "context.h"

namespace context {

TValue TObject::operator[](const std::string &key) const
{
  return at(key);
}

TValue& TObject::operator[](const std::string &key)
{
  return at(key);
}

TValue TObject::at(const std::string &key) const
{
  auto it = index_map_.find(key);
  if (it != index_map_.end())
    return values_[it->second].second;

  throw std::exception();
}

TValue &TObject::at(const std::string &key)
{
  auto it = index_map_.find(key);
  if (it != index_map_.end())
    return values_[it->second].second;

  throw std::exception();
}


TObject::iterator TObject::find(const std::string &s)
{
  auto it = index_map_.find(s);
  if (it != index_map_.end())
    return values_.begin() + it->second;

  return values_.end();
}

TObject::const_iterator TObject::find(const std::string &s) const
{
  auto it = index_map_.find(s);
  if (it != index_map_.end())
    return values_.begin() + it->second;

  return values_.end();
}


TValue TArray::operator[](std::size_t n) const
{
  return at(n);
}

TValue &TArray::operator[](std::size_t n)
{
  return at(n);
}

TValue TArray::at(std::size_t n) const
{
  if (n < values_.size()) {
    return values_[n];
  }

  throw std::exception();
}

TValue &TArray::at(std::size_t n)
{
  if (n < values_.size()) {
    return values_[n];
  }

  throw std::exception();
}


TValue::TValue(const TArray &a)
  : type_(TValue::tType::Array)
{
  value_ = std::make_shared<TArray>(a);
}

TValue::TValue(const TObject &o)
  : type_(TValue::tType::Object)
{
  value_ = std::make_shared<TObject>(o);
}

TValue::TValue(TArray &&a)
  : type_(TValue::tType::Array)
{
  value_ = std::make_shared<TArray>(std::move(a));
}

TValue::TValue(TObject &&o)
  : type_(TValue::tType::Object)
{
  value_ = std::make_shared<TObject>(std::move(o));
}

TValue &TValue::operator=(TValue &&rhs)
{
  if (this != &rhs) {
    value_ = std::move(rhs.value_);
    type_  = std::move(rhs.type_);
  }

  return *this;
}

TValue &TValue::operator=(const TValue &rhs)
{
  if (this != &rhs) {
    value_ = rhs.value_;
    type_  = rhs.type_;
  }

  return *this;
}

}
