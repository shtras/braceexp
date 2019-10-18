# Bash style brace expander

**Contents**<br>
[Installation](#installation)<br>
[Running](#running)<br>
[Testing](#testing)<br>

## Installation

```
git clone https://github.com/shtras/braceexp
cd braceexp
mkdir build run
cd build
cmake -DENABLE_TESTS=1 -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

For Windows Visual Studio 2017 dsolution generation add `-G "Visual Studio 15 2017 Win64"` to the first cmake command

## Running

```
cd run
echo "A{B,C}" | ../bin/braceexp
```

## Testing

### On Linux/Unix based system:

```
cd run
../bin/unit_tests
../tests/test.sh
```

### On Windows (Visual Studio):

Build `RUN_TESTS` project or run `unit_tests` project