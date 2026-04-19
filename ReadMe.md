# Silikego Core

<https://vdamewood.com/software/projects/silikego>

Silikego Core is a library written in C++ for parsing and evaluating
mathematical expressions. For example, if given the input `2 + 2`,
Silikego Core will yeild a result of `4`. It was made as a component of
an example for teaching software development. As the core of the
project, Silikego Core implements an infix parser for mathematical
expressions, and exposes an application programming interface (API) for
using the parser in another program.

## The Language

Silikego Core has a built-in infix parser that handles the following:
* 64-bit Integer values as a series of digits 0 - 9.
* 64-bit (double-precision) Floating-point values as a series of digits
  0 - 9 with a decimal point, or the special values `pi` and `e` which
  represent the values π and Euler's constant.
* Four-function calculations, such as `2 + 2`, `3 * 4.0`, `9.0 - 3.0`
  and `8 / 2`.
* Exponents with a `^` operator. For example `2^3` is `8`.
* Negative numbers, such as `-4`, `-4.0 + 20`.
* parenthetic expressions, such as `2 * (3 + 4)`.
* Random number generation using a `d` operator to simulate die rolls.
  Such as `3d6` or `1d20`.
* Functions: The syntax to call functions is
  `<function name>(<argument>[, ...])`. Functions take one or more
  arguments. The functions `acos`, `asin`, `atan`, `cbrt`, `ceil`,
  `cos`, `cosh`, `exp`, `floor`, `hypot`, `log`, `log2`, `log10`,
  `round`, `sin`, `sinh`, `sqrt`, `tan`, `tanh`, and `trunc` are
  available, and behave the same as their analogs in the C standard
  library. There is an `abs` function that will work as the `abs`
  function in the C standard library if the argument given is an
  integer, or the `fabs` functions if the argument given is a
  floating-point value.

Silikego Core ignores space and tab characters in its input, except to
separate tokens.

Silikego Core supports integer and floating-point arithmetic. In most
situations, integer expressions will yield integer results, and
floating-point expressions will yield floating-point results. The
exceptions are that most of the above-mentioned functions return
floating-point values, and any integer division where the result
would be truncated will result in a floating-point quotient.

## Quick Building

Silikego Core uses CMake (<https://www.cmake.org>) to generate its build
system. For the following example, these directories are used:

* Source Code: `$HOME/src/siiko-core`
* Build Directory: `$HOME/build/silikego-core`
* Staging/Install Directory: `$HOME/stage/silikego-core`

You may change these locations as long as you are consistent through the
process. To build Silikego Core, you can use CMake with the following
commands on macOS, Linux, BSD, and various Unix workalikes.

```Shell
mkdir -p $HOME/build/silikego-core
mkdir -p $HOME/stage/silikego-core
cmake -S $HOME/src/silikego-core -B $HOME/build/silikego-core -DCMAKE_BUILD_TYPE:String:Release
cmake --build $HOME/build/silikego-core
cmake --install $HOME/build/silikego-core --prefix $HOME/stage/silikego-core
```

You can use the following with PowerShell on Windows (Make sure
`cmake.exe` is in your `$ENV:PATH` value):

```PowerShell
New-Item -Type Directory -Force $HOME/build/silikego-core
New-Item -Type Directory -Force $HOME/stage/silikego-core
cmake -S $HOME/src/silikego-core -B $HOME/build/silikego-core
cmake --build $HOME/build/silikego-core --config Release
cmake --install $HOME/build/silikego-core --prefix $HOME/stage/silikego-core
```

After this, you may use the Silikego Core library in
`$HOME/stage/silikego-core`.

## License Information

Copyright 2012-2026 Vincent Damewood

Silikego Core is licensed under the terms of the GNU Lesser General
Public License, version 3 or later.

Silikego Core is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
[GNU Lesser General Public License](License.md) and the
[GNU General Public License](License.Gpl.md) for more details.
