//
// Created by yjrj on 2022/12/20.
//

#include "Random.h"

#if defined(_WIN32)
    #include <windows.h>
    #include <wincrypt.h>
#endif


namespace cc::exp {

bool Random::gen(void *data, uint32_t dataSize) {
#if defined(__linux__) || defined(__APPLE__)
    int res;
    auto *fp = fopen("/dev/urandom", "rb");
    if (fp == nullptr) {
        return false;
    }
    res = fread(data, 1, dataSize, fp);
    fclose(fp);
    if (res != dataSize) {
        return false;
    }
#else
    HCRYPTPROV handle = 0;
    if (!CryptAcquireContext(&handle, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT)) {
        handle = 0;
    }

    if (!CryptGenRandom(handle, static_cast<DWORD>(dataSize), static_cast<PBYTE>(data))) {
        return false;
    }
#endif
    return true;
}

}
