BinIO
====
BinIO is a simplistic library for handling basic binary data types. 
Currently, it is designed to handle data written by itself, but interaction with data written by other software is possible too. 
As of now (2013.08.19) BinIO works with both Little-Endian and Big-Endian data. Before using binio::buffer, call binio::init(). It will determine the system's native endianness. 

You can use it by creating a buffer, filling it with some data ( eg. read a few bytes from a file, copy it from a memory lump, whatever ) seek to the buffer's start and start reading with its builtin functions.
Or, you can write to it with the builtin functions and flush it to a file. 

Currently handled data types:
	* char, unsigned char
	* short, unsigned short
	* int, unsigned int
	* float, double
	* null-terminated strings 

Also, I'm still mediocre at best with C++, so constructive critique is very well appreciated :)

License
====
BinIO is under the MIT license. See the file called 'license'.
Also, a link to the project's github page would be greatly appreciated :) 