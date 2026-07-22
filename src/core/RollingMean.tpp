#pragma once

#include "core/RollingMean.h"  // NOLINT(misc-header-include-cycle)   // both have pragma, no circular include; needed here for linter support

template <Numeric TypeInBuf, int WindowSize>
requires( WindowSize > 1 )
void RollingMean<TypeInBuf, WindowSize>::addElem( TypeInBuf new_elem ) noexcept {
  if ( buf_.full() ) {
    TypeInBuf oldest_val = buf_.getFront();
    current_sum_ -= oldest_val;
    buf_.popFront();
  } else {
    ++current_elem_count_;
  }
  buf_.pushBack( new_elem );
  current_sum_ += new_elem;
}

template <Numeric TypeInBuf, int WindowSize>
requires( WindowSize > 1 )
TypeInBuf RollingMean<TypeInBuf, WindowSize>::getMean() noexcept {
  return current_sum_ / static_cast<TypeInBuf>( current_elem_count_ );
}