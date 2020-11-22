## Table of Contents

* [About](#about)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Building](#building)
* [Usage](#usage)

## About

The goal of this project is to implement a Lex/Yacc based compiler for the Monga programming language - a simple procedural language - that generates LLVM code from Monga source files.

This is a project for the Compilers course (INF1715) at PUC-Rio.

## Getting Started

### Prerequisites

* C11 compliant compiler
* GNU Flex + Bison (scanner and parser generation);
* CMake 3.18 or higher (build system generator);
* Python (unit testing);
* LLVM + Clang (assembly of LLVM generated files);

## Building

Use the following command while in the root of the repository to setup the build system:
```
cmake -S . -B bin
```
Then, ```cd``` onto ```bin``` and call ```make```.
This will generate binaries for monrt (the Monga runtime library), libmonga (the Monga compiler library), monga (the Monga compiler executable) and montests (the Monga Unit test suite) inside their respective folders within ```bin/src```.

## Usage

### Compiling a monga program:
This mode will generate an LLVM file for every specified Monga source file.
```sh
monga -o <programOutputPath> <mongaSource1>[, <otherMongaSource>...]
```

### Token dumping mode:
The token dumping mode will read an input file and dump all tokens scanned by the Lexical Analyser.
```sh
monga -l <sourceFile>
```

### Tree dumping mode:
This mode will dump the Abstract Syntax Tree of an input source file in XML format after parsing it.
```sh
monga -p <sourceFile>
```

### Semantic test mode:
This mode will perform semantic analysis on Monga source files, outputting errors if they occur.
```sh
monga -s <sourceFile1> [<sourceFilen>...]
```