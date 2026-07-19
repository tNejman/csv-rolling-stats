#pragma once

#include <functional>

#include "ISampleReader.h"
#include "IStatProcessor.h"
#include "IStatWriter.h"
class Pipeline {
 private:
  std::reference_wrapper<ISampleReader> reader_;
  std::reference_wrapper<IStatProcessor> processor_;
  std::reference_wrapper<IStatWriter> writer_;

 public:
  Pipeline( ISampleReader& reader, IStatProcessor& processor, IStatWriter& writer )
      : reader_( reader ), processor_( processor ), writer_( writer ) {
  }
};