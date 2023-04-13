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

#ifndef __CRYPTOGRAPHER_INCLUDE_HEADER__
#define __CRYPTOGRAPHER_INCLUDE_HEADER__

#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <syslog.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>



/**
Exported for dynamic loading and calling
 */
#ifdef __cplusplus
extern "C" {
#endif

    /*
     * Note:
     * All functions returning buffers like **out<name> are allocated using new and are null terminated.
     * Users must clear them using delete *out<name>
     */

    // ----------------------------------- libGcmAes.so Exports -----------------------------------
    bool _base64Encode(/*[in]*/ const char *inPlainText, /*[out]*/ char **outBase64Encoded, /*[in, out]*/ int &dataLength);
    bool _base64Decode(/*[in]*/ const char *inBase64Text, /*[out]*/ char **outPlainText, /*[in, out]*/ int &dataLength);
    bool _hexDecode(/*[in]*/ const char *inHexEncodedText, /*[out]*/ char **outHexDecoded);
    bool _hexEncode(/*[in]*/ const char *inData, /*[out]*/ char **outHexEncoded);

    bool _encrypt_GcmAes256(/*[in]*/const char *inHexKey, /*[in]*/const char *inHexIv, /*[in]*/const char *inPlainText, /*[out]*/ char **outEncryptedBase64, /*[in, out]*/int &dataLength);
    bool _decrypt_GcmAes256(/*[in]*/const char *inHexKey, /*[in]*/const char *inHexIv, /*[in]*/const char *inBase64Text, /*[out]*/ char **outDecrypted, /*[in, out]*/int &dataLength);

    bool _getNewAESKeyAndIv(/*[out]*/ char **outHexKey, /*[out]*/ char **outHexIv, /*[in, out]*/int &outKeyLength, /*[in, out]*/ int &outIvLength);
    // ----------------------------------- libGcmAes.so Exports -----------------------------------

#ifdef __cplusplus
}
#endif

#endif