#pragma once

#include <string_view>
namespace io_lib {
constexpr inline int DOUBLE_STR_BUFFER_SIZE = 64;

constexpr std::string_view COLUMN_NAMES_OUT = "Wejście; Avg; Max; Min;\n";
constexpr std::string_view COLUMN_NAMES_IN = "Wejście;\n";

}  // namespace io_lib