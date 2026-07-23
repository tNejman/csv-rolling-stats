#pragma once

namespace exit_codes {
constexpr inline int DEF_EXIT_CODE = 0;

constexpr inline int BAD_INPUT_FILE_OPEN = 1;
constexpr inline int BAD_OUTPUT_FILE_OPEN = 2;

constexpr inline int PARSE_ERROR_IN_INPUT_FILE_CODE = 3;
constexpr inline int HARDWARE_ISSUE_CODE = 4;

constexpr inline int WRITE_ERROR_CODE = 5;

constexpr inline int FLAG_PARSE_ERROR = 6;

constexpr inline int HELP_SHOWN_EXIT_CODE = 7;
}  // namespace exit_codes