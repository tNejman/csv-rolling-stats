#pragma once

#include <array>
#include <cstddef>
#include <optional>
#include <type_traits>

template <class TypeInBuffer, int BufferSize>
requires( BufferSize > 1 && std::is_trivially_copyable_v<TypeInBuffer> )
class CircularBuffer {
 private:
  std::array<TypeInBuffer, BufferSize> buf_{};
  int head_idx_ = 0;  // latest front used slot
  int tail_idx_ = 0;  // latest back used slot
  int elem_count_ = 0;

  void wrapAroundIncrement( int& var ) noexcept;
  void wrapAroundDecrement( int& var ) noexcept;

 public:
  [[nodiscard]] bool empty() const noexcept;
  [[nodiscard]] bool full() const noexcept;
  [[nodiscard]] std::size_t size() const noexcept;

  [[nodiscard]] TypeInBuffer getFront() const noexcept;
  [[nodiscard]] TypeInBuffer getBack() const noexcept;

  [[nodiscard]] std::optional<TypeInBuffer> tryGetFront() const noexcept;
  [[nodiscard]] std::optional<TypeInBuffer> tryGetBack() const noexcept;

  void pushFront( TypeInBuffer new_val ) noexcept;
  void pushBack( TypeInBuffer new_val ) noexcept;

  [[nodiscard]] bool tryPushFront( TypeInBuffer new_val ) noexcept;
  [[nodiscard]] bool tryPushBack( TypeInBuffer new_val ) noexcept;

  void popFront() noexcept;
  void popBack() noexcept;

  [[nodiscard]] bool tryPopFront() noexcept;
  [[nodiscard]] bool tryPopBack() noexcept;
};

#include "core/CircularBuffer.tpp"