//
//  zbytebuf.hpp
//  zbytebuf
//
//  Created by zhouhuang on 2019/9/21.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#pragma once

#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>
#include <set>

namespace zzz {
    
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
            std::string hs = hex_str;
            if (hs.length() % 2 == 1)
             {
                 hs = "0" + hex_str;
             }
            for (size_t i = 0; i < hs.length(); i += 2) {
                std::string byte = hs.substr(i, 2);
                unsigned char chr = (unsigned char)(int)strtol(byte.c_str(), NULL, 16);
                result->push_back(chr);
            }
            m_mem = std::move(result);
        }
        
        zbytebuf(const byte_v &mem) {
            m_mem = std::unique_ptr<byte_v>(new byte_v(mem));
        }
        
        zbytebuf(byte_v &mem) {
            m_mem = std::unique_ptr<byte_v>(new byte_v(mem));
        }
        
    public:
        zbytebuf& reverse() {
            std::reverse(m_mem->begin(), m_mem->end());
            return *this;
        }
    public:
        
        // 对于一般整数类型，直接转换有多长，转多长,不超过8字节
        zbytebuf& append(uint64_t t) {
            std::stringstream ss;
            std::string s;
            ss << std::hex << t;
            ss >> s;
            return *this += zbytebuf(s);
        }
        
        // 可以指定转换为字节的长度，补足则前面补0，默认为类型宽度
        template <typename T, int N = sizeof(T)>
        zbytebuf& append(typename std::enable_if<std::is_integral<T>::value, T>::type ut) {
            for (int i = 0; i < N; ++i)
            {
                m_mem->emplace_back((ut >> (8 * (N - i - 1))) & 0xFF);
            }
            return *this;
        }
        
        template <typename T>
        zbytebuf& append(typename std::enable_if<not std::is_integral<T>::value, T>::type ut) {
            zbytebuf temp(ut);
            *this += temp;
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
        
        // slice the unuse part, keep the useful part.
        zbytebuf& slice(size_t loc, size_t len) {
            m_mem->erase(std::begin(*m_mem), std::begin(*m_mem) + loc);
            m_mem->erase(std::begin(*m_mem) + len, std::end(*m_mem));
            return *this;
        }
        
        // create a new sub buffer from current buffer
        zbytebuf sub_buf(size_t loc, size_t len) const{
            if (loc + len > m_mem->size()) {
                throw std::out_of_range("zbytebuf split out of range");
            }
            return zbytebuf(byte_v(m_mem->begin() + loc, m_mem->begin() + loc + len));
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
        
        template <typename T>
        zbytebuf& pop(T& t) {
            t = read<T>(0);
            drop_front(sizeof(T));
            return *this;
        }
        
        template <typename T>
        zbytebuf& scan(T& t, size_t& offset) {
            t = read<T>(offset);
            offset += sizeof(T);
            return *this;
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
        zbytebuf back(size_t aNum) const {
            if (length() < aNum) {
                throw std::invalid_argument("zbytebuf out of bounds");
            }
            return sub_buf(length() - aNum, aNum);
        }
        
        zbytebuf front(size_t aNum) const {
            if (length() < aNum) {
                throw std::invalid_argument("zbytebuf out of bounds");
            }
            return sub_buf(0, aNum);
        }
        
        zbytebuf& drop_back(size_t aNum) {
            if (length() < aNum) {
                throw std::invalid_argument("zbytebuf out of bounds");
            }
            return slice(0, length() - aNum);
        }
        
        zbytebuf& drop_front(size_t aNum) {
            if (length() < aNum) {
                throw std::invalid_argument("zbytebuf out of bounds");
            }
            return slice(aNum, length() - aNum);
        }
        
        bool begin_with(const std::string& prefix) const {
            auto vec = zbytebuf(prefix).mem();
            return begin_with(vec);
        }
        
        bool begin_with(const byte_v& vec) const {
            if (m_mem->size() < vec.size()) return false;
            auto iter1 = m_mem->begin();
            auto iter2 = vec.begin();
            while(iter2 != vec.end()) {
                if (*iter1 != *iter2) {
                    return false;
                }
                iter1++;
                iter2++;
            }
            return true;
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
        
        zbytebuf& reserve(size_t n)
        {
            byte_v temp(n);
            std::reverse(std::begin(*m_mem), std::end(*m_mem));
            return *this;
        }

        template <typename T>
        zbytebuf& modify(size_t index, size_t len, typename std::enable_if<std::is_integral<T>::value, T>::type val) {
            size_t nb = std::min(sizeof(T), len);
            for (int i = 0; i < nb; ++i)
            {
                (*m_mem)[index + i] = ((val >> (8 * (nb - i - 1))) & 0xFF);
            }
            return *this;
        }
        
        template <typename T>
        zbytebuf& modify(size_t index, size_t len, typename std::enable_if<not std::is_integral<T>::value, T>::type val) {
            zbytebuf tem(val);
            size_t nb = std::min(tem.length(), len);
            for (int i = 0; i < nb; ++i)
            {
                (*m_mem)[index + len - 1 - i] = tem[tem.length() - i - 1];
            }
            return *this;
        }

    public:
        // MARK: functional
        zbytebuf& map(std::function<byte (const byte &)> atransformer) {
            for (int i = 0; i < m_mem->size(); ++i) {
                (*m_mem)[i] = atransformer((*m_mem)[i]);
            }
            return *this;
        }
        
        zbytebuf& for_each(std::function<void (const byte &)> block){
            for (auto b: *(this->m_mem)) {
                block(b);
            }
            return *this;
        }
        
        zbytebuf& debug(){
            std::cout << *this << std::endl;
            return *this;
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
