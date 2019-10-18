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

## Running

```
cd run
echo "A{B,C}" | ../bin/braceexp
```

## Testing

```
cd run
../bin/unit_tests
```