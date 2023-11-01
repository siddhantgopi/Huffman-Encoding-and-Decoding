This project is a C program that implements the Huffman coding algorithm.

Key Components:
1. Data Structures: 
   - The program defines two primary data structures - `Node` and `MinHeap`.
   - `Node` is used to represent individual nodes within the Huffman tree.
   - `MinHeap` is employed to maintain a min-heap data structure, crucial for the efficient construction of the Huffman tree.

2. Functions:
   - The code includes functions for creating and manipulating `Node` structures.
   - These functions enable the construction of the Huffman tree and handle operations like character frequencies, node creation, and more.

3. Building Huffman Tree:
   - The `buildHuffmanTree` function takes an input filename as its parameter and reads the file.
   - Based on the character frequencies within the file, it constructs a Huffman tree.
   - This step is pivotal for the Huffman encoding process.

4. Encoding and Decoding:
   - The program provides functions for encoding and decoding text using the Huffman tree.
   - Users can save the encoded or decoded text to files, rendering it suitable for both data compression and decompression tasks.

Graphical User Interface:
5. User-Friendly Interface:
   - The project features a graphical user interface, tailored for Windows.
   - It includes buttons and file dialogs that facilitate user interaction, making the process more accessible and user-friendly.

6. Main Function:
   - The `WinMain` function sets up the main window and initiates a message loop, which is typical for Windows GUI applications.

Usage:
7. File Operations:
   - Users can select input files, build Huffman trees, encode input files, and save the encoded data.
   - There's also an option for selecting output files to save decoded text, decode input files, and save the decoded data.

Conclusion:
8. Overall Purpose:
   - This project aims to simplify the process of data compression and decompression using the Huffman coding technique.
   - It offers a robust and user-friendly solution for efficiently compressing and decompressing text files, serving purposes like data transmission and storage optimization.

To be noted: There are tow files, huffman.c containing the raw code and main.c containing the working GUI.
