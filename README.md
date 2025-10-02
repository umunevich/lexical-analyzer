# Lexical analyzer for Visual Basic

This is a C++ project that performs lexical analysis of source code written in Visual Basic.
The analyzer processes input files and splits the code into lexemes such as:

- Identifiers
- Keywords
- Literals (string, numeric, boolean, date/time, hex, octal, etc.)
- Comments
- Operators
- Separators

# To build:
## Create build directory
```
mkdir build && cd build
```
## Configure with CMake
```
cmake ..
```
## Build
```
cmake --build .
```
# To run:

Analyze a Visual Basic source file:
```
./LexicalAnalysis ../Test_inputs/test1.txt
```

[Input file (sample)](https://github.com/umunevich/lexical-analyzer/tree/0f14dee683030b57100440bc1e85639e6fd8a63a/Test_inputs)

[Grammar source link](https://learn.microsoft.com/en-us/dotnet/visual-basic/reference/language-specification/lexical-grammar#characters-and-lines)


