#include<iostream>
#include<iterator>

template <typename Cont1, typename Cont2>
class ZipType{

private:
    const Cont1& cont1;
    const Cont2& cont2;

    class ZipInputIterator {
    public:
        using It1 = typename Cont1::const_iterator;
        using It2 = typename Cont2::const_iterator;
        using iterator_category = std::input_iterator_tag;
        using value_type = std::pair<typename It1::value_type, typename It2::value_type>;
        using reference = value_type&;
        using pointer = value_type*;
    private:
        It1 it1;
        It2 it2;
        value_type object;
    public:
        ZipInputIterator(const It1 it1_, const It2 it2_) : it1(it1_), it2(it2_){
            object = {*it1, *it2};
        }

        ZipInputIterator(const ZipInputIterator& other_it) : it1(other_it.it1), it2(other_it.it2), object(other_it.object){}

        ZipInputIterator& operator=(const ZipInputIterator& other_it) {
            if(&other_it == this) return *this;

            it1 = other_it.i_it1;
            it2 = other_it.i_it2;
            object = other_it.object;

            return *this;
        }

        ZipInputIterator& operator++() {
            ++it1;
            ++it2;
            object = {*it1, *it2};
            return *this;
        }

        ZipInputIterator operator++(int) {
            ZipInputIterator temp(*this);
            operator++();
            return temp;
        }

        reference operator*() {
            return object;
        }

        pointer operator->() {
            return &object;
        }

        friend bool operator== (const ZipInputIterator& lhs, const ZipInputIterator& rhs) {

            return lhs.it1 == rhs.it1 || lhs.it2 == rhs.it2;
        }

        friend bool operator!= (const ZipInputIterator& lhs, const ZipInputIterator& rhs) {
            return !(lhs==rhs);
        }

    };


public:
    ZipType(const Cont1& cont1_, const Cont2& cont2_) : cont1(cont1_), cont2(cont2_){}

    ZipInputIterator begin() const {
        return ZipInputIterator(cont1.cbegin(), cont2.cbegin());
    }

    ZipInputIterator end() const {
        return ZipInputIterator(cont1.cend(), cont2.cend());
    }

};


template <typename T, typename U>
ZipType<T, U> zip(const T& container1, const U& container2){
    ZipType pairs (container1, container2);
    return pairs;
}
