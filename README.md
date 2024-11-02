# Process-Based Caesar Cipher System

## Table of Contents
1. [Introduction](#introduction)
2. [Project Overview](#project-overview)
3. [Project Structure](#project-structure)
4. [How to Run the Project](#how-to-run-the-project)
5. [Implementation Details](#implementation-details)
6. [References](#references)

## Introduction
This project implements a multi-process system for encoding and decoding text using the Caesar cipher method. The system uses interprocess communication through named pipes (`NamedPipe`) to coordinate the work between various modules.

## Project Overview
The project is divided into the following main components:
- **Main Process**: Manages the creation and coordination of the `finder`, `decoder`, and `placer` processes.
- **Finder**: Identifies and reads input data to be processed.
- **Decoder**: Applies the Caesar cipher algorithm to decode the data.
- **Placer**: Stores and outputs the decoded data for further use or display.

The project demonstrates concepts such as process creation using `fork`, interprocess communication, and text processing algorithms.

## Project Structure
```
project-root-directory/
├── MainProcess.c
├── finder.c
├── decoder.c
├── placer.c
├── README.md
└── additional-support-files
```

## How to Run the Project
1. **Compilation**:
   Ensure you have `gcc` or a compatible C compiler installed. Compile the code with the following command:
   ```bash
   gcc -o main MainProcess.c finder.c decoder.c placer.c
   ```

2. **Execution**:
   Run the compiled program:
   ```bash
   ./main
   ```

## Implementation Details
- **Interprocess Communication**: Named pipes (`NamedPipe`) are used to facilitate communication between the `finder`, `decoder`, and `placer`. This ensures data flows efficiently from reading to processing to output.
- **Algorithm**:
  - **Caesar Cipher**: A simple text encryption algorithm where each character in the text is shifted by a fixed number of positions in the alphabet. The `decoder` module reverses this process by shifting the characters back to their original positions.
- **Concurrency**:
  - The `fork` system call is used to create child processes (`finder`, `decoder`, `placer`), allowing them to run concurrently.
  - The `finder` reads input and sends it through the `NamedPipe` to the `decoder`, which processes the data and sends it to the `placer` for final handling.

## References
- [Caesar Cipher - ScienceDirect](https://www.sciencedirect.com/topics/computer-science/caesar-cipher)
- [Named Pipe FIFO Example - GeeksforGeeks](https://www.geeksforgeeks.org/named-pipe-fifo-example-c-program/)
