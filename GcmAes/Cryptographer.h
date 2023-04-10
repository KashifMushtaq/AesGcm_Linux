#pragma once

// Crypto++ Include

#include "./cryptopp870/pch.h"
#include "./cryptopp870/files.h"
#include "./cryptopp870/default.h"
#include "./cryptopp870/base64.h"
#include "./cryptopp870/osrng.h"

//AES
#include "./cryptopp870/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "./cryptopp870/cryptlib.h"
using CryptoPP::BufferedTransformation;
using CryptoPP::AuthenticatedSymmetricCipher;

#include "./cryptopp870/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::AuthenticatedEncryptionFilter;
using CryptoPP::AuthenticatedDecryptionFilter;

#include "./cryptopp870/aes.h"
using CryptoPP::AES;

#include "./cryptopp870/gcm.h"
using CryptoPP::GCM;
using CryptoPP::GCM_TablesOption;


#include <iostream>
#include <time.h>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <syslog.h>
#include <stdarg.h>
#include<string>
#include<string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <pwd.h>

USING_NAMESPACE(CryptoPP)
USING_NAMESPACE(std)



static std::string m_ErrorMessage;


static inline RandomNumberGenerator& PSRNG(void);

static inline RandomNumberGenerator& PSRNG(void)
{
    static AutoSeededRandomPool rng;
    rng.Reseed();
    return rng;
}

static void logtosys(int err, const char *format, ...);


/**
Exported for dynamic loading and calling
 */
#ifdef __cplusplus
extern "C"
{
#endif
    
    bool _base64Encode(/*[in]*/const char *inPlainText, /*[out]*/ char **outBase64Encoded, /*[in, out]*/int &dataLength);
    bool _base64Decode(/*[in]*/const char *inBase64Text, /*[out]*/ char **outPlainText, /*[in, out]*/int &dataLength);

    bool _encrypt_GcmAes256(/*[in]*/const char *inHexKey, /*[in]*/const char *inHexIv, /*[in]*/const char *inPlainText, /*[out]*/ char **outEncryptedBase64, /*[in, out]*/int &dataLength);
    bool _decrypt_GcmAes256(/*[in]*/const char *inHexKey, /*[in]*/const char *inHexIv, /*[in]*/const char *inBase64Text, /*[out]*/ char **outDecrypted, /*[in, out]*/int &dataLength);

    bool _getNewAESKeyAndIv(/*[out]*/ char **outHexKey, /*[out]*/ char **outHexIv, /*[out]*/int &outKeyLength, /*[out]*/int &outIvLength);

#ifdef __cplusplus
}
#endif

void Base64Decode(const std::string& inString, std::string& outString);

void hexDecode(std::string &hexString);
bool _encrypt_local(/*[in]*/const char *aesKey, /*[in]*/const char *aesIV, /*[in]*/const char *inPlainText, /*[out]*/ char **outEncryptedBase64, /*[in, out]*/int &dataLength);
bool _decrypt_local(/*[in]*/const char *aesKey, /*[in]*/const char *aesIV, /*[in]*/const char *inBase64Text, /*[out]*/ char **outDecrypted, /*[in, out]*/int &dataLength);
