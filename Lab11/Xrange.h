#include<iostream>
#include<cassert>
#include <vector>

template<typename T>
class XrangeType {
private:
    T start;
    T step;
    T finish;
    T XR_temp;

    class InputIterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = T;
        using reference = T&;
        using difference_type = T;

        InputIterator(const T& it_value_, const T& step_) : it_value(it_value_), step(step_){}

        InputIterator(const InputIterator& other_iterator) : it_value(other_iterator.it_value),
                                                            step(other_iterator.step){}

        InputIterator& operator= (const InputIterator& other_iterator) {
            it_value = other_iterator.it_value;
            step = other_iterator.step;
            return *this;
        }

        friend bool operator== (const InputIterator& lhs, const InputIterator& rhs) {
            return (lhs.it_value >= rhs.it_value && lhs.step >= 0 ) || (lhs.it_value <= rhs.it_value && lhs.step <= 0 );
        }

        friend bool operator!= (const InputIterator& lhs, const InputIterator& rhs) {
            return !(lhs == rhs);
        }

        reference operator* () {
            return it_value;
        }

        InputIterator& operator++() {
            it_value += step;
            return *this;
        }

        InputIterator operator++(int) {
            InputIterator temp(*this);
            operator++();
            return temp;
        }


    private:
        T step;
        T it_value;
    };

public:
    XrangeType(T start_, T finish_, T step_) : start(start_), finish(finish_), step(step_){}

    T& operator[](uint64_t n) {
        XR_temp = start;
        for (size_t i = 0; i < n; ++i) {
            XR_temp+=step;
        }
        return XR_temp;
    }

    InputIterator begin() const {
        return InputIterator(start, step);
    }

    InputIterator end() const {
        return InputIterator(finish, step);
    }

};


template<typename T>
XrangeType<T> xrange(T start, T finish, T step) {
    XrangeType<T> sequence(start, finish, step);
    return sequence;
}


template<typename T>
XrangeType<T> xrange(T start, T finish) {
    return xrange(start, finish, 1);
}


template<typename T>
XrangeType<T> xrange(T finish) {
    return xrange(T(), finish, 1);
}
