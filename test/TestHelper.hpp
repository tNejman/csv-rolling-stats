#pragma once

#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <numeric>
#include <span>
#include <type_traits>
#include <vector>

#include "ISampleReader.h"
#include "IStatProcessor.h"
#include "MockReader.hpp"
#include "StatRow.h"
#include "core/StatProcessor.h"

namespace test_helper {
constexpr inline int SMALL_WINDOW_SIZE = 3;
constexpr inline int EQUAL_WINDOW_SIZE = 6;
constexpr inline int BIG_WINDOW_SIZE = 9;

constexpr inline std::array<int, 3> WINDOW_SIZES = { SMALL_WINDOW_SIZE, EQUAL_WINDOW_SIZE, BIG_WINDOW_SIZE };

static constexpr std::vector<StatRow> drainAndProcessAllSamples( MockReader& reader, IStatProcessor& proc ) {
  std::vector<StatRow> out;
  while ( true ) {
    auto sample = reader.nextSample();
    if ( sample ) {
      out.push_back( proc.process( *sample ) );
      continue;
    }
    if ( sample.error() == ReadError::END_OF_FILE ) {
      break;
    }
    FAIL( "unexpected read error: " << static_cast<int>( sample.error() ) );
  }
  return out;
}

static constexpr StatRow calculateExpectedRow( const std::vector<double>& samples, std::size_t index,
                                               std::size_t window_size ) {
  std::size_t window_begin = ( index + 1 >= window_size ) ? ( index + 1 - window_size ) : 0;
  std::size_t window_length = index - window_begin + 1;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  std::span<const double> window{ samples.data() + window_begin, window_length };

  double window_sum = std::accumulate( window.begin(), window.end(), 0.0 );
  double window_mean = window_sum / static_cast<double>( window_length );

  return StatRow{ OrgInputVal{ samples[index] }, MeanRolVal{ window_mean },
                  MaxRolVal{ *std::ranges::max_element( window ) }, MinRolVal{ *std::ranges::min_element( window ) } };
}

void checkRowApprox( const StatRow& result, const StatRow& expected ) {
  CHECK( result.org_input_ == Catch::Approx( expected.org_input_ ) );
  CHECK( result.mean_ == Catch::Approx( expected.mean_ ) );
  CHECK( result.max_ == Catch::Approx( expected.max_ ) );
  CHECK( result.min_ == Catch::Approx( expected.min_ ) );
}

template <typename T>
void checkValApprox( T res, const StatRow& expected ) {  // NOLINT(readability-function-cognitive-complexity) //
                                                         // clang-tidy counts expansion of CHECK macro
  if constexpr ( std::is_same_v<T, OrgInputVal> ) {
    CHECK( res.value_ == Catch::Approx( expected.org_input_ ) );
  } else if constexpr ( std::is_same_v<T, MeanRolVal> ) {
    CHECK( res.value_ == Catch::Approx( expected.mean_ ) );
  } else if constexpr ( std::is_same_v<T, MaxRolVal> ) {
    CHECK( res.value_ == Catch::Approx( expected.max_ ) );
  } else if constexpr ( std::is_same_v<T, MinRolVal> ) {
    CHECK( res.value_ == Catch::Approx( expected.min_ ) );
  }
}

void testBodyStatProcessor( const std::vector<double>& input ) {
  template for ( constexpr auto window_size : WINDOW_SIZES ) {
    CAPTURE( window_size );
    MockReader mock_reader{ input };
    StatProcessor<window_size> stat_processor{};
    auto output = drainAndProcessAllSamples( mock_reader, stat_processor );

    REQUIRE( output.size() == input.size() );
    for ( std::size_t i = 0; i < output.size(); ++i ) {
      CAPTURE( i );
      checkRowApprox( output.at( i ), calculateExpectedRow( input, i, window_size ) );
    }
  }
}

// NOLINTBEGIN(readability-identifier-length)
struct MeanOps {
  template <std::size_t N>
  using Proc = RollingMean<double, N>;
  static void feed( auto& p, double v ) {
    p.addElem( v );
  }
  static auto read( auto& p ) {
    return MeanRolVal{ p.getMean() };
  }
};

struct MaxOps {
  template <std::size_t N>
  using Proc = RollingMax<double, N>;
  static void feed( auto& p, double v ) {
    p.push( v );
  }
  static auto read( auto& p ) {
    return MaxRolVal{ p.getOptimum() };
  }
};

struct MinOps {
  template <std::size_t N>
  using Proc = RollingMin<double, N>;
  static void feed( auto& p, double v ) {
    p.push( v );
  }
  static auto read( auto& p ) {
    return MinRolVal{ p.getOptimum() };
  }
};

// NOLINTEND(readability-identifier-length)

template <typename Ops>
constexpr void testBodyRolling( const std::vector<double>& input ) {
  template for ( constexpr auto window_size : WINDOW_SIZES ) {
    CAPTURE( window_size );
    typename Ops::template Proc<window_size> proc{};
    std::vector<decltype( Ops::read( proc ) )> output;
    for ( const auto& inp : input ) {
      Ops::feed( proc, inp );
      output.push_back( Ops::read( proc ) );
    }
    REQUIRE( output.size() == input.size() );
    for ( std::size_t i = 0; i < output.size(); ++i ) {
      CAPTURE( i );
      checkValApprox( output[i], calculateExpectedRow( input, i, window_size ) );
    }
  }
}
}  // namespace test_helper
