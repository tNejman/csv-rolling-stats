#pragma once

#include <type_traits>

#include "core/CircularBuffer.h"

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <Numeric TypeInBuf, int WindowSize>
requires( WindowSize > 1 )
class RollingMean {
 private:
  CircularBuffer<TypeInBuf, WindowSize> buf_{};
  TypeInBuf current_sum_ = 0.0;
  int current_elem_count_ = 0;

 public:
  RollingMean() = default;

  void addElem( TypeInBuf new_elem ) noexcept;
  TypeInBuf getMean() noexcept;
};

#include "RollingMean.tpp"
