#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#define ROT(V, S) ((V >> S) | (V << (sizeof(V) * 8 - S)))
#define HEX(N) (static_cast<std::stringstream&>(std::stringstream() << std::hex << std::setw(8) << std::setfill('0') << (N)).str())

class SHA256 {
  private:
    uint32_t words[8] = {
      0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
      0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };
    const uint32_t k[64] = {
      0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
      0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
      0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
      0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
      0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
      0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
      0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
      0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };
  public:
    SHA256(std::string message);
    std::string to_string();
    uint32_t *get_words();
};

SHA256::SHA256(std::string message) {
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
    uint32_t w[64] = {};
    for (int i = 0; i < 64; i++) {
      w[i/4] += blocks[64 * current_block + i] << 8 * (3 - i % 4);
    }
    for (int i = 16; i < 64; i++) {
      const uint32_t r = w[i-15],
                     g = w[i-2];
      w[i] = w[i-16] + (ROT(r, 7) ^ ROT(r, 18) ^ r >> 3) + w[i-7] + (ROT(g, 17) ^ ROT(g, 19) ^ g >> 10);
    }
    uint32_t a = words[0], b = words[1], c = words[2], d = words[3],
             e = words[4], f = words[5], g = words[6], h = words[7];
    for (int i = 0; i < 64; i++) {
      uint32_t r = h + (ROT(e, 6) ^ ROT(e, 11) ^ ROT(e, 25)) + ((e & f) ^ (~e & g)) + k[i] + w[i],
               m = (a & b) ^ (a & c) ^ (b & c);
      h = g;
      g = f;
      f = e;
      e = d + r;
      d = c;
      c = b;
      b = a;
      a = r + (ROT(a, 2) ^ ROT(a, 13) ^ ROT(a, 22)) + m;
    }
    words[0] += a; words[1] += b; words[2] += c; words[3] += d;
    words[4] += e; words[5] += f; words[6] += g; words[7] += h;
  }
}

std::string SHA256::to_string() {
  std::stringstream ss;
  for (auto& it: words) {
    ss << HEX(it);
  }
  return ss.str();
}

uint32_t *SHA256::get_words() { return words; }

std::ostream& operator<<(std::ostream& os, SHA256& hash) {
  return os << hash.to_string();
}

std::string read_stdin() {
  return static_cast<std::ostringstream&>
      ( std::ostringstream() << std::cin.rdbuf() ).str();
}

int main(int argc, char **argv) {
  SHA256 hash(read_stdin());
  std::cout << hash << std::endl;
}
