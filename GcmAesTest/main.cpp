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
    
    /* ----------------------- Key Generation Test ----------------------- */
    bool result = _getNewAESKeyAndIv(&pszKey, &pszIv, keyLen, ivLen);
    
    if(result)
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
    if(result)
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
    if(result && strcmp(plainText.c_str(), outTestDecrypted) == 0)
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
    if(outTestEncrypted) delete outTestEncrypted;
    if(outTestDecrypted) delete outTestDecrypted;
    
    //clear buffers allocated by _getNewAESKeyAndIv function
    if(pszKey) delete pszKey;
    if(pszIv) delete pszIv;
    

    /* ----------------------- Encryption Decryption Test ----------------------- */
    
    
    
    
    
    
    
    /* ----------------------- C++ Encryption and C++ Decryption Test ----------------------- */
    std::string hexKey = "2192B39425BBD08B6E8E61C5D1F1BC9F428FC569FBC6F78C0BC48FCCDB0F42AE";
    std::string hexIV = "E1E592E87225847C11D948684F3B070D";

    printf("Plain Text: %s", plainText.c_str());
    
    char* outEncrypted = nullptr;
    int outEncryptedLen = 0;
    
    //encrypt - result base64 encoded
    result = _encrypt_GcmAes256(hexKey.c_str(), hexIV.c_str(), plainText.c_str(), &outEncrypted, outEncryptedLen);
    if(result)
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
    if(result && strcmp(plainText.c_str(), outDecrypted) == 0)
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
    if(outEncrypted) delete outEncrypted;
    if(outDecrypted) delete outDecrypted;
    
    /* ----------------------- C++ Encryption and C++ Decryption Test ----------------------- */
    
    
    
    
    
    
    
    /* ----------------------- Java based Encryption and C++ Decryption Test ----------------------- */
    //Java Encrypted with same Key and IV as above
    // A/boAixWJKflKviHp2cfDl6l/xn1qw2MsHcKFkrOfm2XOVmawIFct4fS1w7wKw==
    
    std::string javaEncrypted = "A/boAixWJKflKviHp2cfDl6l/xn1qw2MsHcKFkrOfm2XOVmawIFct4fS1w7wKw==";
    char* outCDecrypted;
    int outCDecryptedLen = 0;
    
    //decrypt - result plain text
    result = _decrypt_GcmAes256(hexKey.c_str(), hexIV.c_str(), javaEncrypted.c_str(), &outCDecrypted, outCDecryptedLen);
    if(result && strcmp(plainText.c_str(), outCDecrypted) == 0)
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
    if(outCDecrypted) delete outCDecrypted;
    
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
    return 0;
}

