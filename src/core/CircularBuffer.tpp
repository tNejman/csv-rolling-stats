#pragma once

#include <cstddef>
#include <optional>
#include <type_traits>

#include "core/CircularBuffer.h"  // NOLINT(misc-header-include-cycle)   // both have pragma, no circular include; needed here for linter support

template <class TypeInBuffer, int BufferSize>
requires( BufferSize > 1 && std::is_trivially_copyable_v<TypeInBuffer> )
void CircularBuffer<TypeInBuffer, BufferSize>::wrapAroundIncrement( int& var ) noexcept {
  ++var %= BufferSize;
}

template <class TypeInBuffer, int BufferSize>
requires( BufferSize > 1 && std::is_trivially_copyable_v<TypeInBuffer> )
void CircularBuffer<TypeInBuffer, BufferSize>::wrapAroundDecrement( int& var ) noexcept {
  if ( --var < 0 ) {
    var = BufferSize - 1;
  }
}

template <class TypeInBuffer, int BufferSize>
requires( BufferSize > 1 && std::is_trivially_copyable_v<TypeInBuffer> )
[[nodiscard]] bool CircularBuffer<TypeInBuffer, BufferSize>::empty() const noexcept {
  return elem_count_ == 0;
}

template <class TypeInBuffer, int BufferSize>
requires( BufferSize > 1 && std::is_trivially_copyable_v<TypeInBuffer> )
[[nodiscard]] bool CircularBuffer<TypeInBuffer, BufferSize>::full() const noexcept {
  return elem_count_ == BufferSize;
}

template <class TypeInBuffer, int BufferSize>
requires( BufferSize > 1 && std::is_trivially_copyable_v<TypeInBuffer> )
[[nodiscard]] std::size_t CircularBuffer<TypeInBuffer, BufferSize>::size() const noexcept {
  return elem_count_;
}

template <class TypeInBuffer, int BufferSize>
requires( BufferSize > 1 && std::is_trivially_copyable_v<TypeInBuffer> )
[[nodiscard]] TypeInBuffer CircularBuffer<TypeInBuffer, BufferSize>::getFront() const noexcept {
  return buf_[static_cast<size_t>( head_idx_ )];
}

template <class TypeInBuffer, int BufferSize>
requires( BufferSize > 1 && std::is_trivially_copyable_v<TypeInBuffer> )
[[nodiscard]] TypeInBuffer CircularBuffer<TypeInBuffer, BufferSize>::getBack() const noexcept {
  return buf_[static_cast<size_t>( tail_idx_ )];
}

template <class TypeInBuffer, int BufferSize>
requires( BufferSize > 1 && std::is_trivially_copyable_v<TypeInBuffer> )
[[nodiscard]] std::optional<TypeInBuffer> CircularBuffer<TypeInBuffer, BufferSize>::tryGetFront() const noexcept {
  if ( empty() ) {
    return std::nullopt;
  }
  return getFront();
}

template <class TypeInBuffer, int BufferSize>
requires( BufferSize > 1 && std::is_trivially_copyable_v<TypeInBuffer> )
[[nodiscard]] std::optional<TypeInBuffer> CircularBuffer<TypeInBuffer, BufferSize>::tryGetBack() const noexcept {
  if ( empty() ) {
    return std::nullopt;
  }
  return getBack();
}

template <class TypeInBuffer, int BufferSize>
requires( BufferSize > 1 && std::is_trivially_copyable_v<TypeInBuffer> )
void CircularBuffer<TypeInBuffer, BufferSize>::pushFront( TypeInBuffer new_val ) noexcept {
  if ( !empty() ) {
    wrapAroundDecrement( head_idx_ );
  }
  buf_[head_idx_] = new_val;
  ++elem_count_;
}

template <class TypeInBuffer, int BufferSize>
requires( BufferSize > 1 && std::is_trivially_copyable_v<TypeInBuffer> )
void CircularBuffer<TypeInBuffer, BufferSize>::pushBack( TypeInBuffer new_val ) noexcept {
  if ( !empty() ) {
    wrapAroundIncrement( tail_idx_ );
  }
  buf_[static_cast<std::size_t>( tail_idx_ )] = new_val;
  ++elem_count_;
}

template <class TypeInBuffer, int BufferSize>
requires( BufferSize > 1 && std::is_trivially_copyable_v<TypeInBuffer> )
[[nodiscard]] bool CircularBuffer<TypeInBuffer, BufferSize>::tryPushFront( TypeInBuffer new_val ) noexcept {
  if ( full() ) {
    return false;
  }
  pushFront( new_val );
  return true;
}

template <class TypeInBuffer, int BufferSize>
requires( BufferSize > 1 && std::is_trivially_copyable_v<TypeInBuffer> )
[[nodiscard]] bool CircularBuffer<TypeInBuffer, BufferSize>::tryPushBack( TypeInBuffer new_val ) noexcept {
  if ( full() ) {
    return false;
  }
  pushBack( new_val );
  return true;
}

template <class TypeInBuffer, int BufferSize>
requires( BufferSize > 1 && std::is_trivially_copyable_v<TypeInBuffer> )
void CircularBuffer<TypeInBuffer, BufferSize>::popFront() noexcept {
  --elem_count_;
  if ( !empty() ) {
    wrapAroundIncrement( head_idx_ );
  }
}

template <class TypeInBuffer, int BufferSize>
requires( BufferSize > 1 && std::is_trivially_copyable_v<TypeInBuffer> )
void CircularBuffer<TypeInBuffer, BufferSize>::popBack() noexcept {
  --elem_count_;
  if ( !empty() ) {
    wrapAroundDecrement( tail_idx_ );
  }
}

template <class TypeInBuffer, int BufferSize>
requires( BufferSize > 1 && std::is_trivially_copyable_v<TypeInBuffer> )
[[nodiscard]] bool CircularBuffer<TypeInBuffer, BufferSize>::tryPopFront() noexcept {
  if ( empty() ) {
    return false;
  }
  popFront();
  return true;
}

template <class TypeInBuffer, int BufferSize>
requires( BufferSize > 1 && std::is_trivially_copyable_v<TypeInBuffer> )
[[nodiscard]] bool CircularBuffer<TypeInBuffer, BufferSize>::tryPopBack() noexcept {
  if ( empty() ) {
    return false;
  }
  popBack();
  return true;
}