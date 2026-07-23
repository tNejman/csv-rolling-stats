#include <catch2/catch_test_macros.hpp>
#include <format>
#include <sstream>

#include "Pipeline.h"
#include "core/StatProcessor.h"
#include "io/CsvReader.h"
#include "io/CsvWriter.h"
#include "io/IoLib.hpp"

// NOLINTBEGIN

TEST_CASE( "good-path" ) {
  std::stringstream in{ "Wejście;\n1.0;\n" };
  std::stringstream out{};

  CsvReader reader{ in };
  StatProcessor<500> processor{};
  CsvWriter writer{ out };

  Pipeline pipeline{ reader, processor, writer };
  pipeline.run();
  REQUIRE( out.str() == std::format( "{}1.00; 1.00; 1.00; 1.00; \n", io_lib::COLUMN_NAMES_OUT ) );
}

// NOLINTEND