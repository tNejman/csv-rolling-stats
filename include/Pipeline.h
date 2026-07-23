#pragma once

#include <functional>
#include <variant>

#include "ISampleReader.h"
#include "IStatProcessor.h"
#include "IStatWriter.h"

struct WriteError {};
struct NoError {};

class Pipeline {
 private:
  std::reference_wrapper<ISampleReader> reader_;
  std::reference_wrapper<IStatProcessor> processor_;
  std::reference_wrapper<IStatWriter> writer_;

 public:
  Pipeline( ISampleReader& reader, IStatProcessor& processor, IStatWriter& writer )
      : reader_( reader ), processor_( processor ), writer_( writer ) {
  }
  std::variant<ReadError, WriteError, NoError> run() noexcept {
    if ( !reader_.get().validateColumnName() ) {
      return ReadError::COLUMN_NAME_ERROR;
    }

    auto maybe_sample = reader_.get().nextSample();
    while ( maybe_sample ) {
      auto stat_row = processor_.get().process( *maybe_sample );
      if ( !writer_.get().writeStatRow( stat_row ) ) {
        return WriteError{};
      }
    }
    if ( maybe_sample.error() != ReadError::END_OF_FILE ) {
      return maybe_sample.error();
    }
    return NoError{};
  }
};