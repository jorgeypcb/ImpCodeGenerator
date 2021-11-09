#pragma once

namespace imp {
// Allows you to create an overload set from a bunch of lambdas
template <class... F>
struct overload_set : F... {
    using F::operator()...;
};
template <class... F>
overload_set(F...) -> overload_set<F...>;
} // namespace imp
