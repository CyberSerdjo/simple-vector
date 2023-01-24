#pragma once
 
#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
 
#include "array_ptr.h"
 
struct ReserveProxyObj {
    size_t capacity_to_reserve_;
    ReserveProxyObj(size_t capacity_to_reserve)
        : capacity_to_reserve_(capacity_to_reserve) {}
};
 
ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}
 
template <typename Type>
class SimpleVector {
private:
    using Data = ArrayPtr<Type>;
    Data raw_vector_;
    size_t size_ = 0;
    size_t capacity_ = 0;
 
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;
 
    SimpleVector() noexcept = default;
 
    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size);
    // конструктор с резервированием
    SimpleVector(ReserveProxyObj obj);
    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type &value);
    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init);
    // Меняет вектора местами
    void swap(SimpleVector<Type> &other) noexcept;
    // конструктор копирования
    SimpleVector(const SimpleVector& other);
    SimpleVector(SimpleVector&& other);
    // оператор присваивания
    SimpleVector& operator=(const SimpleVector& rhs);
    SimpleVector& operator=(SimpleVector&& rhs);
    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept;
    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept;
    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept;
    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept;
    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept;
    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index);
    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const;
    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept;
    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size);
    // изменяет кол-во зарезервированного места
    void Reserve(size_t new_capacity);
    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept;
    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept;
    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept;
    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept;
    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept;
    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept;
    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(Type item);
    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept;
    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, Type value);
    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos);
};
 
template<typename Type>
void SimpleVector<Type>::swap(SimpleVector<Type> &other) noexcept {
    raw_vector_.swap(other.raw_vector_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}
 
template<typename Type>
SimpleVector<Type>::SimpleVector(size_t size) : raw_vector_(size), size_(size), capacity_(size) {
}
 
template<typename Type>
SimpleVector<Type>::SimpleVector(ReserveProxyObj obj) : SimpleVector() {
    Reserve(obj.capacity_to_reserve_);
}
 
template<typename Type>
SimpleVector<Type>::SimpleVector(size_t size, const Type &value) : SimpleVector(size) {
    std::fill(begin(), end(), value);
}
 
template<typename Type>
SimpleVector<Type>::SimpleVector(std::initializer_list<Type> init) : SimpleVector(init.size ()) {
    std::copy(std::make_move_iterator(init.begin()), std::make_move_iterator(init.end()), begin());
}
 
template<typename Type>
SimpleVector<Type>::SimpleVector(const SimpleVector<Type> &other) {
    SimpleVector<Type> temp(other.size_);
    std::copy(other.begin(), other.end(), temp.begin());
    swap(temp);
}
 
template<typename Type>
SimpleVector<Type>::SimpleVector(SimpleVector &&other) {
    raw_vector_ = std::move(other.raw_vector_);
    size_ = std::exchange(other.size_, 0);
    capacity_ = std::exchange(other.capacity_, 0);
}
 
template<typename Type>
SimpleVector<Type> &SimpleVector<Type>::operator=(const SimpleVector &rhs) {
    if (this != &rhs) {
        SimpleVector<Type> temp(rhs);
        swap(temp);
    }
    return *this;
}
 
template<typename Type>
SimpleVector<Type> &SimpleVector<Type>::operator=(SimpleVector&& rhs) {
    if (this != &rhs) {
        SimpleVector<Type> temp(std::move(rhs));
        swap(temp);
    }
    return *this;
}
 
template<typename Type>
size_t SimpleVector<Type>::GetSize() const noexcept {
    return size_;
}
 
template<typename Type>
size_t SimpleVector<Type>::GetCapacity() const noexcept {
    return capacity_;
}
 
template<typename Type>
bool SimpleVector<Type>::IsEmpty() const noexcept {
    return size_ == 0;
}
 
template<typename Type>
Type &SimpleVector<Type>::operator[](size_t index) noexcept {
    assert(index < size_);
    return raw_vector_[index];
}
 
template<typename Type>
const Type &SimpleVector<Type>::operator[](size_t index) const noexcept {
    assert(index < size_);
    return raw_vector_[index];
}
 
template<typename Type>
Type &SimpleVector<Type>::At(size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Index is out of range");
    }
    return raw_vector_[index];
}
 
template<typename Type>
const Type &SimpleVector<Type>::At(size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Index is out of range");
    }
    return raw_vector_[index];
}
 
template<typename Type>
void SimpleVector<Type>::Clear() noexcept {
    size_ = 0;
}
 
template<typename Type>
void SimpleVector<Type>::Resize(size_t new_size) {
    if(size_ >= new_size)
        {
            size_ = new_size;
            return;
        }
       /* 
        auto gen = []()
        {
            return Type();
        };*/
        
        if(capacity_ >= new_size)
        {

            
            for(size_t i{0}; i < (capacity_ - size_) ; ++i)
            {
                PushBack(std::move(Type{}));
            }
            return;
        }
        
        ArrayPtr<Type> tmp{new_size};

     
        
        std::move(raw_vector_.Get(), 
                  raw_vector_.Get() + size_, 
                  tmp.Get());
        
        raw_vector_.swap(tmp);
        size_ = new_size;
        capacity_ = new_size;
        
        for(size_t i{0}; i < (capacity_ - size_) ; ++i)
        {
            PushBack(std::move(Type{}));            
        }
		
    
}
 
template<typename Type>
void SimpleVector<Type>::Reserve(size_t new_capacity) {
    if (new_capacity > capacity_) {
        SimpleVector<Type> temp(new_capacity);
        std::copy(std::make_move_iterator(begin()), std::make_move_iterator(end()), temp.begin());
        temp.size_ = size_;
        swap(temp);
    }
}
 
template<typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::begin() noexcept {
    return raw_vector_.Get();
}
 
template<typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::begin() const noexcept {
    return raw_vector_.Get();
}
 
template<typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::cbegin() const noexcept {
    return raw_vector_.Get();
}
 
template<typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::end() noexcept {
    return raw_vector_.Get() + size_;
}
 
template<typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::end() const noexcept {
    return raw_vector_.Get() + size_;
}
 
template<typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::cend() const noexcept {
    return raw_vector_.Get() + size_;
}
 
template<typename Type>
void SimpleVector<Type>::PushBack(Type item){
    if (size_ < capacity_) {
        raw_vector_[size_] = std::move(item);
        ++size_;
    } else {
        SimpleVector<Type> temp(std::max(size_ * 2, static_cast<size_t>(1)));
        temp.size_ = size_ + 1;
        std::copy(std::make_move_iterator(begin()), std::make_move_iterator(end()), temp.begin());
        temp[size_] = std::move(item);
        swap(temp);
    }
}
 
template<typename Type>
void SimpleVector<Type>::PopBack() noexcept {
    assert(!IsEmpty());
    --size_;
}
 
template<typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::Insert(ConstIterator pos, Type value) {
    if (pos < begin() || pos > end()) {
        throw std::out_of_range("Iterator is out of range");
    }
    size_t index = static_cast<size_t>(pos - begin());
    if (size_ < capacity_) {
        std::copy_backward(std::make_move_iterator(begin() + index), std::make_move_iterator(end()), end() + 1);
        raw_vector_[index] = std::move(value);
        ++size_;
    } else {
        SimpleVector<Type> temp(std::max(size_ * 2, static_cast<size_t>(1)));
        temp.size_ = size_ + 1;
        std::copy(std::make_move_iterator(begin()), std::make_move_iterator(begin() + index), temp.begin());
        temp[index] = std::move(value);
        std::copy(std::make_move_iterator(begin() + index), std::make_move_iterator(end()), temp.begin() + index + 1);
        swap(temp);
    }
    return begin() + index;
}
 
template<typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::Erase(ConstIterator pos) {
    if (pos < begin() || pos >= end()) {
        throw std::out_of_range("Iterator is out of range");
    }
    size_t index = static_cast<size_t>(pos - begin());
    std::copy(std::make_move_iterator(begin() + index + 1), std::make_move_iterator(end()), begin() + index);
    --size_;
    return begin() + index;
}
 
template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return (&lhs == &rhs)
        || (lhs.GetSize() == rhs.GetSize()
            && std::equal(lhs.begin(), lhs.end(), rhs.begin()));
}
 
template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}
 
template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
 
template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs < lhs);
}
 
template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs <= rhs);
}
 
template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs < rhs);
}
