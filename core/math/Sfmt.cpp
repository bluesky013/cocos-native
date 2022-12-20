
#include "Sfmt.h"
#include "Random.h"

namespace cc::exp {

SFMTRandom::SFMTRandom()
{
    uint32_t seed[32] = {0};
    Random::gen(&seed, sizeof(seed));
    sfmt_init_by_array(&sfmt, seed, 32);
}

SFMTRandom::SFMTRandom(uint32_t s) : seed(s)
{
    sfmt_init_gen_rand(&sfmt, seed);
}

uint32_t SFMTRandom::genU32()
{
    std::lock_guard<std::mutex> lock(mutex);
    return sfmt_genrand_uint32(&sfmt);
}

uint64_t SFMTRandom::genU64()
{
    std::lock_guard<std::mutex> lock(mutex);
    return sfmt_genrand_uint64(&sfmt);
}

}
