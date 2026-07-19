#pragma once

#include <expected>
#include <functional>
#include <istream>
#include <optional>
#include <string>

enum class ReadError { END_OF_FILE, PARSE_ERROR };

class ISampleReader {  // NOLINT(cppcoreguidelines-special-member-functions)
 private:
  std::reference_wrapper<std::istream> input_stream_;

 protected:
  std::optional<char> nextChar() {
    int char_read = input_stream_.get().get();
    if ( char_read == std::char_traits<char>::eof() ) {
      return std::nullopt;
    }
    return static_cast<char>( char_read );
  }

 public:
  ISampleReader( std::istream& stream ) : input_stream_( stream ) {
  }
  virtual ~ISampleReader() = default;
  virtual std::expected<double, ReadError> nextSample() = 0;
};