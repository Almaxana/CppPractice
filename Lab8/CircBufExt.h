#pragma once
#include "CircBuf.h"

template <typename T, typename A = std::allocator<T>>
class CCircularBufferExt : public CCircularBuffer<T, A>  {
public:
    uint8_t EXTEND_COEFF = 2;

    using Base = CCircularBuffer<T, A>;

    using typename Base::value_type;
    using typename Base::reference;
    using typename Base::const_reference;
    using typename Base::difference_type;
    using typename Base::size_type;
    using typename Base::iterator;
    using typename Base::const_iterator;


    CCircularBufferExt() : CCircularBuffer <T, A> () {};
    CCircularBufferExt(CCircularBufferExt& other_buf) : CCircularBuffer <T, A> (other_buf) {};
    CCircularBufferExt(size_type n, const value_type& t) : CCircularBuffer <T, A> (n, t) {};
    CCircularBufferExt(const iterator& i, const iterator& j) : CCircularBuffer <T, A> (i, j) {};

    iterator insert(const_iterator p, value_type t) override {
        insert(p, 1, t);
    }

    iterator insert(const_iterator p, size_type n, value_type t) {
        size_t head = p - Base::cbegin();
        size_t tail = Base::filled - head;

        if (Base::filled + n > Base::capacity) {

            T* old_body= Base::body;
            int old_capacity = Base::capacity;

            while (Base::capacity < Base::filled + n) {
                Base::capacity *= EXTEND_COEFF;
            }

            Base::body = Base::allocator.allocate(Base::capacity);


            Base::filled = 0;

            T* p_temp = Base::p_begin;
            std::make_signed_t<size_t> delta_temp = Base::delta_begin;

            Base::p_begin = Base::body;
            Base::delta_begin = 0;

            Base::p_end = Base::body;
            Base::delta_end = 0;

            for (size_t i = 0; i < head; ++i) {
                push_back(*(p_temp));
                Base::inc_p(p_temp, delta_temp, old_capacity, old_body);
            }

            for (size_t i = 0; i < n; ++i) {
                push_back(t);
            }

            for (size_t i = 0; i < tail; ++i) {
                push_back(*(p_temp));
                Base::inc_p(p_temp, delta_temp, old_capacity, old_body);
            }

            for (size_t i = 0; i < old_capacity; ++i) {
                Base::allocator.destroy(old_body + i);
            }
            Base::allocator.deallocate(old_body, old_capacity);

            return iterator((Base::body + head), head, this);
        }

        T* p_read = Base::p_begin;
        std::make_signed_t<size_t> delta_read = Base::delta_begin;

        T* p_write = Base::p_begin;
        std::make_signed_t<size_t> delta_write = Base::delta_begin;
        for (size_t i = 0; i < n; ++i) {
            Base::dec_p(p_write, delta_write, Base::capacity, Base::body);
            Base::dec_p(Base::p_begin, Base::delta_begin, Base::capacity, Base::body);
        }


        for (size_t i = 0; i < head; ++i) {
            *p_write = *p_read;
            Base::inc_p(p_read, delta_read, Base::capacity, Base::body);
            Base::inc_p(p_write, delta_write, Base::capacity, Base::body);
        }

        for (size_t i = 0; i < n; ++i) {
            *p_write = t;
            Base::inc_p(p_write, delta_write, Base::capacity, Base::body);
        }
        Base::filled+=n;
        return iterator(Base::p_begin + head, Base::delta_begin + head, this);

    }


    iterator insert(const_iterator p, iterator i, iterator j) {

        int n = j - i;

        size_t head = p - Base::cbegin();
        size_t tail = Base::filled - head;

        if (Base::filled + n > Base::capacity) {
            T* old_body= Base::body;
            int old_capacity = Base::capacity;

            while (Base::capacity < Base::filled + n) {
                Base::capacity *= EXTEND_COEFF;
            }


            Base::body = Base::allocator.allocate(Base::capacity);


            Base::filled = 0;

            T* p_temp = Base::p_begin;
            std::make_signed_t<size_t> delta_temp = Base::delta_begin;

            Base::p_begin = Base::body;
            Base::delta_begin = 0;

            Base::p_end = Base::body;
            Base::delta_end = 0;

            for (size_t k = 0; k < head; ++k) {
                push_back(*(p_temp));
                Base::inc_p(p_temp, delta_temp, old_capacity, old_body);
            }

            for (size_t k = 0; k < n; ++k) {
                push_back(*(i + k));
            }

            for (size_t k = 0; k < tail; ++k) {
                push_back(*(p_temp));
                Base::inc_p(p_temp, delta_temp, old_capacity, old_body);
            }

            for (size_t i = 0; i < old_capacity; ++i) {
                Base::allocator.destroy(old_body + i);
            }
            Base::allocator.deallocate(old_body, old_capacity);

            return iterator((Base::body + head), head, this);
        }


        T* p_read = Base::p_begin;
        std::make_signed_t<size_t> delta_read = Base::delta_begin;

        T* p_write = Base::p_begin;
        std::make_signed_t<size_t> delta_write = Base::delta_begin;
        for (size_t i = 0; i < n; ++i) {
            Base::dec_p(p_write, delta_write, Base::capacity, Base::body);
            Base::dec_p(Base::p_begin, Base::delta_begin, Base::capacity, Base::body);
        }


        for (size_t i = 0; i < head; ++i) {
            *p_write = *p_read;
            Base::inc_p(p_read, delta_read, Base::capacity, Base::body);
            Base::inc_p(p_write, delta_write, Base::capacity, Base::body);
        }

        for (size_t k = 0; k < n; ++k) {
            *p_write = *(i + k);
            Base::inc_p(p_write, delta_write, Base::capacity, Base::body);
        }
        Base::filled+=n;

        return iterator(Base::p_begin + head, Base::delta_begin + head, this);
    }

    iterator insert(const_iterator p, CCircularBufferExt& il) {
        insert(p, il.begin(), il.end());
    }


    void push_back(value_type t) override{
        if (Base::filled == Base::capacity) {
            T* old_body = Base::body;
            int old_capacity = Base::capacity;
            Base::capacity = old_capacity * EXTEND_COEFF;
            Base::body = Base::allocator.allocate(Base::capacity);

            re_record(old_body, Base::body, Base::p_begin, Base::delta_begin, Base::p_end, Base::delta_end, Base::filled, old_capacity);
        }

        *(Base::p_end) = t;
        ++Base::filled;
        Base::inc_p(Base::p_end, Base::delta_end, Base::capacity, Base::body);

    }

    void push_front(value_type t) override {
        if (Base::filled == Base::capacity) {
            T* old_body = Base::body;
            int old_capacity = Base::capacity;
            Base::capacity = old_capacity * EXTEND_COEFF;
            Base::body = Base::allocator.allocate(Base::capacity);

            re_record(old_body, Base::body, Base::p_begin, Base::delta_begin, Base::p_end, Base::delta_end, Base::filled, old_capacity);
        }

        Base::dec_p(Base::p_begin, Base::delta_begin, Base::capacity, Base::body);
        *(Base::p_begin) = t;
        ++Base::filled;
    }


    void re_record(T* old_body, T* new_body, T*& p_begin, std::make_signed_t<size_t>& delta_begin, T*& p_end, std::make_signed_t<size_t>& delta_end, int n, int old_cap) {
        for (size_t i = 0; i < n; ++i) {
            Base::allocator.construct(new_body + i, *(p_begin));
            Base::inc_p(p_begin, delta_begin, old_cap, old_body);
        }

        p_begin = new_body;
        delta_begin = 0;

        p_end = new_body + n;
        delta_end = n;

        for (size_t i = 0; i < old_cap; ++i) {
            Base::allocator.destroy(old_body + i);
        }
        Base::allocator.deallocate(old_body, old_cap);
    }

};
