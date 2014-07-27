#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

#define HEX(N) (static_cast<std::stringstream&>(std::stringstream() << std::hex << std::setw(8) << std::setfill('0') << (N)).str())

#define ROTL(X, C) ((X << C) ^ (X >> (32 - C)))

class SHA1 {
  private:
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;
  public:
    SHA1(string message) {
      const uint64_t message_length = message.size(),
            block_length = 512 * ((message_length * 8) / 512) + 512;
      uint8_t blocks[block_length / 8] = {};
      blocks[message_length] = 1 << 7;
      for (uint64_t i = 0; i < message_length; i++) {
        blocks[i] = message[i];
      }
      for (uint64_t i = (block_length / 8) - 8; i < block_length / 8; i++) {
        blocks[i] = message_length * 8 >> 8 * (8 - (i - 55)) & ((1 << 8) - 1);
      }
      for (uint64_t current_block = 0; current_block < block_length / 512; current_block++) {
        uint32_t w[80] = {};
        for (int i = 0; i < 64; i++) {
          w[i/4] += blocks[64 * current_block + i] << 8 * (3 - i % 4);
        }
        for (int i = 16; i < 80; i++) {
          w[i] = ROTL((w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]), 1);
        }
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;
        for (int i = 0; i < 80; i++) {
          uint32_t f, k;
          if (i < 20) {
            f = (b & c) | ((~b) & d);
            k = 0x5A827999;
          } else if (i < 40) {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
          } else if (i < 60) {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDC;
          } else if (i < 80) {
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
          }
          uint32_t temp = ROTL(a, 5) + f + e + k + w[i];
          e = d;
          d = c;
          c = ROTL(b, 30);
          b = a;
          a = temp;
        }
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
      }
    }
    string to_string() {
      std::stringstream ss;
      ss << HEX(h0) <<
            HEX(h1) <<
            HEX(h2) <<
            HEX(h3) <<
            HEX(h4);
      return ss.str();
    }

};

std::ostream& operator<<(std::ostream& os, SHA1& hash) {
  return os << hash.to_string();
}

int main(int argc, char **argv) {
  SHA1 sha1(argv[1]);
  cout << sha1 << endl;
  return 0;
}
