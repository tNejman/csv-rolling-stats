#pragma once

#include <utility>

template <class T, class Tag>
struct NamedVal {
  T value_;
  explicit NamedVal( T init_val ) : value_( std::move( init_val ) ) {
  }
  operator T() const {
    return value_;
  };
};