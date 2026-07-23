#pragma once

#include <deque>
#include <expected>
#include <sstream>
#include <utility>
#include <vector>

#include "ISampleReader.h"

class MockReader : public ISampleReader {
 private:
  std::istringstream dummy_stream_;
  std::deque<double> sample_stream_;

 public:
  MockReader( const std::vector<double>& init_stream )
      : ISampleReader( dummy_stream_ ), sample_stream_( init_stream.begin(), init_stream.end() ) {
  }
  MockReader( std::deque<double> init_stream )
      : ISampleReader( dummy_stream_ ), sample_stream_( std::move( init_stream ) ) {
  }

  [[nodiscard]] bool validateColumnName() noexcept override {
    return true;
  }

  [[nodiscard]] std::expected<double, ReadError> nextSample() noexcept override {
    if ( sample_stream_.empty() ) {
      return std::unexpected{ ReadError::END_OF_FILE };
    }
    const double popped = sample_stream_.front();
    sample_stream_.pop_front();
    return popped;
  }
};