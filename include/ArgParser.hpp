#pragma once

#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "NamedVal.hpp"

using InputFilePath = NamedVal<std::filesystem::path, struct InputFilePathTag>;
using OutputFilePath = NamedVal<std::filesystem::path, struct OutputFilePathTag>;

class ArgParser {
 private:
  std::span<char*> concrete_arguments_;

 public:
  explicit ArgParser( int argc, char** argv ) noexcept : concrete_arguments_( argv, static_cast<std::size_t>( argc ) ) {
  }

  static constexpr bool HELP_MESSAGE_SHOWN = false;

  [[nodiscard]] bool tryHandleHelpFlag() const {
    constexpr int EXPECTED_ARG_COUNT_FOR_HELP = 2;
    constexpr int EXPECTED_PROGRAM_NAME_POS = 0;
    constexpr int EXPECTED_HELP_FLAG_POS = 1;
    constexpr std::string_view HELP_FLAG_LITERAL = "--help";
    constexpr std::string_view HELP_FLAG_LITERAL_SHORT = "-h";

    if ( concrete_arguments_.size() != EXPECTED_ARG_COUNT_FOR_HELP ) {
      return !HELP_MESSAGE_SHOWN;
    }

    static const std::string help_message = std::format(
        "Usage: {} --in <input-filename> --out <output-filename>\n\n"
        "  -h, --help     Show this help message and exit.\n",
        concrete_arguments_.at( EXPECTED_PROGRAM_NAME_POS ) );

    if ( concrete_arguments_.at( EXPECTED_HELP_FLAG_POS ) != HELP_FLAG_LITERAL
         && concrete_arguments_.at( EXPECTED_HELP_FLAG_POS ) != HELP_FLAG_LITERAL_SHORT ) {
      std::cout << "Invalid arguments.\n\n";
    }
    std::cout << help_message;
    return HELP_MESSAGE_SHOWN;
  }

  [[nodiscard]] std::pair<InputFilePath, OutputFilePath> getFileNames() const {
    constexpr int EXPECTED_ARG_COUNT_DEFAULT = 5;
    if ( concrete_arguments_.size() != EXPECTED_ARG_COUNT_DEFAULT ) {
      std::cout << std::format( "arg count does not match expected: {}\n\n", EXPECTED_ARG_COUNT_DEFAULT )
                << HELP_MESSAGE_SHOWN;
      std::exit( 1 );
    }

    constexpr int EXPECTED_INPUT_FILE_FLAG_POS = 1;
    constexpr int EXPECTED_INPUT_FILE_NAME_POS = 2;
    constexpr int EXPECTED_OUTPUT_FILE_FLAG_POS = 3;
    constexpr int EXPECTED_OUTPUT_FILE_NAME_POS = 4;

    constexpr std::string_view FLAG_INPUT_FILE = "--in";
    constexpr std::string_view FLAG_OUTPUT_FILE = "--out";

    if ( concrete_arguments_.at( EXPECTED_INPUT_FILE_FLAG_POS ) != FLAG_INPUT_FILE
         || concrete_arguments_.at( EXPECTED_OUTPUT_FILE_FLAG_POS ) != FLAG_OUTPUT_FILE ) {
      std::cout << "Invalid arguments.\n\n" << HELP_MESSAGE_SHOWN;
      std::exit( 1 );
    }
    return { InputFilePath{ concrete_arguments_.at( EXPECTED_INPUT_FILE_NAME_POS ) },
             OutputFilePath{ concrete_arguments_.at( EXPECTED_OUTPUT_FILE_NAME_POS ) } };
  }
};