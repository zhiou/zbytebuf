//
//  zbytebuf.hpp
//  transmit
//
//  Created by zhiou on 2019/7/1.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

namespace zzstudio {

typedef unsigned char byte;
typedef std::vector<byte> byte_v;

class zbytebuf {
public:
  zbytebuf() : m_mem(new byte_v()) {}

  zbytebuf(size_t size,
             bool random = false) // 初始化指定大小，默认全0，否则填充随机数
  {
    m_mem = std::unique_ptr<byte_v>(new byte_v(size));
    if (!random)
      return;

    std::default_random_engine source;
    std::uniform_int_distribution<byte> dist(0, 0xFF);
    auto r = std::bind(dist, source);
    std::generate(std::begin(*m_mem), std::end(*m_mem), std::ref(r));
  }

  zbytebuf(const void *hex, size_t len) {
    m_mem = std::unique_ptr<byte_v>(new byte_v(len));
    for (int i = 0; i < len; ++i) {
      (*m_mem)[i] = ((const unsigned char *)hex)[i];
    }
  }

    zbytebuf(const char* hex_str) {
        std::unique_ptr<byte_v> result(new byte_v());
        std::string hex(hex_str);
        for (size_t i = 0; i < hex.length(); i += 2) {
            std::string byte = hex.substr(i, 2);
            unsigned char chr = (unsigned char)(int)strtol(byte.c_str(), NULL, 16);
            result->push_back(chr);
        }
        m_mem = std::move(result);
    }
    
  zbytebuf(const std::string &hex_str) {
    std::unique_ptr<byte_v> result(new byte_v());
    for (size_t i = 0; i < hex_str.length(); i += 2) {
      std::string byte = hex_str.substr(i, 2);
      unsigned char chr = (unsigned char)(int)strtol(byte.c_str(), NULL, 16);
      result->push_back(chr);
    }
    m_mem = std::move(result);
  }

  zbytebuf(const byte_v &mem) {
    m_mem = std::unique_ptr<byte_v>(new byte_v());
    for (auto &b : mem) {
      m_mem->emplace_back(b);
    }
  }

public:
    zbytebuf& little_ending() {
        std::reverse(m_mem->begin(), m_mem->end());
        return *this;
    }
public:
    template <typename T>
    zbytebuf& append(T ut) {
        for (int i = 0; i < sizeof(T); ++i)
        {
            m_mem->emplace_back((ut >> (8 * (sizeof(T) - i - 1))) & 0xFF);
        }
        return *this;
    }

    template <typename T>
    typename std::enable_if<std::is_same<T, T>::value, T>::type
    read(size_t index) const {
        size_t end_index = std::min(sizeof(T), length() - index);
        T ret = 0;
        for (int i = 0; i < end_index; ++i)
        {
            ret |= (((uint64_t)(*m_mem)[index + i]) << (8 * (sizeof(T) - 1 - i)));
        }
        
        return ret;
    }

  zbytebuf slice(size_t loc, size_t length) const {
    if (loc + length > m_mem->size()) {
      throw std::out_of_range("zbytebuf split out of range");
    }
    return zbytebuf(
        byte_v(m_mem->begin() + loc, m_mem->begin() + loc + length));
  }

public:
  // MARK: copy constructor & assignment operator
  zbytebuf(const zbytebuf &other) {
    m_mem = std::unique_ptr<byte_v>(new byte_v());
    try {
      for (byte &b : other.mem()) {
        m_mem->emplace_back(b);
      }
    } catch (std::exception &e) {
    }
  }

  zbytebuf(zbytebuf &&other) { m_mem = std::move(other.m_mem); }

  zbytebuf &operator=(const zbytebuf &other) {
    if (&other == this) {  return *this; }
    this->m_mem.reset(new byte_v(*(other.m_mem)));
    return *this;
  }

  zbytebuf &operator=(zbytebuf &&other) {
    if (&other == this)
      return *this;

    this->m_mem = std::move(other.m_mem);
    return *this;
  }

  unsigned char &operator[](int i) { return m_mem->at(i); }

  const byte &operator[](int i) const { return m_mem->at(i); }

public:
  // MARK: operators
  zbytebuf &operator+=(const zbytebuf &other) {
    for (auto &ele : other.mem()) {
      m_mem->emplace_back(ele);
    }
    return *this;
  }

  zbytebuf operator+(const zbytebuf &other) {
    zbytebuf result;
    result += *this;
    result += other;
    return result;
  }

  zbytebuf operator~() // ~x, 求逆
  {
    byte_v vec(m_mem->size());
    for (int i = 0; i < m_mem->size(); ++i) {
      vec[i] = ~(*m_mem)[i];
    }
    return zbytebuf(vec);
  }

  zbytebuf &operator^=(const zbytebuf &other) {
    size_t shorter = std::min(m_mem->size(), other.mem().size());
    for (int i = 0; i < shorter; ++i) {
      (*m_mem)[i] ^= other.mem()[i];
    }
    return *this;
  }

  zbytebuf operator^(const zbytebuf &other) {
    zbytebuf result(*this);
    result ^= other;
    return result;
  }

  bool operator==(const zbytebuf &other) const {
    return this == &other || *(this->m_mem) == other.mem();
  }

  bool operator==(const std::string &hex_str) const {
    return hex_str == this->hex_str();
  }

  bool operator!=(const zbytebuf &other) const { return !(*this == other); }

  bool operator!=(const std::string &hex_str) const {
    return hex_str != this->hex_str();
  }

public:
  // MARK: getters
  const byte *bytes() const { return m_mem->data(); }

  size_t length() const { return m_mem->size(); }

  byte_v &mem() const { return *m_mem; }

  std::string utf8_str() const {
    byte_v utf8_bytes(*m_mem);
    if (utf8_bytes.empty() || utf8_bytes.back() != 0) {
      utf8_bytes.push_back(0);
    }
    return std::string((char *)utf8_bytes.data());
  }

  std::string hex_str() const {
    std::stringstream ss;
    for (auto b : *m_mem) {
      ss << std::uppercase << std::setw(2) << std::setfill('0') << std::hex
         << (unsigned int)b;
    }
    return ss.str();
  }

public:
  zbytebuf tail(size_t aNum) const {
    if (length() < aNum) {
      throw std::invalid_argument("zbytebuf out of bounds");
    }
    return this->slice(length() - aNum, aNum);
  }

  zbytebuf front(size_t aNum) const {
    if (length() < aNum) {
      throw std::invalid_argument("zbytebuf out of bounds");
    }
    return this->slice(0, aNum);
  }

  zbytebuf drop_tail(size_t aNum) {
    if (length() < aNum) {
      throw std::invalid_argument("zbytebuf out of bounds");
    }
    return this->slice(0, length() - aNum);
  }

  zbytebuf drop_front(size_t aNum) {
    if (length() < aNum) {
      throw std::invalid_argument("zbytebuf out of bounds");
    }
    return this->slice(aNum, length() - aNum);
  }

  bool startWith(const std::string &prefix) const {
    return this->hex_str().substr(0, prefix.length()) == prefix;
  }

    zbytebuf& pad(std::function<zbytebuf(const zbytebuf& ori)> padding)
    {
        auto tail = padding(*this);
        m_mem->insert(std::end(*m_mem), std::begin(tail.mem()), std::end(tail.mem()));
        return *this;
    }
    
    zbytebuf& pad_head(std::function<zbytebuf(const zbytebuf& ori)> padding)
    {
        auto tail = padding(*this);
        m_mem->insert(std::begin(*m_mem), std::begin(tail.mem()), std::end(tail.mem()));
        return *this;
    }

public:
  // MARK: functional
  template <class T> T map(std::function<T(const zbytebuf &)> atransformer) {
    return atransformer(*this);
  }

public:
  friend std::ostream &operator<<(std::ostream &out, zbytebuf &obj);

private:
  std::unique_ptr<byte_v> m_mem;
};

inline std::ostream &operator<<(std::ostream &out, zbytebuf &obj) {
  for (auto b : *(obj.m_mem)) {
    out << std::setw(2) << std::setfill('0') << std::hex << (unsigned int)b;
  }
  out << std::endl;
  return out;
}

} // namespace excelsecu
