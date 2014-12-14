#include <iostream>
#include <iomanip>
#include <sstream>

#define ROTL(x, c) (((x) << (c)) bitor ((x) >> (32 - (c))))
#define CEIL(n) (static_cast<int64_t>(n) + (static_cast<int64_t>(n) != (n)))

class MD5 {
    static const uint32_t s[], K[];
    std::string message;
    uint32_t words[4];
  public:
    const std::string str();
    void update(const char *_message);
    MD5() = default;
    ~MD5() = default;
    MD5(const char *_message) {
      update(_message);
    }
};

const uint32_t MD5::s[] =
{ 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
  5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20,
  4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
  6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };

const uint32_t MD5::K[]=
{ 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
  0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
  0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
  0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
  0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
  0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
  0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
  0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
  0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
  0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
  0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
  0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
  0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
  0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
  0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
  0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

void MD5::update(const char *_message) {
  message += _message;
  const uint32_t block_count = CEIL(message.length() / 64.0);
  uint8_t bytes[block_count << 6];
  std::fill(bytes, bytes + (block_count << 6), 0);
  std::copy(message.begin(), message.end(), bytes);
  bytes[message.length()] = 128;
  reinterpret_cast<uint64_t*>(bytes)[block_count * 8 - 1] =
    static_cast<uint64_t>(message.length()) * 8;
  int a0 = 0x67452301, b0 = 0xEFCDAB89,
      c0 = 0x98BADCFE, d0 = 0x10325476;
  for (uint32_t block = 0, M[16]; block < block_count; block++) {
    for (uint32_t j = 0, i = block << 6; j < 64; j++, i++) {
      M[j >> 2] = (bytes[i] << 24) | (M[j >> 2] >> 8);
    }
    register int A = a0, B = b0, C = c0, D = d0;
    for (int j = 0, f, g, Dt; j < 64; j++) {
      if (j < 16) {
        f = (B & C) | (~B & D);
        g = j;
      } else if (j < 32) {
        f = (D & B) | (~D & C);
        g = (j * 5 + 1) & 0xf;
      } else if (j < 48) {
        f = B ^ C ^ D;
        g = (j * 3 + 5) & 0xf;
      } else {
        f = C ^ (B | ~D);
        g = (j * 7) & 0xf;
      }
      Dt = D;
      D = C;
      C = B;
      B += ROTL(A + f + K[j] + M[g], s[j]);
      A = Dt;
    }
    a0 += A; b0 += B;
    c0 += C; d0 += D;
  }
  words[0] = a0; words[1] = b0;
  words[2] = c0; words[3] = d0;
}

const std::string MD5::str() {
  auto sum_bytes = reinterpret_cast<uint8_t*>(words);
  std::ostringstream sum;
  for (int i = 0; i < 16; i++) {
    sum << std::setfill('0') << std::setw(2) << std::hex <<
      static_cast<uint32_t>(sum_bytes[i]);
  }
  return sum.str();
}

std::string read_stdin() {
  return static_cast<std::ostringstream&>
      ( std::ostringstream() << std::cin.rdbuf() ).str();
}

int main() {
  auto buffer = read_stdin();
  MD5 md5;
  md5.update(buffer.c_str());
  std::cout << md5.str() << std::endl;
}
