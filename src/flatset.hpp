#pragma once

#include <iostream>

template <typename T> class Vector {
public:
  Vector();                                   // Default constructor
  explicit Vector(size_t size);               // Constructor with parametrs
  Vector(Vector &&other) noexcept;            // Move constructor
  Vector(const Vector &other);               
  Vector &operator=(const Vector &other);     // Copy assignment operator
  Vector &operator=(Vector &&other) noexcept; // Move assignment operator
  ~Vector();                                  // Default destructor

  void insert(const T &value);
  void merge(const Vector<T> &other);
  T *erase(const T &value);
  T *find(const T &value);
  void print();

  T *begin() const { return array_; }
  T *rbegin() const { return array_ + size_ - 1; }
  T *end() const { return array_ + size_ - 1; }
  T *rend() const { return array_; }

  size_t getSize() const { return size_; }

  size_t binarySearch(const T &value) const;

  T &operator[](size_t index) const;
  bool operator==(const Vector<T> &other) const;

private:
  T *array_;
  size_t size_;
  size_t capacity_;

  void resize(int newCapacity);
};

template <typename T> class FlatSet {
public:
  void insert(T value);
  void merge(const FlatSet<T> &other) { return flatSet_.merge(other.flatSet_); }
  bool contains(const T &value) const;
  T *erase(const T &value) { return flatSet_.erase(value); }
  T *find(const T &value) { return flatSet_.find(value); }
  bool empty() { return flatSet_.getSize() == 0; }
  void print() { flatSet_.print(); }

  T *begin() const { return flatSet_.begin(); }
  T *rbegin() const { return flatSet_.rbegin(); }
  T *end() const { return flatSet_.end(); }
  T *rend() const { return flatSet_.rend(); }

  size_t binarySearch(const T &value) const {
    return flatSet_.binarySearch(value);
  }

  T &operator[](size_t index) const { return flatSet_[index]; }
  bool operator==(const FlatSet<T> &other) const;

private:
  Vector<T> flatSet_;
};

template <typename T> Vector<T>::~Vector() { delete[] array_; }

template <typename T> bool Vector<T>::operator==(const Vector<T> &other) const {
  if (size_ != other.size_) {
    return false;
  }

  for (size_t i = 0; i < size_; ++i) {
    if (array_[i] != other.array_[i]) {
      return false;
    }
  }

  return true;
}

template <typename T> T &Vector<T>::operator[](size_t index) const {
  if (index >= size_) {
    throw std::out_of_range("Index out of range!");
  }
  return array_[index];
}

template <typename T> size_t Vector<T>::binarySearch(const T &value) const {
  int low = 0;
  int high = size_ - 1;

  while (low <= high) {
    int mid = low + (high - low) / 2;

    if (array_[mid] == value) {
      return mid;
    }

    if (array_[mid] < value) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  return low;
}

// Default constructor
template <typename T>
Vector<T>::Vector() : array_(nullptr), size_(0), capacity_(1) {}

// Constructor with parametrs
template <typename T>
Vector<T>::Vector(size_t size)
    : array_(nullptr), size_(size), capacity_(size + 1) {
  array_ = new T[capacity_];
}

template <typename T> Vector<T> &Vector<T>::operator=(const Vector &other) {
  if (this != &other) {
    delete[] array_;

    size_ = other.size_;
    capacity_ = other.capacity_;

    array_ = new T[capacity_];

    for (size_t i = 0; i < size_; ++i) {
      array_[i] = other.array_[i];
    }
  }

  return *this;
}

template <typename T> Vector<T> &Vector<T>::operator=(Vector &&other) noexcept {
  if (this != &other) {
    delete[] array_;

    array_ = other.array_;
    size_ = other.size_;
    capacity_ = other.capacity_;

    other.array_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  return *this;
}

// Move constructor
template <typename T>
Vector<T>::Vector(Vector &&other) noexcept
    : size_(other.size_), array_(other.array_), capacity_(other.capacity_) {
  other.array_ = nullptr;
  other.size_ = 0;
  other.capacity_ = 0;
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector &other)
    : size_(other.size_), capacity_(other.capacity_), array_(new T[capacity_]) {
  std::copy(other.array_, other.array_ + size_, array_);
}

// Resize of the capacity of an array
template <typename T> void Vector<T>::resize(int newCapacity) {
  T *newArray = new T[newCapacity];
  for (size_t i = 0; i < size_; ++i) {
    newArray[i] = array_[i];
  }
  delete[] array_;
  array_ = newArray;
  capacity_ = newCapacity;
}

template <typename T> void Vector<T>::merge(const Vector<T> &other) {
  // Temp array
  T *temp = new T[size_ + other.size_];

  // Merging arrays
  size_t i = 0;
  size_t j = 0;
  size_t k = 0;

  while (i < size_ && j < other.size_) {
    if (array_[i] < other.array_[j]) {
      temp[k++] = array_[i++];
    } else if (array_[i] > other.array_[j]) {
      temp[k++] = other.array_[j++];
    } else {
      temp[k++] = array_[i++];
      j++;
    }
  }

  // Copy remaining elements
  while (i < size_) {
    temp[k++] = array_[i++];
  }
  while (j < other.size_) {
    temp[k++] = other.array_[j++];
  }

  // Empty current array and replace with temp
  delete[] array_;
  array_ = temp;
  size_ = k;
  capacity_ = k;
}

template <typename T> void Vector<T>::insert(const T &value) {
  if (array_ == nullptr) {
    array_ = new T[1];
  }
  size_t insertionPoint = binarySearch(value);

  // If the value is already present
  if (insertionPoint < size_ && array_[insertionPoint] == value) {
    return;
  }

  if (size_ >= capacity_) {
    resize(capacity_ * 2);
  }

  // Shift elements to the right to make room for the new value
  for (size_t i = size_; i > insertionPoint; --i) {
    array_[i] = array_[i - 1];
  }
  // Insert the new value
  array_[insertionPoint] = value;

  // Increment the size of the vector
  ++size_;
}

template <typename T> T *Vector<T>::erase(const T &value) {
  size_t index = binarySearch(value);

  // If the value is found, shift elements to the left to remove it
  if (index < size_ && array_[index] == value) {
    for (size_t i = index; i < size_ - 1; ++i) {
      array_[i] = array_[i + 1];
    }
    --size_;
    return array_ + index;
  }

  return nullptr;
}

template <typename T> T *Vector<T>::find(const T &value) {
  size_t index = binarySearch(value);

  if (index < size_ && array_[index] == value) {
    return &array_[index];
  }

  return nullptr;
}

template <typename T> void Vector<T>::print() {
  for (size_t i = 0; i < size_; ++i) {
    std::cout << array_[i] << " ";
  }
  std::cout << std::endl;
}

template <typename T> void FlatSet<T>::insert(T key) { flatSet_.insert(key); }

template <typename T> bool FlatSet<T>::contains(const T &value) const {
  size_t index = flatSet_.binarySearch(value);
  return (index < flatSet_.getSize() && flatSet_[index] == value);
}

template <typename T>
bool FlatSet<T>::operator==(const FlatSet<T> &other) const {
  return flatSet_ == other.flatSet_;
}
