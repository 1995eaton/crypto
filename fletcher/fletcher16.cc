#include <iostream>
#include <string>

#define _F16(N) ((N & 0xff) + (N >> 8))

uint32_t Fletcher16(std::string message) {
  size_t mlen = message.size();
  uint16_t sum1 = 0xff,
           sum2 = 0xff;
  std::string::iterator it = message.begin();
  while (mlen) {
    size_t tlen = mlen > 20 ? 20 : mlen;
    mlen -= tlen;
    do {
      sum2 += sum1 += (uint16_t) *it++;
    } while (--tlen);
    sum1 = _F16(sum1);
    sum2 = _F16(sum2);
  }
  sum1 = _F16(sum1);
  sum2 = _F16(sum2);
  return sum2 << 8 | sum1;
}

int main(int argc, char **argv) {
  std::cout << Fletcher16(argv[1]) << std::endl;
  return 0;
}
