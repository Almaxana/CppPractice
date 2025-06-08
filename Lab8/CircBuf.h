#pragma once

#include <iostream>
#include <stdexcept>
#include <memory>
#include <iterator>


template<typename T, typename Allocator = std::allocator<T>>
class CCircularBuffer {
public:

    template<typename U, typename clear_value_type>
    class Iterator {
    public:
        friend class CCircularBuffer;

        using iterator_category = std::random_access_iterator_tag;
        using value_type = clear_value_type;
        using difference_type = std::make_signed_t<size_t>;
        using pointer = U*;
        using reference = U&;

        operator Iterator<const U, value_type>() {
            if (std::is_const(point)) return *this;
            return Iterator<const U, value_type>(const_cast<U*>(point), delta, buf);
        }

        Iterator(CCircularBuffer* buf_) {
            buf = buf_;
            delta = 0;
            point = nullptr;
        }

        Iterator(U* point_, std::make_signed_t<size_t> delta_, CCircularBuffer* buf_) {
            buf = buf_;
            delta = delta_;
            point = point_;
        }

        Iterator(U* point_, std::make_signed_t<size_t> delta_, const CCircularBuffer* buf_) {
            buf = buf_;
            delta = delta_;
            point = point_;
        }

        Iterator(const Iterator& other_it) {
            point = other_it.point;
            buf = other_it.buf;
            delta = other_it.delta;
        }

        Iterator& operator=(const Iterator& other_it) {
            point = other_it.point;
            buf = other_it.buf;
            delta = other_it.delta;

            return *this;
        }


        friend bool operator==(const Iterator& lsh, const Iterator& rsh) {
            return lsh.point == rsh.point && lsh.delta == rsh.delta;
        }

        friend bool operator!=(const Iterator& lsh, const Iterator& rsh) {
            return !(lsh == rsh);
        }

        reference operator*() {
            return *point;

        }

        pointer operator->() {
            return point;
        }

        Iterator& operator++() {
            point = buf->body + ((point - buf->body + 1) % (buf->capacity));
            ++delta;

            return *this;
        }

        friend Iterator operator+(const Iterator& iter, difference_type n) {
            Iterator<U, value_type> temp = iter;
            return temp += n;
        }

        friend Iterator operator+(difference_type n, const Iterator& iter) {
            Iterator<U, value_type> temp = iter;
            return temp += n;
        }

        friend difference_type operator-(const Iterator& lsh, const Iterator& rsh) {
            return lsh.delta - rsh.delta;
        }

        friend Iterator operator-(const Iterator& iter, difference_type n) {
            Iterator temp = iter;
            return temp += -n;
        }

        Iterator operator++(int) {
            Iterator temp(*this);
            operator++();
            return temp;
        }

        Iterator& operator--() {
            if (point == buf->body) {
                point = buf->body + buf->capacity - 1;
            } else --point;
            delta--;

            return *this;
        }

        Iterator& operator--(int) {
            Iterator temp(*this);
            operator--();
            return temp;
        }

        Iterator& operator+=(difference_type n) {
            if (n >=0) {
                delta+=n;
                n = n % buf->capacity;
                if (point - buf->body + n >= buf->capacity) {
                    point = buf->body + (n - (buf->capacity - (point - buf->body)));
                } else {
                    point+=n;
                }

            } else {
                n*=(-1);
                delta-=n;
                n = n % buf->capacity;
                if (n > point - buf->body) {
                    point = buf->body + buf->capacity - (n - ((point - buf->body)));
                } else {
                    point-=n;
                }

            }

            return *this;
        }

        Iterator& operator-=(difference_type n) {
            (*this) += -n;
            return *this;
        }

        reference operator[](difference_type n) {
            return *(buf->begin() + (n % buf->capacity));
        };

        friend bool operator<(const Iterator& lsh, const Iterator& rsh) {
            return rsh.point - lsh.point > 0;
        }

        friend bool operator>(const Iterator& lsh, const Iterator& rsh) {
            return rsh.point - lsh.point < 0;
        }

        friend bool operator>=(const Iterator& lsh, const Iterator& rsh) {
            return !(lsh < rsh);
        }

        friend bool operator<=(const Iterator& lsh, const Iterator& rsh) {
            return !(lsh > rsh);
        }

    protected:
        const CCircularBuffer* buf;
        U* point;

        Iterator() = default;

        difference_type delta; ///offset from body

    };


    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using difference_type = std::make_signed_t<size_t>;
    using size_type = size_t;
    using iterator = Iterator<T, T>;
    using const_iterator = Iterator<const T, T>;

    friend class Iterator<T, T>;


    CCircularBuffer() {
        body = allocator.allocate(1);

        p_begin = body;
        delta_begin = 0;

        p_end = body;
        delta_end = 0;

        filled = 0;
        capacity = 1;

    }

    CCircularBuffer(const CCircularBuffer& other_buf) : CCircularBuffer(other_buf.begin(), other_buf.end()) {}

    CCircularBuffer(size_type n, const value_type& t) {
        body = allocator.allocate(n);

        filled = 0;
        capacity = n;

        p_begin = body;
        delta_begin = 0;

        p_end = body;
        delta_end = 0;

        for (size_t i = 0; i < n; i++) {
            allocator.construct(p_end, t);
            inc_p(p_end, delta_end, capacity, body);
            ++filled;
        }
    }

    CCircularBuffer(const iterator& i, const iterator& j) {
        body = allocator.allocate(j - i);

        filled = 0;
        capacity = j - i;

        p_begin = body;
        delta_begin = 0;

        p_end = body;
        delta_end = 0;

        for (size_t k = 0; k < j - i; ++k) {
            allocator.construct(p_end, *(i + k));
            inc_p(p_end, delta_end, capacity, body);
            ++filled;
        }
    }

    ~CCircularBuffer() {

        for (size_t i = 0; i < capacity; ++i) {
            allocator.destroy(body + i);
        }
        allocator.deallocate(body, capacity);

    }

    CCircularBuffer& operator=(const CCircularBuffer& other_buf) {
        if (this == &other_buf) return *this;

        for (size_t i = 0; i < capacity; ++i) {
            allocator.destroy(body + i);
        }
        allocator.deallocate(body, capacity);
        body = allocator.allocate(other_buf.capacity);

        p_begin = body;
        delta_begin = 0;

        p_end = body;
        delta_end = 0;

        filled = 0;
        capacity = other_buf.capacity;

        for (size_t i = 0; i < other_buf.filled; ++i) {
            allocator.construct(p_end, *(other_buf.p_begin + i));
            inc_p(p_end, delta_end, capacity, body);
            ++filled;
        }

        return *this;
    }

    bool empty() {
        return filled == 0;
    }

    Iterator<T, value_type> begin() {
        return Iterator<T, value_type>(p_begin, delta_begin, this);
    }

    Iterator<T, value_type> end() {
        return Iterator<T, value_type>(p_end, delta_end, this);
    }

    Iterator<T, value_type> begin() const {
        return Iterator<T, value_type>(p_begin, delta_begin, this);
    }

    Iterator<T, value_type> end() const{
        return Iterator<T, value_type>(p_end, delta_end, this);
    }

    Iterator<const T, value_type> cbegin() const{
        return Iterator<const T, value_type>(p_begin, delta_begin, this);
    }

    const_iterator cend() const{
        return Iterator<const T, value_type>(p_end, delta_end, this);
    }

    size_type size() {
        return filled;
    }

    size_type max_size() const {
        return capacity;
    }

    void clear() {
        while (filled != 0) {
            pop_front();
        }
    }

    friend bool operator==(CCircularBuffer& lhs, CCircularBuffer& rhs) {
        return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    friend bool operator!=(CCircularBuffer& lhs, CCircularBuffer& rhs) {
        return !(lhs == rhs);
    }


    reference front() {
        return *begin();
    }

    const_reference front() const {
        return *begin();
    }

    reference back() {
        auto tmp = end();
        --tmp;
        return *tmp;
    }

    const_reference back() const {
        auto tmp = end();
        --tmp;
        return *tmp;
    }

    reference operator[](size_type n) {
        return *(begin() + (n % capacity));

    }

    const_reference operator[](size_type n) const {
        return *(begin() + (n % capacity));
    }

    void resize(size_type n) {
        if (filled >= n) {
            while (filled != n) {
                pop_back();
            }
        } else {
            if (capacity >= n) {
                while (filled != n) {
                    push_back(*body);
                }
            } else {
                T* old_body = body;
                size_t old_capacity = capacity;

                body = allocator.allocate(n);
                capacity = n;

                p_end = body;
                delta_end = 0;

                p_begin = body;
                delta_begin = 0;

                filled = 0;


                for (size_t i = 0; i < n; ++i) {
                    allocator.construct(p_end, *(old_body + (i % old_capacity)));
                    inc_p(p_end, delta_end, capacity, body);
                    ++filled;
                }

                for (size_t i = 0; i < old_capacity; ++i) {
                    allocator.destroy(old_body + i);
                }
                allocator.deallocate(old_body, old_capacity);
            }
        }
    }

    void reserve(size_type new_cap) {
        if (new_cap > capacity) {

            T* old_body = body;
            size_t old_capacity = capacity;

            body = allocator.allocate(new_cap);
            capacity = new_cap;

            p_end = body;
            delta_end = 0;


            for (size_t i = 0; i < filled; i++) {
                allocator.construct(p_end, *(old_body + (p_begin - old_body + i) % old_capacity));
                inc_p(p_end, delta_end, capacity, body);
            }

            p_begin = body;
            delta_begin = 0;

            for (size_t i = 0; i < capacity; ++i) {
                allocator.destroy(old_body + i);
            }
            allocator.deallocate(old_body, capacity);
        }
    }

    virtual void push_back(value_type t) {
        if (filled == capacity) {
            pop_front();
        }
        *p_end = t;
        inc_p(p_end, delta_end, capacity, body);
        ++filled;
    }

    virtual void push_front(value_type t) {
        if (filled == capacity) {
            pop_back();
        }

        dec_p(p_begin, delta_begin, capacity, body);

        *p_begin = t;
        ++filled;
    }

    void pop_front() {
        allocator.destroy(p_begin);
        inc_p(p_begin, delta_begin, capacity, body);
        --filled;
    }

    void pop_back() {
        allocator.destroy(p_end - 1);

        dec_p(p_end, delta_end, capacity, body);

        --filled;
    }

    void swap(CCircularBuffer& lhs, CCircularBuffer& rhs) {
        lhs.swap(rhs);
    }

    void swap(CCircularBuffer& rhs) {
        std::swap(p_end, rhs.p_end);
        std::swap(delta_end, rhs.delta_end);
        std::swap(p_begin, rhs.p_begin);
        std::swap(delta_begin, rhs.delta_begin);
        std::swap(body, rhs.body);
        std::swap(filled, rhs.filled);
        std::swap(capacity, rhs.capacity);
    }

    virtual iterator insert(const_iterator p, value_type t) {
        return insert(p, 1, t);
    }

    virtual iterator insert(const_iterator p, size_type n, value_type t) {

        if (n == 0) {
            return iterator(const_cast<T*>(p.point), p.delta, this);
        }

        size_t tail = delta_end - p.delta;
        size_t head = filled - tail;

        if (filled + n <=capacity) {
            T* p_read = p_begin;
            std::make_signed_t<size_t> delta_read = delta_begin;

            T* p_write = p_begin;
            std::make_signed_t<size_t> delta_write = delta_begin;
            for (size_t i = 0; i < n; ++i) {
                dec_p(p_write, delta_write, capacity, body);
                dec_p(p_begin, delta_begin, capacity, body);
            }


            for (size_t i = 0; i < head; ++i) {
                *p_write = *p_read;
                inc_p(p_read, delta_read, capacity, body);
                inc_p(p_write, delta_write, capacity, body);
            }

            for (size_t i = 0; i < n; ++i) {
                *p_write = t;
                inc_p(p_write, delta_write, capacity, body);
            }
            filled+=n;
            return iterator(p_begin + head, delta_begin + head, this);


        }

        T* old_body = body;
        body = allocator.allocate(capacity);



        T* p_temp = p_end;
        std::make_signed_t<size_t> delta_temp = delta_end;
        dec_p(p_temp, delta_temp, capacity, old_body);

        p_begin = body;
        delta_begin = capacity;

        p_end = body;
        delta_end = capacity;

        filled = 0;

        for (size_t i = 0; i < tail; ++i) {
            push_front(*p_temp);
            dec_p(p_temp, delta_temp, capacity, old_body);
        }

        for (size_t i = 0; i < n; ++i) {
            push_front(t);
        }

        for (size_t i = 0; i < head; ++i) {
            push_front(*p_temp);
            dec_p(p_temp, delta_temp, capacity, old_body);
        }

        for (size_t i = 0; i < capacity; ++i) {
            allocator.destroy(old_body + i);
        }
        allocator.deallocate(old_body, capacity);

        return iterator(body + capacity - tail - n, capacity - tail - n, this);

    }

    virtual iterator insert(const_iterator p, iterator i, iterator j) {
        size_t n = j - i;

        if (n == 0) {
            return iterator(const_cast<T*>(p.point), p.delta, this);
        }

        size_t tail = delta_end - p.delta;
        size_t head = filled - tail;

        if (filled + n <= capacity) {
            T* p_read = p_begin;
            std::make_signed_t<size_t> delta_read = delta_begin;

            T* p_write = p_begin;
            std::make_signed_t<size_t> delta_write = delta_begin;
            for (size_t i = 0; i < n; ++i) {
                dec_p(p_write, delta_write, capacity, body);
                dec_p(p_begin, delta_begin, capacity, body);
            }


            for (size_t i = 0; i < head; ++i) {
                *p_write = *p_read;
                inc_p(p_read, delta_read, capacity, body);
                inc_p(p_write, delta_write, capacity, body);
            }

            for (size_t k = 0; k < n; ++k) {
                *p_write = *(i + k);
                inc_p(p_write, delta_write, capacity, body);
            }
            filled+=n;
            return iterator(p_begin + head, delta_begin + head, this);
        }

        T* old_body = body;
        body = allocator.allocate(capacity);



        T* p_temp = p_end;
        std::make_signed_t<size_t> delta_temp = delta_end;
        dec_p(p_temp, delta_temp, capacity, old_body);

        p_begin = body;
        delta_begin = capacity;

        p_end = body;
        delta_end = capacity;

        filled = 0;

        for (size_t i = 0; i < tail; ++i) {
            push_front(*p_temp);
            dec_p(p_temp, delta_temp, capacity, old_body);
        }

        for (size_t i = 0; i < n; ++i) {
            push_front(*(j - 1 - i));
        }

        for (size_t i = 0; i < head; ++i) {
            push_front(*p_temp);
            dec_p(p_temp, delta_temp, capacity, old_body);
        }

        for (size_t i = 0; i < capacity; ++i) {
            allocator.destroy(old_body + i);
        }
        allocator.deallocate(old_body, capacity);

        return iterator(body + capacity - tail - n, capacity - tail - n, this);
    }

    virtual iterator insert(const_iterator p, CCircularBuffer& il) {
        return insert(p, il.begin(), il.end());
    }

    iterator erase(const_iterator q) {
        return erase(q, std::next(q));
    }

    iterator erase(const_iterator q1, const_iterator q2) {
        T* old_body = body;
        body = allocator.allocate(capacity);

        size_t head = q1.delta - begin().delta;
        for (size_t i = 0; i < head; ++i) {
            allocator.construct(body + i, *(old_body + i));
        }

        size_t tail = end().delta - q2.delta;
        for (size_t i = 0; i < tail; ++i) {
            allocator.construct(body + head + i, *(old_body + head + (q2 - q1) + i));
        }

        for (size_t i = 0; i < capacity; ++i) {
            allocator.destroy(old_body + i);
        }
        allocator.deallocate(old_body, capacity);

        filled -= (q2 - q1);

        p_begin = body;
        delta_begin = 0;

        p_end = body + filled;
        delta_end = filled;

        return iterator(const_cast<T*>(q2.point), q2.delta, this);
    }

    void assign(iterator i, iterator j) {
        std::uninitialized_copy(i, j, begin());
    }

    void assign(CCircularBuffer& il) {
        assign(il.begin(), il.end());
    }

    void assign(size_type n, value_type t) {
        std::fill(begin(), begin() + n, t);
    }

protected:
    size_t capacity;
    Allocator allocator;

    T* p_begin;
    std::make_signed_t<size_t> delta_begin;

    T* p_end;
    std::make_signed_t<size_t> delta_end;

    T* body;
    size_t filled;

    void inc_p(T*& point, std::make_signed_t<size_t>& delta, size_t capacity, T* body) {
        point = body + ((point - body + 1) % (capacity));
        ++delta;
    }

    void dec_p(T*& point, std::make_signed_t<size_t>& delta, size_t capacity, T* body) {
        if (point == body) {
            point = body + capacity - 1;
        } else --point;
        delta--;
    }

};
