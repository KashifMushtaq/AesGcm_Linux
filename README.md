# Cross Platform AES 256 GCM Encryption / Decryption (C++ dynamic library)

![C# Test](/GcmAes.png)

### Introduction
While working in security, identity management and data protection fields for a while, I found a very few working examples in the public domain on cross platform encryption based on AES 256 GCM algorithm. This is the same algorithm used by Google when you access Gmail, etc.

This article may help you implement very strong cross platform encryption / decryption. The sample code is in C++, C# and Java. However, Java through JNI (Java Native Interface) and C# through COM, can call native C++ code, which in my testing appears to be much faster as compared to pure Java or C# implementations. Still, there are times when one wants to do it without calling a native C++ layer.


### Using the Code
This repository consists 2 **Apache Netbeans projects**. Please clone the repository and open projects in Netbeans IDE.

### Build Steps:

### Crypto++:
To build, please open **cryptopp870** folder in **terminal** and enter **make** command. It will build **libcryptopp.a** static library. The ligrary defaults to OS architecture. GcmAes Netbeans project depends on libcryptopp.a and is linked to it.

Once the library is available, you can build both projects individually from Netbeans IDE. **GcmAesTest** project has **libGcmAes.so** depedency and will be pre-build. Once build, the projects could be debugged as well.

The test program validates few encryption and decryption tests. One of them is the decryption of text which was encrypted using Java code (links available below).

It produce following sample test restlts:

```
Key=AA85FDCFDB92658D1DA0F8D9271435AA10416A118557F5772C8CD9F63AC6911F
Len=64

IV=D60C51CD45D77997787B5B601B075F75
Len=32
Key and IV Test OK

Test 1 -> Encrypted base64 encoded: 8iHGeXHV4giaO3TqTizUOVEmC8Ik04aOlPHIt13mhZLS84Vie2grBGg4/Yoezw==
Test 1 -> Decrypted: Test encryption and decryption
Test 1 -> Encryption / Decryption OK

Plain Text: Test encryption and decryptionTest 2 -> Encrypted base64 encoded: A/boAixWJKflKviHp2cfDl6l/xn1qw2MsHcKFkrOfm2XOVmawIFct4fS1w7wKw==
Test 2 -> Decrypted: Test encryption and decryption
Test 2 -> Encryption / Decryption OK

Test 3 -> Decrypted: Test encryption and decryption
Test 3 -> Java Encrypted / C++ Decryption OK
```

The test project also demonstrates how to use the **libGcmAes.so** with other C++ projects on Linux like systems. Normally, just have to include **GcmAes.h** in your project and must link your project against **libGcmAes.so**. Code uses **C++ 14** standard.

### Background

[Cross Platform AES 256 GCM Encryption and Decryption (C++, C# and Java)](https://www.codeproject.com/Articles/1265115/Cross-Platform-AES-256-GCM-Encryption-Decryption)

You can also read more about Crypto++ AES GCM implementation or algorithm itself here and [here](https://www.cryptopp.com/).


### Related Projects:

### [C# Verion](https://github.com/KashifMushtaq/AesGcm256)
### [C++ Verion](https://github.com/KashifMushtaq/AES_GCM_256_C)
### [Java Verion](https://github.com/KashifMushtaq/Aes256GCM_Java)
