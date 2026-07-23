#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "StatRow.h"
#include "TestHelper.hpp"

// NOLINTBEGIN(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)

TEST_CASE( "hand-calc-data1" ) {
  constexpr int WINDOW_SIZE = 3;
  const std::vector<double> samples = { 1.0, 2.0, 5.0, 3.0, 4.0, 2.0 };
  std::vector<StatRow> expected_output;
  expected_output.emplace_back( OrgInputVal{ 1.0 }, MeanRolVal{ 1.0 }, MaxRolVal{ 1.0 }, MinRolVal{ 1.0 } );
  expected_output.emplace_back( OrgInputVal{ 2.0 }, MeanRolVal{ 1.5 }, MaxRolVal{ 2.0 }, MinRolVal{ 1.0 } );
  expected_output.emplace_back( OrgInputVal{ 5.0 }, MeanRolVal{ 2.666666 }, MaxRolVal{ 5.0 }, MinRolVal{ 1.0 } );
  expected_output.emplace_back( OrgInputVal{ 3.0 }, MeanRolVal{ 3.333333 }, MaxRolVal{ 5.0 }, MinRolVal{ 2.0 } );
  expected_output.emplace_back( OrgInputVal{ 4.0 }, MeanRolVal{ 4.0 }, MaxRolVal{ 5.0 }, MinRolVal{ 3.0 } );
  expected_output.emplace_back( OrgInputVal{ 2.0 }, MeanRolVal{ 3.0 }, MaxRolVal{ 4.0 }, MinRolVal{ 2.0 } );

  REQUIRE( samples.size() == expected_output.size() );
  for ( int i = 0; i < samples.size(); ++i ) {
    const StatRow calculated = test_helper::calculateExpectedRow( samples, i, WINDOW_SIZE );
    const StatRow& expected = expected_output.at( i );
    test_helper::checkRowApprox( calculated, expected );
  }
}

// NOLINTEND(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)