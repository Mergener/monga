## Table of Contents

* [About](#about)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Building](#building)
* [Usage](#usage)

## About

The goal of this project is to implement a Lex/Yacc based compiler for the Monga programming language - a simple procedural language - that will generate LLVM code from Monga source files.

This is a project for the Compilers course (INF1715) at PUC-Rio.

## Getting Started

### Prerequisites

* flex/lex
```sh
sudo apt-get install flex
```

* yacc/bison
```sh
sudo apt-get install bison
```

* gcc

Even though any C99 (or more recent) compliant compiler should be able to compile the code, the build scripts currently only support gcc.

```sh
sudo apt install gcc
```

### Building

The top-level scripts ```buildall```, ```buildlex``` and ```buildyacc``` are respectively responsible for building the entire monga compiler, the lexical analyser and the parser. Running them with all the pre-requisites being met should generate Lex/Yacc sources (and other outputs) on ```bin/```.

To run them, simply use:

```sh
./buildlex
./buildyacc
./buildall
```

Note: ```./buildall``` automatically runs ```./buildlex``` and ```./buildyacc```.

The ```bin/monga``` file is the generated executable file for the Monga compiler.

## Usage

### Compiling a monga program:
This mode will generate an executable file from a set of monga source files.
```sh
./bin/monga -o <programOutputPath> <mongaSource1>[, <otherMongaSource>...] [Extra Args]
```

### Token dumping mode:
The token dumping mode will read an input file and dump all tokens scanned by the Lexical Analyser.
```sh
./bin/monga -l <sourceFile> [Extra Args]
```

### Tree dumping mode:
This mode will dump the Abstract Syntax Tree of an input source file in XML format after parsing it.
```sh
./bin/monga -p <sourceFile> [Extra Args]
```