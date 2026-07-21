#pragma once

#include <functional>
#include <ostream>

#include "StatRow.h"

class IStatWriter {  // NOLINT(cppcoreguidelines-special-member-functions)
 private:
  std::reference_wrapper<std::ostream> output_stream_;

 protected:
  virtual void writeChar( char ) = 0;

 public:
  IStatWriter( std::ostream& stream ) noexcept : output_stream_( stream ) {
  }
  virtual ~IStatWriter() noexcept = default;
  virtual void writeStatRow( StatRow ) noexcept = 0;
};