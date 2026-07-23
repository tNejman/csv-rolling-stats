#pragma once

#include <utility>
#include <variant>

#include "ExitCodes.h"
#include "ISampleReader.h"
#include "Pipeline.h"

namespace support {

// NOLINTBEGIN
template <class... Ts>
struct Overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
Overloaded( Ts... ) -> Overloaded<Ts...>;
// NOLINTEND

}  // namespace support

// NOLINTBEGIN(bugprone-exception-escape)
// std::visit doesn't throw unless valueless_by_exception which cannot happen because all ctors are noexcept
static inline int pipelineReturnToExitCode( std::variant<ReadError, WriteError, NoError> p_ret ) noexcept {
  return std::visit( support::Overloaded{ []( ReadError read_error ) noexcept {
                                           switch ( read_error ) {
                                             case ReadError::END_OF_FILE: return exit_codes::DEF_EXIT_CODE;
                                             case ReadError::PARSE_ERROR:
                                               return exit_codes::PARSE_ERROR_IN_INPUT_FILE_CODE;
                                             case ReadError::HARDWARE_ISUE: return exit_codes::HARDWARE_ISSUE_CODE;
                                             case ReadError::COLUMN_NAME_ERROR:
                                               return exit_codes::INVALID_INPUT_FILE_STYLE;
                                           }
                                           std::unreachable();
                                         },
                                          []( WriteError ) noexcept { return exit_codes::WRITE_ERROR_CODE; },
                                          []( NoError ) noexcept { return exit_codes::DEF_EXIT_CODE; } },
                     p_ret );
}
// NOLINTEND(bugprone-exception-escape)