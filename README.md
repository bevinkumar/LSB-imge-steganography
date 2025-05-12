Overview
This project demonstrates the implementation of a Steganography technique known as Least Significant Bit (LSB). LSB is a method of hiding secret data (like text or files) within an image by manipulating the least significant bits of the image's pixel values. The idea is to encode the secret message into the image without significantly altering its appearance. This project is implemented in C, and the primary objective is to hide and extract secret data from a BMP image using the LSB technique.

Features
Encoding: Hide a message or data into a BMP image.

Decoding: Extract the hidden message from the BMP image.

Simple C implementation: No external libraries required, only basic file handling and bit manipulation.

Support for text data encoding: Supports encoding text data into images.

Concept
Least Significant Bit (LSB) Steganography
LSB is the process of manipulating the least significant bits of the pixel values in an image to embed secret information.

How it works:

Every pixel in an image is represented by a combination of Red, Green, and Blue (RGB) values.

Each RGB component is typically stored in an 8-bit format (values from 0 to 255).

By altering the least significant bit of each component (i.e., the rightmost bit), we can embed data without significantly changing the image's appearance.

For example, if the pixel value is 10110010, changing the least significant bit (the last 0) will result in 10110011, which is a minor change that is usually imperceptible to the human eye.

Process Overview
Encoding Process:

A message (text or binary data) is first converted to binary.

The LSB of each pixel's RGB component is replaced with the corresponding bits of the message.

The modified image is then saved with the secret data embedded in it.

Decoding Process:

The hidden message is extracted by reading the least significant bits of each pixel's RGB components.

The bits are combined to reconstruct the original message.
