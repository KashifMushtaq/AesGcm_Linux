/*
Copyright (©) 2023 Kashif Mushtaq

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sub-license, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

/* 
 * File:   main.cpp
 * Author: kmushtaq
 *
 * Created on April 11, 2023, 9:21 AM
 */

//Include GcmAES header and link this project against libGcmAes.so
#include "GcmAes.h"

#include <cstdlib>
#include <regex>
#include <cstring>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{

    // lets first generate encryption Key and IV for AES 256 GCM. Key size is 32 bytes and IV size is 16 bytes
    // Lib generated Key and IV are hex encoded. 
    // Encryption and decryption function expects hex encoded Key and IV.

    std::string inHexKey;
    std::string inHexIV;
    char* pszKey = nullptr;
    char* pszIv = nullptr;
    int keyLen = 0;
    int ivLen = 0;

    std::string plainText = "Test encryption and decryption";
    bool result = false;

    /* ----------------------- Key Generation Test ----------------------- */
    result = _getNewAESKeyAndIv(&pszKey, &pszIv, keyLen, ivLen);

    if (result)
    {
        printf("Key=%s\nLen=%i\n\nIV=%s\nLen=%i\n", pszKey, keyLen, pszIv, ivLen);
        printf("Key and IV Test OK\n\n");
    }
    else
    {
        printf("%s", "Key and IV generation failed. Please check syslog for errors");
        return 1;
    }
    /* ----------------------- Key Generation Test ----------------------- */


    /* ----------------------- Encryption Decryption Test ----------------------- */
    //Save key and IV to call encryption and decryption functions
    inHexKey = pszKey;
    inHexIV = pszIv;

    char* outTestEncrypted = nullptr;
    int outTestEncryptedLen = 0;

    //encrypt - result base64 encoded
    result = _encrypt_GcmAes256(inHexKey.c_str(), inHexIV.c_str(), plainText.c_str(), &outTestEncrypted, outTestEncryptedLen);
    if (result)
    {
        printf("Test 1 -> Encrypted base64 encoded: %s\n", outTestEncrypted);
    }
    else
    {
        printf("%s", "Test 1 -> Encryption failed. Please check syslog for errors");
        return 1;
    }

    char* outTestDecrypted = nullptr;
    int outTestDecryptedLen = 0;

    //decrypt - result plain text
    result = _decrypt_GcmAes256(inHexKey.c_str(), inHexIV.c_str(), outTestEncrypted, &outTestDecrypted, outTestDecryptedLen);
    if (result && strcmp(plainText.c_str(), outTestDecrypted) == 0)
    {
        printf("Test 1 -> Decrypted: %s\n", outTestDecrypted);
        printf("Test 1 -> Encryption / Decryption OK\n\n");
    }
    else
    {
        printf("%s", "Test 1 -> Decryption failed. Please check syslog for errors");
        return 1;
    }

    inHexKey.clear();
    inHexIV.clear();

    //clear buffers
    if (outTestEncrypted) delete[] outTestEncrypted;
    outTestEncrypted = nullptr;

    if (outTestDecrypted) delete[] outTestDecrypted;
    outTestDecrypted = nullptr;


    //clear buffers allocated by _getNewAESKeyAndIv function
    if (pszKey) delete[] pszKey;
    pszKey = nullptr;

    if (pszIv) delete[] pszIv;
    pszIv = nullptr;


    /* ----------------------- Encryption Decryption Test ----------------------- */







    /* ----------------------- C++ Encryption and C++ Decryption Test ----------------------- */
    std::string hexKey = "2192B39425BBD08B6E8E61C5D1F1BC9F428FC569FBC6F78C0BC48FCCDB0F42AE";
    std::string hexIV = "E1E592E87225847C11D948684F3B070D";

    printf("Plain Text: %s\n", plainText.c_str());

    char* outEncrypted = nullptr;
    int outEncryptedLen = 0;

    //encrypt - result base64 encoded
    result = _encrypt_GcmAes256(hexKey.c_str(), hexIV.c_str(), plainText.c_str(), &outEncrypted, outEncryptedLen);
    if (result)
    {
        printf("Test 2 -> Encrypted base64 encoded: %s\n", outEncrypted);
    }
    else
    {
        printf("%s", "Test 2 -> Encryption failed. Please check syslog for errors");
        return 1;
    }

    char* outDecrypted = nullptr;
    int outDecryptedLen = 0;

    //decrypt - result plain text
    result = _decrypt_GcmAes256(hexKey.c_str(), hexIV.c_str(), outEncrypted, &outDecrypted, outDecryptedLen);
    if (result && strcmp(plainText.c_str(), outDecrypted) == 0)
    {
        printf("Test 2 -> Decrypted: %s\n", outDecrypted);
        printf("Test 2 -> Encryption / Decryption OK\n\n");
    }
    else
    {
        printf("%s", "Test 2 -> Decryption failed. Please check syslog for errors");
        return 1;
    }

    //clear buffers
    if (outEncrypted) delete[] outEncrypted;
    outEncrypted = nullptr;

    if (outDecrypted) delete[] outDecrypted;
    outDecrypted = nullptr;

    /* ----------------------- C++ Encryption and C++ Decryption Test ----------------------- */







    /* ----------------------- Java based Encryption and C++ Decryption Test ----------------------- */
    //Java Encrypted with same Key and IV as above
    // A/boAixWJKflKviHp2cfDl6l/xn1qw2MsHcKFkrOfm2XOVmawIFct4fS1w7wKw==

    std::string javaEncrypted = "A/boAixWJKflKviHp2cfDl6l/xn1qw2MsHcKFkrOfm2XOVmawIFct4fS1w7wKw==";
    char* outCDecrypted=nullptr;
    int outCDecryptedLen = 0;

    //decrypt - result plain text
    result = _decrypt_GcmAes256(hexKey.c_str(), hexIV.c_str(), javaEncrypted.c_str(), &outCDecrypted, outCDecryptedLen);
    if (result && strcmp(plainText.c_str(), outCDecrypted) == 0)
    {
        printf("Test 3 -> Decrypted: %s\n", outCDecrypted);
        printf("Test 3 -> Java Encrypted / C++ Decryption OK\n\n");
    }
    else
    {
        printf("%s", "Test 3 -> Java Decryption failed. Please check syslog for errors");
        return 1;
    }

    //clear buffers
    if (outCDecrypted) delete[] outCDecrypted;
    outCDecrypted = nullptr;

    /* ----------------------- Java based Encryption and C++ Decryption Test ----------------------- */



    /*
    Result:
    
    Key=BF2F0CEC3D78EEB8388D8A78402510B9F3A99F7F69E98C6DB7168D0C57270EF4
    Len=64

    IV=FE6A26A45DB769F02C55CF12252F8A32
    Len=32
    Key and IV Test OK

    Test 1 -> Encrypted base64 encoded: 5NKdId7ZkBMWTtM5OhtPiaq1x0lBhbUSCxpcWK8PjjIGUvVdMdYET0Gw1mwmOw==
    Test 1 -> Decrypted: Test encryption and decryption
    Test 1 -> Encryption / Decryption OK

    Plain Text: Test encryption and decryptionTest 2 -> Encrypted base64 encoded: A/boAixWJKflKviHp2cfDl6l/xn1qw2MsHcKFkrOfm2XOVmawIFct4fS1w7wKw==
    Test 2 -> Decrypted: Test encryption and decryption
    Test 2 -> Encryption / Decryption OK

    Test 3 -> Decrypted: Test encryption and decryption
    Test 3 -> Java Encrypted / C++ Decryption OK


    RUN FINISHED; exit value 0; real time: 20ms; user: 0ms; system: 0ms

     */
    
    /* ----------------------- Hex Encoding / Decoding Test ----------------------- */
    
    std::string pszPlainText = "syllabic kana – hiragana (平仮名) and katakana (片仮名)";
    char* hexEncoded = nullptr;
    char* hexDecoded = nullptr;
    
    result = _hexEncode(pszPlainText.c_str(), &hexEncoded);
    
    if(result)
    {
        printf("Test 4 -> Multi-byte Text: %s\n", pszPlainText.c_str());
        printf("Test 4 -> Hex Encoded: %s\n", hexEncoded);
        printf("Test 4 -> Hex Encoding OK\n\n");
    }
    else
    {
        printf("%s", "Test 4 -> Encoding failed.");
        return 1;
    }
    
    result = _hexDecode(hexEncoded, &hexDecoded);
    
    if(result && strcmp(pszPlainText.c_str(), hexDecoded) == 0)
    {
        printf("Test 5 -> Multi-byte Text: %s\n", pszPlainText.c_str());
        printf("Test 5 -> Hex Decoded: %s\n", hexDecoded);
        printf("Test 5 -> Hex Decoding OK\n\n");
    }
    else
    {
        printf("%s", "Test 4 -> Decoding failed.");
        return 1;
    }
    
    if(hexEncoded) delete[] hexEncoded;
    if(hexDecoded) delete[] hexDecoded;
    hexEncoded = nullptr;
    hexDecoded = nullptr;

    /* ----------------------- Hex Encoding / Decoding Test ----------------------- */
    
    
    /* ----------------------- Base64 Encoding / Decoding Test ----------------------- */
    
    char* base64Encoded = nullptr;
    char* base64Decoded = nullptr;
    int base64Len = pszPlainText.length();
    
    result = _base64Encode(pszPlainText.c_str(), &base64Encoded, base64Len);
    
    if(result)
    {
        printf("Test 6 -> Multi-byte Text: %s\n", pszPlainText.c_str());
        printf("Test 6 -> Base64 Encoded: %s\n", base64Encoded);
        printf("Test 6 -> Base64 Encoding OK\n\n");
    }
    else
    {
        printf("%s", "Test 6 -> Encoding failed.");
        return 1;
    }
    
    base64Len = strlen(base64Encoded);
    result = _base64Decode(base64Encoded, &base64Decoded, base64Len);
    
    if(result && strcmp(pszPlainText.c_str(), base64Decoded) == 0)
    {
        printf("Test 7 -> Multi-byte Text: %s\n", pszPlainText.c_str());
        printf("Test 7 -> Base64 Decoded: %s\n", base64Decoded);
        printf("Test 7 -> Base64 Decoding OK\n\n");
    }
    else
    {
        printf("%s", "Test 7 -> Decoding failed.");
        return 1;
    }
    
    if(base64Encoded) delete[] base64Encoded;
    if(base64Decoded) delete[] base64Decoded;
    base64Encoded = nullptr;
    base64Decoded = nullptr;
    
    /* ----------------------- Base64 Encoding / Decoding Test ----------------------- */
    
    
    
    hexKey.clear();
    hexIV.clear();
    pszPlainText.clear();
    
    return 0;
}

