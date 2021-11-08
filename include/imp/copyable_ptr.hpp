#pragma once
#include <type_traits>
#include <utility>

namespace imp {
template <class T>
class copyable_ptr {
    T* pointer = nullptr;
    constexpr explicit copyable_ptr(T* pointer) noexcept
      : pointer(pointer) {}

   public:
    static constexpr copyable_ptr aquire(T* pointer) {
        return copyable_ptr(pointer);
    }
    constexpr copyable_ptr() = default;
    constexpr copyable_ptr(T&& value)
      : pointer(new T(static_cast<T&&>(value))) {}
    constexpr copyable_ptr(T const& value)
      : pointer(new T(value)) {}
    copyable_ptr(copyable_ptr&& c) noexcept
      : pointer(std::exchange(c.pointer, nullptr)) {}
    copyable_ptr(copyable_ptr const& c)
      : pointer(new T(*c.pointer)) {}

    constexpr auto& operator=(copyable_ptr other) noexcept {
        std::swap(pointer, other.pointer);
        return *this;
    }

    constexpr T* operator->() noexcept { return pointer; }
    constexpr T const* operator->() const noexcept { return pointer; }

    constexpr T& operator*() & noexcept { return *pointer; }
    constexpr T const& operator*() const& noexcept { return *pointer; }
    constexpr T&& operator*() && noexcept { return static_cast<T&&>(*pointer); }
    constexpr T const&& operator*() const&& noexcept {
        return static_cast<T const&&>(*pointer);
    }

    constexpr operator bool() const noexcept {
        return bool(pointer);
    }

    constexpr ~copyable_ptr() {
        delete pointer;
    }
};
} // namespace imp
