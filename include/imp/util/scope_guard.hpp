#pragma once

#include <type_traits>

namespace imp {

// A scope_guard contains one member, on_exit. on_exit will be invoked when the
// scope_guard is destroyed (aka, when exiting the scope in which the
// scope_guard was declared.)
template <class F>
struct scope_guard {
    // on_exit will be invoked when the scope_guard is destroyed
    F on_exit;

    // ~scope_guard() will be noexcept if invoking on_exit() won't throw an
    // exception
    ~scope_guard() noexcept(noexcept(on_exit())) { on_exit(); }
};

template <class F>
scope_guard(F) -> scope_guard<typename std::unwrap_ref_decay<F>::type>;
} // namespace imp
