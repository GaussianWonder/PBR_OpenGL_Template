#ifndef __GENERAL_CONCEPTS_H__
#define __GENERAL_CONCEPTS_H__

#include <concepts>

// Is shared ptr
template <typename T> struct is_shared_ptr : std::false_type {};
template <typename T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

template <typename ...T> concept IsSharedPtr = (is_shared_ptr<T>::value && ...);

#endif // __GENERAL_CONCEPTS_H__
