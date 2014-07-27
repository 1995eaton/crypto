#include <iostream>
#include <string>

#define _F32(N) ((N & 0xffff) + (N >> 16))

uint32_t Fletcher32(std::string message) {
  size_t mlen = message.size();
  uint32_t sum1 = 0xffff,
           sum2 = 0xffff;
  std::string::iterator it = message.begin();
  while (mlen) {
    unsigned tlen = mlen > 359 ? 359 : mlen;
    mlen -= tlen;
    do {
      sum2 += sum1 += (uint32_t) *it++;
    } while (--tlen);
    sum1 = _F32(sum1);
    sum2 = _F32(sum2);
  }
  sum1 = _F32(sum1);
  sum2 = _F32(sum2);
  return sum2 << 16 | sum1;
}

int main(int argc, char **argv) {
  std::cout << Fletcher32(argv[1]) << std::endl;
  return 0;
}
