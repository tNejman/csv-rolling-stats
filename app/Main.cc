#include <cstddef>
#include <fstream>
#include <utility>
#include <variant>

#include "ArgParser.hpp"
#include "ExitCodes.h"
#include "Pipeline.h"
#include "PipelineReturnToExitProcessor.h"
#include "core/StatProcessor.h"
#include "io/CsvReader.h"
#include "io/CsvWriter.h"

int main( int argc, char** argv )  // noexcept
{
  constexpr std::size_t SAMPLE_WINDOW_SIZE = 500;

  const ArgParser arg_parser{ argc, argv };
  if ( arg_parser.tryHandleHelpFlag() ) {
    return exit_codes::HELP_SHOWN_EXIT_CODE;
  }
  auto [input_file_name, output_file_name] = arg_parser.getFileNames();

  std::ifstream input_file{ input_file_name.value_ };
  if ( !input_file.is_open() ) {
    return exit_codes::BAD_INPUT_FILE_OPEN;
  }
  std::ofstream output_file{ output_file_name.value_ };
  if ( !output_file.is_open() ) {
    return exit_codes::BAD_OUTPUT_FILE_OPEN;
  }

  CsvReader csv_reader{ input_file };
  StatProcessor<SAMPLE_WINDOW_SIZE> stat_processor;
  CsvWriter csv_writer{ output_file };

  Pipeline pipeline{ csv_reader, stat_processor, csv_writer };

  auto run_res = pipeline.run();

  int const exit_code = pipelineReturnToExitCode( run_res );

  return exit_code;
};