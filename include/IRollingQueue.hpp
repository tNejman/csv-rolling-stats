#pragma once

#include <functional>
#include <type_traits>

#include "core/CircularBuffer.h"

enum class QueuePolicy { MIN, MAX };

template <typename TypeInQueue, int QueueSize, QueuePolicy Policy>
requires( QueueSize > 1 && std::is_trivially_copyable_v<TypeInQueue> )
class IRollingQueue {
 private:
  struct QueuePos {
    TypeInQueue value_;
    int index_;
  };

  CircularBuffer<QueuePos, QueueSize> deque_{};
  int current_index_ = 0;

  static constexpr auto COMPARE = []( const TypeInQueue& left_op, const TypeInQueue& right_op ) {
    if constexpr ( Policy == QueuePolicy::MIN ) {
      return std::greater_equal<TypeInQueue>{}( left_op, right_op );
    } else {
      return std::less_equal<TypeInQueue>{}( left_op, right_op );
    }
  };

 public:
  IRollingQueue() = default;
  void push( const TypeInQueue& value ) noexcept;
  [[nodiscard]] TypeInQueue getOptimum() const noexcept;
  [[nodiscard]] bool isWindowFull() const noexcept;
};

template <typename TypeInQueue, int QueueSize, QueuePolicy Policy>
requires( QueueSize > 1 && std::is_trivially_copyable_v<TypeInQueue> )
void IRollingQueue<TypeInQueue, QueueSize, Policy>::push( const TypeInQueue& value ) noexcept {
  if ( !deque_.empty() && deque_.getFront().index_ + QueueSize <= current_index_ ) {
    deque_.popFront();
  }
  while ( !deque_.empty() && COMPARE( deque_.getBack().value_, value ) ) {
    deque_.popBack();
  }
  deque_.pushBack( QueuePos{ .value_ = value, .index_ = current_index_ } );
  ++current_index_;
}

template <typename TypeInQueue, int QueueSize, QueuePolicy Policy>
requires( QueueSize > 1 && std::is_trivially_copyable_v<TypeInQueue> )
[[nodiscard]] TypeInQueue IRollingQueue<TypeInQueue, QueueSize, Policy>::getOptimum() const noexcept {
  return deque_.getFront().value_;
}

template <typename TypeInQueue, int QueueSize, QueuePolicy Policy>
requires( QueueSize > 1 && std::is_trivially_copyable_v<TypeInQueue> )
[[nodiscard]] bool IRollingQueue<TypeInQueue, QueueSize, Policy>::isWindowFull() const noexcept {
  return current_index_ >= QueueSize;
}