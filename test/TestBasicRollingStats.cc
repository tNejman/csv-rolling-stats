#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "TestHelper.hpp"

/* ==== MEAN ==== */

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

TEST_CASE( "baseline-mean" ) {
  test_helper::testBodyRolling<test_helper::MeanOps>( { 1.0, 2.0, 5.0, 3.0, 4.0, 2.0 } );
}

/* ==== MAX ==== */

TEST_CASE( "baseline-max" ) {
  test_helper::testBodyRolling<test_helper::MaxOps>( { 1.0, 2.0, 5.0, 3.0, 4.0, 2.0 } );
}

/* ==== MIN ==== */

TEST_CASE( "baseline-min" ) {
  test_helper::testBodyRolling<test_helper::MaxOps>( { 1.0, 2.0, 5.0, 3.0, 4.0, 2.0 } );
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)