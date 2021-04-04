#pragma once

// STL containers
#include <memory>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

// Smart pointer
template<class T, class Deleter = std::default_delete<T>>
using UniquePtr = std::unique_ptr<T, Deleter>;

template<class T>
using SharedPtr = std::shared_ptr<T>;

template<class T>
using WeakPtr = std::weak_ptr<T>;

template<class T, class Deleter = std::default_delete<T>>
using UPtr = std::unique_ptr<T, Deleter>;

template<class T>
using SPtr = std::shared_ptr<T>;

template<class T>
using WPtr = std::weak_ptr<T>;

// Containers
template<class T, class Allocator = std::allocator<T>>
using Vector = std::vector<T, Allocator>;

template<class T, class Allocator = std::allocator<T>>
using List = std::list<T, Allocator>;

template<class T, class Allocator = std::allocator<T>>
using Queue = std::queue<T, Allocator>;

template<class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, T>>>
using Map = std::map<Key, T, Compare, Allocator>;

template<class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
using Set = std::set<Key, Compare, Allocator>;

template<class Key, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<Key>, class Allocator = std::allocator<Key>>
using HashSet = std::unordered_set<Key, Hash, KeyEqual, Allocator>;

template<class Key, class T, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<Key>, class Allocator = std::allocator< std::pair<const Key, T>>>
using HashMap = std::unordered_map<Key, T, Hash, KeyEqual, Allocator>;