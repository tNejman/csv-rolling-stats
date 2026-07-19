# CSV Rolling Stats

Command-line tool for automatic transformation of sampled-signal CSV files. For each input double it computes, over a sliding window of configurable size:

* rolling average
* rolling maximum
* rolling minimum

## Data Format

**Input**: `*.csv` file with a single column of floating-point values, semicolon as separator.
**Output**: `*.csv` file, semicolon-separated, with 4 columns:

1. original data sample
2. rolling average
3. rolling maximum
4. rolling minimum

Output precision is limited to two decimal places. Until the window fills up, statistics are computed over all samples seen so far.

## Algorithm

@TODO

## Building

Requirements: GCC with C++23 support, CMake >= 4.x, Make >= 4.x, LD
[Optional requirements for alternative toolchain: Clang with C++23 support, Ninja, LLD]

```sh
cmake --preset=gcc-make-ld
cmake --build build -j
```

OR

```sh
cmake --preset=clang-ninja-lld
cmake --build build
```

## Usage

```sh
universal-csv-transformer --in <input-filename> --out <output-filename>
```

### Example

For input (window size 3):

```
1.00;
2.00;
5.00;
3.00;
4.00;
2.00;
```

the output is:

```
1.00;1.00;1.00;1.00;
2.00;1.50;2.00;1.00;
5.00;2.66;5.00;1.00;
3.00;3.33;5.00;2.00;
4.00;4.00;5.00;3.00;
2.00;3.00;4.00;2.00;
```