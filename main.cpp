#include "vector.h"
#include <cassert>
#include <vector>
#include <iostream>

namespace {

    using StdVector = std::vector<Vector::ValueType>;

    void assertEquality(const Vector &actual, const StdVector &expected) {
        assert(actual.Size() == expected.size());
        for (size_t i = 0; i < actual.Size(); ++i) {
            assert(actual[i] == expected[i]);
        }
    }
}  // namespace

bool check_constuctors() {
    {
        Vector a;
        assert(a.Size() == 0u);
        assert(a.Capacity() == 0u);
    }
    {
        Vector a{1, 2, 3, 4};
        assertEquality(a, StdVector{1, 2, 3, 4});
    }
    {
        Vector a(5);
        assertEquality(a, StdVector(5));
    }
    return true;
}

bool check_basic_methods() {
    Vector a{1, 3, 5};
    assert(a.Capacity() == 3u);
    assertEquality(a, StdVector{1, 3, 5});

    a.PopBack();
    assertEquality(a, StdVector{1, 3});
    a.Clear();
    assert(a.Capacity() == 3u);
    assertEquality(a, StdVector());

    a.PushBack(6);
    assertEquality(a, StdVector{6});

    Vector b{3, 4};

    auto *a_data = a.Data();
    auto *b_data = b.Data();

    a.Swap(b);
    assertEquality(a, StdVector{3, 4});
    assertEquality(b, StdVector{6});

    assert(a_data == b.Data());
    assert(b_data == a.Data());
    return true;
}

bool check_modifications() {
    Vector a{3, 7, 8};
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    assertEquality(a, StdVector{1, 2, 3});
    return true;
}


bool check_iterators_1() {
    Vector a(5);
    for (size_t i = 0; i < a.Size(); ++i) {
        a[i] = i;
    }
    {
        Vector::ValueType cur = 0;
        for (auto elem: a) {
            assert(cur++ == elem);
        }
    }

    {
        auto first = a.Begin();
        auto last = a.End();
        assert(static_cast<Vector::SizeType>(last - first) == a.Size());
        assert(*first == 0);

        assert(first == a.Begin());
        assert(first != last);
        assert(first < last);
        assert(first <= last);
        assert(last > first);
        assert(last >= first);

        ++first;
        assert(*first == 1);
        auto tmp = first++;
        assert(*tmp == 1);
        assert(*first == 2);
        auto end = last--;
        assert(end == a.End());
        assert(*last == 4);
        --last;
        assert((first += 1) == last);
        last -= 3;
        assert(*last == 0);

        {
            auto first = a.Begin();
            int size = static_cast<int>(a.Size());
            for (int i = 0; i < size; ++i) {
                for (int diff = -2; diff <= 2; ++diff) {
                    if (i + diff >= 0 && i + diff < size) {
                        assert(*(first + i + diff) == i + diff);
                    }
                }
            }
        }
    }
    return true;
}

bool check_iterators_2() {
    Vector a{1, 3, 5};
    *(a.begin().operator->()) = 2;
    *((--a.end()).operator->()) = 4;

    assertEquality(a, StdVector{2, 3, 4});

    Vector::Iterator it;
    it = a.Begin() + 1;
    assert(*it == 3);
    return true;
}

bool test_realocation() {
    const int steps = 16;
    Vector data;
    for (
            int step = 0;
            step <=
            steps;
            ++step) {
        StdVector ok_data;
        for (
                Vector::ValueType i = 0;
                i <= (1 << step); ++i) {
            data.
                    PushBack(i);
            ok_data.
                    push_back(i);
        }
        assertEquality(data, ok_data
        );
        assert(data.Capacity() == (1u << (step + 1)));
        data.Clear();
    }
    return true;
}

bool check_iterators() {
    return check_iterators_1() && check_iterators_2();
}


bool check_comparison() {
    assert(Vector({1, 2, 3}) == Vector({1, 2, 3}));
    assert(Vector({1, 2, 3}) != Vector({2, 3, 4}));

    assert(Vector({1, 2, 3}) < Vector({2, 3, 4}));
    assert(Vector({1, 2, 3}) < Vector({1, 2, 3, 4}));
    assert(Vector({1, 2, 3, 4}) <= Vector({2}));
    assert(Vector({1, 2, 3, 4}) > Vector());
    assert(Vector({1, 2, 3, 4}) >= Vector({1}));
    return true;
}

bool check_reverse() {
    Vector a;
    a.Reserve(5);
    assert(a.Size() == 0u);
    assert(a.Capacity() == 5u);

    for (Vector::ValueType i = 0; i < 5; ++i) {
        a.PushBack(i);
    }

    assert(a.Capacity() == 5u);
    a.PushBack(4);
    assert(a.Capacity() == 10u);
    a.Reserve(3);
    assert(a.Capacity() == 10u);
    a.Clear();
    assert(a.Size() == 0u);
    assert(a.Capacity() == 10u);
    return true;
}


int main() {
    if (check_basic_methods()) std::cout << "Basic methods are ok" << std::endl;
    if (check_comparison()) std::cout << "Comparison is ok" << std::endl;
    if (check_constuctors()) std::cout << "Constructor is ok" << std::endl;
    if (check_iterators()) std::cout << "Iterators are ok" << std::endl;
    if (check_modifications()) std::cout << "Modifications are ok" << std::endl;
    if (check_reverse()) std::cout << "Reverse is ok" << std::endl;
}
