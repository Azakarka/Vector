#include <cassert>
#include "vector.h"

Vector::Iterator::Iterator(ValueType* pointer) {
    iter_ = pointer;
}
Vector::Iterator::Iterator() {
    iter_ = nullptr;
}

Vector::ValueType& Vector::Iterator::operator*() const {
    return *iter_;
}
Vector::ValueType* Vector::Iterator::operator->() const {
    return iter_;
}

Vector::Iterator& Vector::Iterator::operator=(const Vector::Iterator& other) {
    iter_ = other.iter_;
    return *this;
}

Vector::Iterator& Vector::Iterator::operator++() {
    ++iter_;
    return *this;
}
Vector::Iterator Vector::Iterator::operator++(int) {
    auto iter = *this;
    ++iter_;
    return iter;
}
Vector::Iterator& Vector::Iterator::operator--() {
    --iter_;
    return *this;
}
Vector::Iterator Vector::Iterator::operator--(int) {
    auto iter = *this;
    --iter_;
    return iter;
}

Vector::Iterator Vector::Iterator::operator+(Vector::DifferenceType shift) const {
    return Vector::Iterator(iter_ + shift);
}
Vector::DifferenceType Vector::Iterator::operator-(Vector::Iterator other) const {
    return iter_ - other.iter_;
}
Vector::Iterator& Vector::Iterator::operator+=(Vector::DifferenceType shift) {
    iter_ += shift;
    return *this;
}
Vector::Iterator& Vector::Iterator::operator-=(Vector::DifferenceType shift) {
    iter_ -= shift;
    return *this;
}

bool Vector::Iterator::operator==(const Vector::Iterator& other) const {
    return iter_ == other.iter_;
}
bool Vector::Iterator::operator!=(const Vector::Iterator& other) const {
    return !(*this == other);
}
std::strong_ordering Vector::Iterator::operator<=>(const Vector::Iterator& other) const {
    return iter_ <=> other.iter_;
}

Vector::Vector() {
}
Vector::Vector(size_t size) {
    AllocateNewMemory(size);
    size_ = size;
    for (Vector::SizeType i = 0; i < size; ++i) {
        arr_[i] = 0;
    }
}
Vector::Vector(std::initializer_list<Vector::ValueType> list) {
    AllocateNewMemory(list.size());
    size_ = list.size();
    Vector::SizeType counter = 0;
    for (const auto& k : list) {
        arr_[counter] = k;
        ++counter;
    }
}
Vector::Vector(const Vector& other) {
    (*this) = other;
}
Vector& Vector::operator=(const Vector& other) {
    AllocateNewMemory(other.capacity_);
    size_ = other.size_;
    for (Vector::SizeType i = 0; i < size_; ++i) {
        arr_[i] = other.arr_[i];
    }
    return *this;
}

Vector::~Vector() {
    if (capacity_ != 0) {
        delete[] arr_;
    }
}

Vector::SizeType Vector::Size() const {
    return size_;
}
Vector::SizeType Vector::Capacity() const {
    return capacity_;
}

const Vector::ValueType* Vector::Data() const {
    return arr_;
}

Vector::ValueType& Vector::operator[](size_t position) {
    return arr_[position];
}
Vector::ValueType Vector::operator[](size_t position) const {
    return arr_[position];
}

bool Vector::operator==(const Vector& other) const {
    if (other.size_ != size_) {
        return false;
    }
    return (*this <=> other) == 0;
}
bool Vector::operator!=(const Vector& other) const {
    return !(*this == other);
}
std::strong_ordering Vector::operator<=>(const Vector& other) const {
    for (Vector::SizeType i = 0; i < std::min(size_, other.size_); ++i) {
        if (arr_[i] < other.arr_[i]) {
            return std::strong_ordering::less;
        }
        if (arr_[i] > other.arr_[i]) {
            return std::strong_ordering::greater;
        }
    }
    if (size_ < other.size_) {
        return std::strong_ordering::less;
    }
    if (size_ > other.size_) {
        return std::strong_ordering::greater;
    }
    return std::strong_ordering::equal;
}

void Vector::Reserve(Vector::SizeType new_capacity) {
    if (capacity_ >= new_capacity) {
        return;
    }
    AllocateNewMemory(new_capacity);
}
void Vector::Clear() {
    size_ = 0;
}

void Vector::AllocateNewMemory(Vector::SizeType new_capacity) {
    auto* arr = new Vector::ValueType[new_capacity];
    for (Vector::SizeType i = 0; i < size_; ++i) {
        arr[i] = arr_[i];
    }
    if (capacity_ != 0) {
        delete[] arr_;
    }
    capacity_ = new_capacity;
    arr_ = arr;
}

void Vector::PushBack(const Vector::ValueType& new_element) {
    if (size_ == capacity_) {
        AllocateNewMemory(capacity_ == 0 ? 1 : capacity_ << 1);
    }
    arr_[size_] = new_element;
    ++size_;
}
void Vector::PopBack() {
    assert(size_ != 0);
    --size_;
}

void Vector::Swap(Vector& other) {
    std::swap(arr_, other.arr_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

Vector::Iterator Vector::Begin() {
    return begin();
}
Vector::Iterator Vector::End() {
    return end();
}

Vector::Iterator Vector::begin() {
    return Iterator(arr_);
}
Vector::Iterator Vector::end() {
    return Iterator(arr_ + size_);
}