#include "Cryptographer.h"

bool
_base64Encode(/*[in]*/const char *inPlainText, /*[out]*/ char **outBase64Encoded, /*[in, out]*/int &dataLength)
{
    bool bR = false;
    std::string pszOut;

    CryptoPP::Base64Encoder *base64Encoder = new CryptoPP::Base64Encoder(new StringSink(pszOut), false);
    base64Encoder->PutMessageEnd(reinterpret_cast<const unsigned char *> (inPlainText), strlen(inPlainText));
    delete base64Encoder;
    dataLength = pszOut.length();
    if (dataLength > 0)
    {
        if (*outBase64Encoded) free(*outBase64Encoded);
        *outBase64Encoded = (char*) malloc(dataLength + 1);
        memset(*outBase64Encoded, '\0', dataLength + 1);
        memcpy(*outBase64Encoded, pszOut.c_str(), dataLength);
        bR = true;
    }
    pszOut.clear();
    return bR;
}

bool
_base64Decode(/*[in]*/const char *inBase64Text, /*[out]*/ char **outPlainText, /*[in, out]*/int &dataLength)
{
    bool bR = false;
    std::string pszOut;

    Base64Decode(inBase64Text, pszOut);

    if (pszOut.length() > 0)
    {
        if (*outPlainText) free(*outPlainText);
        dataLength = pszOut.length();
        *outPlainText = (char*) malloc(dataLength + 1);
        memset(*outPlainText, '\0', dataLength + 1);
        memcpy(*outPlainText, pszOut.c_str(), dataLength);
        bR = true;
    }
    pszOut.clear();
    return bR;
}

void
Base64Decode(const std::string &inString, std::string &outString)
{
    StringSource(inString, true, new Base64Decoder(new StringSink(outString)));
}

bool
_encrypt_GcmAes256(/*[in]*/const char *inHexKey, /*[in]*/const char *inHexIv, /*[in]*/const char *inPlainText, /*[out]*/ char **outEncryptedBase64, /*[in, out]*/int &dataLength)
{
    bool bR = false;

    std::string aesKey(inHexKey);
    std::string aesIv(inHexIv);

    std::string outText;
    std::string outBase64;

    if (aesKey.length() > 0 && aesIv.length() > 0)
    {
        bR = _encrypt_local(aesKey.c_str(), aesIv.c_str(), inPlainText, outEncryptedBase64, dataLength);
    }
    else
    {
        m_ErrorMessage.append("_encrypt_GcmAes256 -> AES Session Key or IV cannot be empty");
    }

    outText.clear();
    outBase64.clear();

    return bR;
}

bool
_decrypt_GcmAes256(/*[in]*/const char *inHexKey, /*[in]*/const char *inHexIv, /*[in]*/const char *inBase64Text, /*[out]*/ char **outDecrypted, /*[in, out]*/int &dataLength)
{
    bool bR = false;

    std::string aesKey(inHexKey);
    std::string aesIv(inHexIv);

    if (aesKey.length() > 0 && aesIv.length() > 0)
    {
        bR = _decrypt_local(aesKey.c_str(), aesIv.c_str(), inBase64Text, outDecrypted, dataLength);
    }
    else
    {
        m_ErrorMessage.append("_decrypt_GcmAes256 -> AES Session Key or IV cannot be empty");
    }

    return bR;
}

bool
_getNewAESKeyAndIv(/*[out]*/ char **outHexKey, /*[out]*/ char **outHexIv, /*[out]*/int &outKeyLength, /*[out]*/int &outIvLength)
{
    bool bR = false;

    try
    {
        std::string outAESKey;
        std::string outAESIV;

        byte *bKey = new byte[AES::MAX_KEYLENGTH];
        memset(bKey, 0, AES::MAX_KEYLENGTH);
        PSRNG().GenerateBlock(bKey, AES::MAX_KEYLENGTH);

        byte *bIV = new byte[AES::BLOCKSIZE];
        memset(bIV, 0, AES::BLOCKSIZE);
        PSRNG().GenerateBlock(bIV, AES::BLOCKSIZE);



        HexEncoder *hexEncoder = new HexEncoder(new StringSink(outAESKey));
        hexEncoder->Put(bKey, AES::MAX_KEYLENGTH);
        hexEncoder->MessageEnd();
        delete hexEncoder;

        hexEncoder = new HexEncoder(new StringSink(outAESIV));
        hexEncoder->Put(bIV, AES::BLOCKSIZE);
        hexEncoder->MessageEnd();

        delete bKey;
        delete bIV;
        delete hexEncoder;

        hexEncoder = NULL;

        outKeyLength = outAESKey.length();
        outIvLength = outAESIV.length();

        if (outKeyLength > 0 && outIvLength > 0)
        {
            if (*outHexKey) free(*outHexKey);
            if (*outHexIv) free(*outHexIv);

            *outHexKey = (char*) malloc(outKeyLength + 1);
            *outHexIv = (char*) malloc(outIvLength + 1);

            memset(*outHexKey, '\0', outKeyLength + 1);
            memset(*outHexIv, '\0', outIvLength + 1);

            memcpy(*outHexKey, outAESKey.c_str(), outKeyLength);
            memcpy(*outHexIv, outAESIV.c_str(), outIvLength);

            bR = true;
        }
        else
        {
            m_ErrorMessage.append("_getNewAESKey -> Failed");
        }

        outAESKey.clear();
        outAESIV.clear();
    }
    catch (CryptoPP::Exception *e)
    {
        m_ErrorMessage.append(e->GetWhat());
    }

    if (m_ErrorMessage.length() > 0)
    {
        logtosys(0, m_ErrorMessage.c_str());
        m_ErrorMessage.clear();
    }

    return bR;
}

bool
_encrypt_local(/*[in]*/const char *aesKey, /*[in]*/const char *aesIV, /*[in]*/const char *inPlainText, /*[out]*/ char **outEncryptedBase64, /*[in, out]*/int &dataLength)
{
    bool bR = false;
    std::string outText;
    std::string outBase64;

    if (strlen(aesKey) > 64 && strlen(aesIV) > 32)
    {
        try
        {
            GCM< AES >::Encryption aesEncryption;
            aesEncryption.SetKeyWithIV(reinterpret_cast<const byte*> (aesKey), AES::MAX_KEYLENGTH, reinterpret_cast<const byte*> (aesIV), AES::BLOCKSIZE);

            StringSource(inPlainText, true, new AuthenticatedEncryptionFilter(aesEncryption, new StringSink(outText)
                                                                              ) // AuthenticatedEncryptionFilter
                         ); // StringSource

            CryptoPP::Base64Encoder *base64Encoder = new CryptoPP::Base64Encoder(new StringSink(outBase64), false);
            base64Encoder->PutMessageEnd(reinterpret_cast<const unsigned char *> (outText.data()), outText.length());
            delete base64Encoder;

            dataLength = outBase64.length();
            if (outBase64.length() > 0)
            {
                if (*outEncryptedBase64) free(*outEncryptedBase64);
                *outEncryptedBase64 = (char*) malloc(dataLength + 1);
                memset(*outEncryptedBase64, '\0', dataLength + 1);
                memcpy(*outEncryptedBase64, outBase64.c_str(), dataLength);

                bR = true;
            }
            else
            {
                m_ErrorMessage.append("_encrypt_local -> Encryption Failed");
            }

        }
        catch (CryptoPP::InvalidArgument& e)
        {
            m_ErrorMessage.append(e.what());
        }
        catch (CryptoPP::Exception& e)
        {
            m_ErrorMessage.append(e.what());
        }
    }
    else
    {
        m_ErrorMessage.append("_encrypt_local -> AES Key and IV must be 64 and 32 hex characters");
    }

    outText.clear();
    outBase64.clear();

    return bR;
}

bool
_decrypt_local(/*[in]*/const char *aesKey, /*[in]*/const char *aesIV, /*[in]*/const char *inBase64Text, /*[out]*/ char **outDecrypted, /*[in, out]*/int &dataLength)
{
    bool bR = false;
    std::string outText;

    std::string pszDecodedText;
    Base64Decode(inBase64Text, pszDecodedText);

    if (strlen(aesKey) == 64 && strlen(aesIV) == 32)
    {
        try
        {
            GCM< AES >::Decryption aesDecryption;
            aesDecryption.SetKeyWithIV(reinterpret_cast<const byte*> (aesKey), AES::MAX_KEYLENGTH, reinterpret_cast<const byte*> (aesIV), AES::BLOCKSIZE);

            AuthenticatedDecryptionFilter df(aesDecryption, new StringSink(outText));

            // The StringSource dtor will be called immediately
            //  after construction below. This will cause the
            //  destruction of objects it owns. To stop the
            //  behavior so we can get the decoding result from
            //  the DecryptionFilter, we must use a redirector
            //  or manually Put(...) into the filter without
            //  using a StringSource.
            StringSource(pszDecodedText, true,
                         new Redirector(df /*, PASS_EVERYTHING */)
                         ); // StringSource

            // If the object does not throw, here's the only
            //  opportunity to check the data's integrity
            bR = df.GetLastResult();


            dataLength = outText.length();
            if (outText.length() > 0)
            {
                if (*outDecrypted) free(*outDecrypted);
                *outDecrypted = (char*) malloc(dataLength + 1);
                memset(*outDecrypted, '\0', dataLength + 1);
                memcpy(*outDecrypted, outText.c_str(), dataLength);

                bR = true;
            }
            else
            {
                m_ErrorMessage.append("_decrypt_local -> Decryption Failed");
            }
        }
        catch (CryptoPP::HashVerificationFilter::HashVerificationFailed& e)
        {
            m_ErrorMessage.append(e.what());
        }
        catch (CryptoPP::InvalidArgument& e)
        {
            m_ErrorMessage.append(e.what());
        }
        catch (CryptoPP::Exception& e)
        {
            m_ErrorMessage.append(e.what());
        }
    }
    else
    {
        m_ErrorMessage.append("_decrypt_local -> AES Key and IV must be 64 and 32 hex characters");
    }

    return bR;
}

void
hexDecode(std::string &hexString)
{
    if (!hexString.c_str()) return;
    if (hexString.length() == 0) return;

    std::string binValue;
    HexDecoder decoder;
    decoder.Attach(new StringSink(binValue));
    decoder.Put((byte*) hexString.data(), hexString.size());
    decoder.MessageEnd();

    hexString.clear();
    hexString.append(binValue);
    binValue.clear();

}

/* some syslogging */
static void
logtosys(int err, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    openlog("GcmAes", LOG_CONS | LOG_PID, LOG_SYSLOG);
    vsyslog(err, format, args);
    va_end(args);
    closelog();
}

