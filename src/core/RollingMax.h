#pragma once

#include <type_traits>

#include "core/IRollingQueue.hpp"

template <typename TypeInQueue, int QueueSize>
requires( QueueSize > 1 && std::is_trivially_copyable_v<TypeInQueue> )
class RollingMax : public IRollingQueue<TypeInQueue, QueueSize, QueuePolicy::MAX> {};