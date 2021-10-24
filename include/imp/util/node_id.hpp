#pragma once

#include <fmt/format.h>
#include <rva/variant.hpp>

namespace imp {
struct node_id {
    size_t type_size;
    size_t node_location;
    template <class T>
    constexpr node_id(T const& node) noexcept
      : type_size(sizeof(T))
      , node_location((size_t)&node) {}
};

template <class T>
node_id get_id(const T& anything) {
    // This creates a unique identifier by combining the location of the
    // variable in memory with the size of the type. This ensures that types
    // that contain other types produce a unique identifier.
    return node_id(anything);
}
template <class... T>
node_id get_id(const rva::variant<T...>& anything) {
    auto visitor = [](auto const& x) { return get_id(x); };
    return rva::visit(visitor, anything);
}
} // namespace imp

// Having a formatter is like providing an overload of ostream. This will allow
// you to use imp::node_id in fmt expressions
template <>
struct fmt::formatter<imp::node_id> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    template <typename FormatContext>
    constexpr auto format(imp::node_id n, FormatContext& ctx) {
        return fmt::format_to(
            ctx.out(),
            "s{:02x}x{:x}",
            n.type_size,
            n.node_location);
    }
};
