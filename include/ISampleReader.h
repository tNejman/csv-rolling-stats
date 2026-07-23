#pragma once

#include <array>
#include <expected>
#include <functional>
#include <ios>
#include <istream>

#include "io/IoLib.hpp"

enum class ReadError { END_OF_FILE, PARSE_ERROR, HARDWARE_ISUE };
enum class ReadLineError { END_OF_FILE, HARDWARE_ISSUE, TOO_LONG_LINE };

class ISampleReader {  // NOLINT(cppcoreguidelines-special-member-functions)
 private:
  std::reference_wrapper<std::istream> input_stream_;
  std::array<char, io_lib::DOUBLE_STR_BUFFER_SIZE> line_buffer_;

 protected:
  [[nodiscard]] std::expected<std::array<char, io_lib::DOUBLE_STR_BUFFER_SIZE>, ReadLineError>
  nextLine() noexcept {  // ok return arr by value; small enough
    input_stream_.get().getline( line_buffer_.data(), static_cast<std::streamsize>( line_buffer_.size() ) );
    if ( input_stream_.get().bad() ) {
      return std::unexpected{ ReadLineError::HARDWARE_ISSUE };
    }
    if ( input_stream_.get().fail()
         && input_stream_.get().gcount() == static_cast<std::streamsize>( line_buffer_.size() ) - 1 ) {
      return std::unexpected{ ReadLineError::TOO_LONG_LINE };
    }
    if ( input_stream_.get().eof() && input_stream_.get().gcount() == 0 ) {
      return std::unexpected{ ReadLineError::END_OF_FILE };
    }
    return line_buffer_;
  }

 public:
  ISampleReader( std::istream& stream ) : input_stream_( stream ), line_buffer_() {
  }
  virtual ~ISampleReader() = default;
  [[nodiscard]] virtual std::expected<double, ReadError> nextSample() noexcept = 0;
};