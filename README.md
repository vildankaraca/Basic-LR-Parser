# 📝 SWE 204 - Homework 1: LR Parser

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)

> **Student Name:** Vildan Karaca  
> **Student ID:** B231202027

---

## 📖 1. Description
This project implements an LR parser for basic arithmetic expressions. The parser is written in C++ and utilizes Standard Template Library (STL) data structures such as `std::vector` and `std::map` to handle stack operations and parsing tables. It reads the provided Grammar, Action, and Goto tables to process given input files and generates a step-by-step trace table along with a parse tree for each expression.

## ⚙️ 2. How to Compile
To compile the source code, open your terminal (command prompt), navigate to the directory containing `LR-parser.cpp` and the table `.txt` files, and run the following command:
```bash
g++ LR-parser.cpp -o LR-parser
```

## 🚀 3. How to Run and Test
The program is designed to accept command-line arguments for dynamic testing. You do not need to recompile the code for different input files. To run the parser, use the executable name followed by the target input file and the desired output file name:

```bash
./LR-parser <input_file.txt> <output_file.txt>
```

** ✅ Example Usage (Valid Expression)**

```bash
./LR-parser input1.txt output1.txt
```
When executed, the program will load the tables, parse the tokens in input1.txt, and generate output1.txt. The output file will include a formatted trace table showing stack states and actions, followed by the generated parse tree.

** ❌ Example Usage (Erroneous Expression)**

```bash
./LR-parser input7.txt output7.txt
```
If the input file contains a syntax error or an unknown token, the parser will detect it and **print an error message directly to the terminal**. It will safely stop the parsing process and output the partial trace table along with the partial parse tree up to the point of failure into the output file, matching the required error-handling behavior.
