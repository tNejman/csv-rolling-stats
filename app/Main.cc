#include <fstream>
#include <utility>

#include "ArgParser.hpp"

int main( int argc, char** argv ) {  // NOLINT(bugprone-exception-escape)
  // constexpr std::size_t SAMPLE_WINDOW_SIZE = 500;

  const ArgParser arg_parser{ argc, argv };
  if ( arg_parser.tryHandleHelpFlag() ) {
    return 1;
  }
  auto [input_file_name, output_file_name] = arg_parser.getFileNames();

  const std::ifstream input_file{ input_file_name.value_ };
  const std::ofstream output_file{ output_file_name.value_ };

  return 0;
};