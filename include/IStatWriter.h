#pragma once

#include <array>
#include <functional>
#include <ios>
#include <ostream>
#include <string_view>

#include "StatRow.h"
#include "io/IoLib.hpp"

class IStatWriter {  // NOLINT(cppcoreguidelines-special-member-functions)
 private:
  std::reference_wrapper<std::ostream> output_stream_;

 protected:
  bool writeChar( char char_to_write ) noexcept {
    output_stream_.get().exceptions( std::ios_base::goodbit );
    output_stream_.get() << char_to_write;
    return !output_stream_.get().fail();
  }
  bool writeCharBuf( std::array<char, io_lib::DOUBLE_STR_BUFFER_SIZE>& buf_to_write ) noexcept {
    output_stream_.get().exceptions( std::ios_base::goodbit );
    output_stream_.get() << buf_to_write.data();
    // output_stream_.get().flush();
    return !output_stream_.get().fail();
  }
  bool writeStringView( std::string_view str_to_write ) noexcept {
    output_stream_.get().exceptions( std::ios_base::goodbit );
    output_stream_.get() << str_to_write;
    return !output_stream_.get().fail();
  }

 public:
  IStatWriter( std::ostream& stream ) noexcept : output_stream_( stream ) {
  }
  virtual ~IStatWriter() noexcept = default;
  virtual bool writeStatRow( StatRow ) noexcept = 0;
};