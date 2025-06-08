#include <iostream>

namespace CustomAlgo {
    template<typename InputIterator, typename Predicate>
    bool all_of(InputIterator start, InputIterator end, Predicate predicate) {
        while (start != end) {
            if (!predicate(*start)) return false;
            ++start;
        }

        return true;
    }

    template<typename InputIterator, typename Predicate>
    bool any_of(InputIterator start, InputIterator end, Predicate predicate) {
        while (start != end) {
            if (predicate(*start)) return true;
            ++start;
        }

        return false;
    }

    template<typename InputIterator, typename Predicate>
    bool none_of(InputIterator start, InputIterator end, Predicate predicate) {
        return !CustomAlgo::any_of(start, end, predicate);
    }

    template<typename InputIterator, typename Predicate>
    bool one_of(InputIterator start, InputIterator end, Predicate predicate) {
        bool flag = false;
        while (start != end) {
            if (predicate(*start)) {
                if (flag) return false;
                flag = true;
            }
            ++start;
        }

        return flag;
    }

    template<typename ForwardIterator, typename Predicate>
    bool is_sorted(ForwardIterator start, ForwardIterator end, Predicate predicate) {
        ForwardIterator next = start;
        while (++next != end) {
            if (!predicate(*start, *next)) return false;
            ++start;
        }

        return true;
    }

    template<typename InputIterator, typename Predicate>
    bool is_partitioned(InputIterator start, InputIterator end, Predicate predicate) {
        bool flag = false;
        while (start != end && predicate(*start)) {
            flag = true;
            ++start;
        }
        if (!flag) return false;
        while (start != end && !predicate(*start)) {
            flag = false;
            ++start;
        }
        return start == end && !flag;
    }

    template<typename InputIterator, typename Predicate>
    InputIterator find_not(InputIterator start, InputIterator end, Predicate predicate) {
        while (start != end && predicate(*start)) {
            ++start;
        }

        return start;
    }

    template<typename BidirectionalIterator, typename Predicate>
    BidirectionalIterator find_backward(BidirectionalIterator start, BidirectionalIterator end, Predicate predicate) {
        --start;
        --end;
        while (end != start && !predicate(*end)) {
            --end;
        }

        return end;
    }

    template<typename BidirectionalIterator, typename Predicate>
    bool is_palindrome(BidirectionalIterator start, BidirectionalIterator end, Predicate predicate) {
        --end;
        while (end != start) {
            if (predicate(*end) != predicate(*start)) return false;
            --end;
            ++start;
        }

        return true;
    }

}
