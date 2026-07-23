# CSV Rolling Stats

Command-line tool for automatic transformation of sampled-signal CSV files. For each input double it computes, over a sliding window of configurable size:

* rolling average
* rolling maximum
* rolling minimum

## Data Format

**Input**: `*.csv` file with a single column of floating-point values, semicolon and newline as separator. Column name has to be strictly `"Wejście"`.
**Output**: `*.csv` file, semicolon-separated, with 4 columns:

1. original data sample
2. rolling average
3. rolling maximum
4. rolling minimum

Output precision is limited to two decimal places. Until the window fills up, statistics are computed over all samples seen so far.

## Algorithm

The program utilizes a monotonic queue to calculate rolling minimum and maximum without recalculating from the whole window, resulting in O(n) time complexity rather than brute force O(n^2).

## Building

Requirements: GCC with C++23 support (26 for tests), CMake >= 4.x, Ninja, LLD

```sh
cmake --preset=gcc-ninja-lld -DCMAKE_BUILD_TYPE=Release
cmake --build build 
```

## Usage

[location of compilet binary is `/build/bin`]

```sh
csv-rolling-stats --in <input-filename> --out <output-filename>
```

### Example

For input (window size 3):

```
Wejście;
1.00;
2.00;
5.00;
3.00;
4.00;
2.00;
```

the output is:

```
Wejście; Avg; Max; Min;
1.00; 1.00; 1.00; 1.00;
2.00; 1.50; 2.00; 1.00;
5.00; 2.66; 5.00; 1.00;
3.00; 3.33; 5.00; 2.00;
4.00; 4.00; 5.00; 3.00;
2.00; 3.00; 4.00; 2.00;
```