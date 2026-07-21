#pragma once

#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <numeric>
#include <span>
#include <vector>

#include "ISampleReader.h"
#include "IStatProcessor.h"
#include "MockReader.hpp"
#include "StatRow.h"

// NOLINTBEGIN

static constexpr std::vector<StatRow> drainAndProcessAllSamples( MockReader& reader, IStatProcessor& proc ) {
  std::vector<StatRow> out;
  while ( true ) {
    auto s = reader.nextSample();
    if ( s ) {
      out.push_back( proc.process( *s ) );
      continue;
    }
    if ( s.error() == ReadError::END_OF_FILE ) break;
    FAIL( "unexpected read error: " << static_cast<int>( s.error() ) );
  }
  return out;
}

static constexpr StatRow calculateExpectedRow( const std::vector<double>& samples, std::size_t index,
                                               std::size_t window_size ) {
  std::size_t window_begin = ( index + 1 >= window_size ) ? ( index + 1 - window_size ) : 0;
  std::size_t window_length = index - window_begin + 1;
  std::span<const double> window{ samples.data() + window_begin, window_length };

  double window_sum = std::accumulate( window.begin(), window.end(), 0.0 );
  double window_mean = window_sum / static_cast<double>( window_length );

  return StatRow{ OrgInputVal{ samples[index] }, MeanRolVal{ window_mean },
                  MaxRolVal{ *std::ranges::max_element( window ) }, MinRolVal{ *std::ranges::min_element( window ) } };
}

void checkRowApprox( const StatRow& a, const StatRow& e ) {
  CHECK( a.org_input_ == Catch::Approx( e.org_input_ ) );
  CHECK( a.mean_ == Catch::Approx( e.mean_ ) );
  CHECK( a.max_ == Catch::Approx( e.max_ ) );
  CHECK( a.min_ == Catch::Approx( e.min_ ) );
}

// NOLINTEND