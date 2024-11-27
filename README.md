**Huffman-Enhanced-Pathfinding-with-C-Data-Structures**


Nikhil Patel

**Purpose**


I created this program to compress or decompress any type of file using Huffman Coding, an efficient and lossless data compression algorithm. This program processes files to either reduce their size for storage or decompress them back to their original form while preserving all data integrity. It is designed to handle a wide variety of file types and ensures optimized data storage through a systematic implementation of the Huffman Coding methodology.

**Compression and Encoding**


The primary goal of this project is to implement a functional compression tool that reduces file sizes by identifying patterns in data. For example, repetitive sequences such as "AAAAAAAAAAAA" can be compressed into shorter binary representations using Huffman Coding, which assigns the shortest binary codes to the most frequently occurring symbols. This ensures efficient storage without any data loss.

Huffman Coding, a lossless compression algorithm, differs from lossy compression methods like JPEG by ensuring that no data is discarded. Instead, it reorganizes and optimizes data representation. This makes it particularly suitable for text files or any files requiring exact preservation. While this program can handle any type of file, its compression effectiveness depends on the file's structure. Files with recurring patterns, like text files, compress significantly better than those with highly random data.

The algorithm's efficiency comes from its ability to compress data at the byte level. Each byte or symbol in a file is represented in binary, and the Huffman Coding algorithm ensures shorter representations for frequently occurring bytes. This is particularly effective for files with a predictable or repetitive pattern, such as logs or plain text files.

**Testing and Compression Examples**


I tested the program extensively using various file types, including text files, images, and PDFs. For example, compressing an image that was already reduced using JPEG compression did not result in a noticeable size reduction. This highlights the limitation of applying Huffman Coding to already compressed files. However, testing the program on text files showed remarkable results. For instance, the text "My name is Nikhil, I am a computer science student at UCSC" was reduced from 82 bytes to approximately 45.1 bytes. This demonstrated a compression ratio of about 0.55, showcasing the program’s effectiveness for text-based data.

In addition, I calculated compression ratios by comparing the original and compressed sizes. For a text-based example, Huffman Coding achieved over 40% size reduction, further validating its suitability for files with repeating patterns or high symbol frequency.

**Implementation Details**


The program is structured around several key components that work together to execute Huffman Coding. Each component serves a unique role in either the compression or decompression process.

BitWriter and BitReader: These modules handle reading and writing at the bit level. The BitWriter manages binary data output, writing bits to files and ensuring that file buffers are properly managed. BitReader handles reading bits from files, processing binary sequences, and preparing them for decoding.

Node Structure: Nodes are the building blocks of the Huffman tree. Each node represents a symbol and its frequency in the input file. Using functions like node_create, I initialized nodes with the necessary attributes and structured them into a binary tree. This tree enables efficient traversal to assign binary codes to symbols based on their frequency.

Priority Queue: The priority queue was used to manage nodes during the Huffman tree construction. Nodes with lower frequencies were dequeued and combined first to ensure an optimal tree structure. The queue operations, such as enqueue and dequeue, maintained order and enabled efficient tree building.

Huffman Compression and Decompression:

Compression: The compression process begins by creating a histogram of symbol frequencies. This histogram is used to construct the Huffman tree, which assigns shorter binary codes to frequently occurring symbols. The encoded file is written using these codes, significantly reducing its size.
Decompression: During decompression, the Huffman tree is reconstructed from the encoded file. The program reads bits sequentially and traverses the tree to decode the binary sequences back into their original symbols, restoring the file to its initial state.
How to Use the Program
The program provides functionality for both compression and decompression. To compress a file, the command syntax is as follows:


huff -i infile -o outfile
Here, infile is the input file to be compressed, and outfile is the destination for the compressed file. Similarly, to decompress a file, the command is:


dehuff -i infile -o outfile
The program supports a variety of file types, including text files, images, and PDFs. It processes each file bit by bit, ensuring precise compression and decompression. Users must specify the input file and output file paths to run the program.

**Final Design**


The development process began with implementing foundational components like the BitWriter and BitReader. These modules manage bit-level data operations, essential for both compression and decompression. The BitWriter writes individual bits and manages file buffers, while the BitReader processes bits from the input file and prepares them for decoding.

Next, I worked on the Node structure, which forms the backbone of the Huffman tree. Nodes were created to represent symbols and their frequencies, and the tree was constructed by combining nodes with the lowest frequencies. The tree enables efficient traversal, assigning binary codes to symbols and ensuring optimal compression.

The Priority Queue ensured that nodes were processed in the correct order during tree construction. This component prioritized nodes with lower frequencies, maintaining the integrity of the Huffman algorithm.

The main functions for Huffman Compression and Decompression utilized these components to perform the core tasks. Compression involved analyzing the input file, building the Huffman tree, and writing the encoded data to the output file. Decompression reversed this process, reconstructing the tree from the encoded file and restoring the original data.

To validate functionality, I tested each module individually using provided test files. Once all components passed their respective tests, I integrated them into the main program and conducted comprehensive tests with various file types. I also created custom shell scripts to simulate different scenarios, ensuring robust handling of edge cases and incorrect inputs.

**Reflection**


This project allowed me to deepen my understanding of data structures and algorithms. Implementing the Huffman Coding algorithm required working with binary trees, priority queues, and bit-level operations. It also emphasized the importance of modular programming and thorough testing. By integrating these components, I developed a reliable and efficient compression tool capable of handling diverse file types.
