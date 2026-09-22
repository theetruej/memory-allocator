# memory-allocator
C memory allocator project is a custom dynamic memory allocator written in C that manages a manually allocated heap rather than relying directly on the standard malloc() / free() implementation provided by C's standard library.

The allocator maintains metadata for individual memory blocks and provides functionality for allocating, reading, writing, reallocating, and deallocating memory. The project was designed to explore how dynamic memory management works at a lower level, including block metadata, pointer arithmetic, fragmentation, and heap organization.

<img width="684" height="575" alt="MemoryAllocator drawio" src="https://github.com/user-attachments/assets/aa79695b-42e8-488b-a261-01780b850807" />

The heap space used in the memory allocator, has a dedicated 1 GB of space, which is organized into words. Each word is exactly 4 bytes (32 bits) of data. These serve as basic units of memory addressed by the allocator. Memory within the heap is divided into blocks. Each block contains metadata about itself, stored in block headers. Headers are comprised of data about the number of words in the block, a 1 bit allocation flag that checks if it's allocated memory, and 1 reserved bit.

<img width="663" height="317" alt="Screenshot 2026-09-21 at 10 37 12 AM" src="https://github.com/user-attachments/assets/5849402a-2cfa-43b5-8251-7f051690a9f5" />


For initializing the heap space needed for the memory allocator, I wrote Assembly code to initialize a heap space called memspace. This heap space is 1 GB in size.

<img width="449" height="296" alt="Screenshot 2026-09-21 at 10 41 32 AM" src="https://github.com/user-attachments/assets/b7635224-cb10-4754-a41d-69e03c5d33bc" />

In order to allow the C code to call and reference the heap space, I had to write in the MakeFile for compiling C code to access heap.o (compiled asm code of the heap space) to use as a dependency. 

<img width="629" height="450" alt="Screenshot 2026-09-21 at 10 47 28 AM" src="https://github.com/user-attachments/assets/ddec211e-6ebb-4fa0-9b1e-769e682928af" />

The allocator's C header file establishes the foundation of the memory allocator. It defines the custom data types, memory representation, constants, compiler attributes, error codes, and declarations used throughout the project. These definitions provide a consistent interface and memory model for the allocator's implementation.

The file begins by enabling GNU-specific functionality with _GNU_SOURCE and includes the standard libraries required by the allocator. These libraries provide functionality used throughout the project, including memory operations, system calls, string manipulation, Boolean values, assertions, and error handling. I also implemented custom integer types, using fixed-width integer aliases to make the size of the data types within the allocator explicit. The heap type is defined as an 8-bit unsigned integer, meaning that each heap element represents one byte of memory. Words are also defined as 32-bit integer. Two compiler attributes are defined for use throughout the project, packed and unused. packed instructs the compiler to minimize or eliminate padding between structure members, allowing structures used by the allocator to have a predictable memory layout. The predictable memory layout allows easier manipulation and access of memory for my custom memory allocator. The unused marks variables or parameters that may intentionally remain unused, preventing compiler warnings. 

<img width="396" height="198" alt="Screenshot 2026-09-21 at 1 01 48 PM" src="https://github.com/user-attachments/assets/ee8d7c24-1e67-4fec-875b-5a081173ce33" />

Packed header is a struct which contains the number of words dedicated to that block of memory (30 bits), a reserved bit, and a boolean variable which tracks if its used or unused.

<img width="482" height="223" alt="Screenshot 2026-09-21 at 1 44 16 PM" src="https://github.com/user-attachments/assets/6e87e30c-a727-44ef-880c-8fe3574db0e8" />

I also used macro shortcuts for commonly used type casts and pointer types. These macros reduce repetive casting syntax through the allocator project, and help make code for low-level memory operations more concise.

<img width="461" height="206" alt="Screenshot 2026-09-21 at 1 50 48 PM" src="https://github.com/user-attachments/assets/62a65ef0-808a-49fa-b791-f550f10474d7" />

I finally defined functions needed for the allocator, an error function used to return error values, mkalloc() which makes an allocation, alloc() which creates an allocation, read_mem() which prints out data stored in memory, write_mem() which writes data into memory, dealloc() which de-allocates memory, and reallocate() which reallocates memory. 

<img width="265" height="85" alt="Screenshot 2026-09-21 at 2 01 16 PM" src="https://github.com/user-attachments/assets/c03cc00c-9550-45ca-bde8-cfc921c534da" />

In the alloc.c program, it grabs the header file (alloc.h) and standard definition header as dependencies, and requires a pointer of heap space (memspace).

<img width="1027" height="547" alt="image" src="https://github.com/user-attachments/assets/c44eb51a-0ba7-44a8-a514-f89a789dea09" />

mkalloc() takes word parameter and a pointer to a hdr. It calculates current usage, to check how many bytes have already been used up to the current header, by subtracting the start of the memory pool memspace from the current headers position. It calculates the new total size, and uses that value to check for an Out-Memory-Error. It then returns the hdr pointer with an offset of 4, because thats the number of bytes a word is.

<img width="451" height="461" alt="Screenshot 2026-09-21 at 3 15 10 PM" src="https://github.com/user-attachments/assets/71543c52-4ca1-4126-a497-e90df546ea75" />

The alloc() function is utilized to allocated a requested amount of memory from the allocator's managed heap. It determines where the requested memory can be placed, creates or updates the corresponding block metadata, and returns a pointer to a usable memory region.

<img width="622" height="1043" alt="Alloc drawio" src="https://github.com/user-attachments/assets/1c515d19-5037-4dae-a45c-d1e3eac09753" />

The function of alloc takes an int32 number of bytes, in the diagram the example is 16 bytes, converts it into the amount of words of memory in the heap space needed for the amount bytes. It then checks to see if the hdr is already allocated, if it isn't it uses mkalloc() to make the allocation of words and hands over the hdr struct.

<img width="798" height="211" alt="image" src="https://github.com/user-attachments/assets/021894d6-59b6-44c5-b3b1-b334c35e3014" />

dealloc() checks if the pointer is null, and sets the pointer parameter value (ptr) of the hdr to be the difference in the postion of ptr and the size of the header.


<img width="861" height="580" alt="image" src="https://github.com/user-attachments/assets/c486b39f-d7db-4a86-9326-6822aae81c9d" />

read_mem() reads data from a region of the allocators managed heap, which then outputs the requested number of bytes. write_mem() writes data to a specified region of the allocator's managed heap, allowing stored memory to be modified.

<img width="828" height="811" alt="image" src="https://github.com/user-attachments/assets/20a7e9cf-cf28-42c7-8fc7-67368c5b0270" />

reallocate() resizes an existing allocation when a larger amount of memory is requested. It allocates a new block, copies existing data into the new allocation, deallocates the original block of memory, and updates the pointer to reference the new allocation. However if the requested size is smaller than or equal to the current allocation, the existing block is left unchanged.

<img width="717" height="750" alt="image" src="https://github.com/user-attachments/assets/aeae219e-593c-45a7-88cb-ee864ae9a5e2" />

Finally int main() runs test code to check all of the memory allocating functions.

<img width="736" height="519" alt="image" src="https://github.com/user-attachments/assets/7de7cdec-d099-4d0c-91b6-bc851a374e03" />

