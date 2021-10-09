#include <noam/parser.hpp>
#include <noam/result_types.hpp>
#include <noam/combinators.hpp>

namespace imp {
using noam::parser;
using noam::result;
using noam::state_t;
constexpr parser multiline_comment {
    [](std::string_view st) -> result<std::string_view> {
        if (st.starts_with("{-")) {
            size_t found = st.find("-}", 2);
            return {st.substr(found + 2), st.substr(2, found)};
        } else {
            return {};
        }
    }
};

constexpr parser singleline_comment {
    [](std::string_view st) -> result<std::string_view> {
        if (st.starts_with("--")) {
            size_t found = st.find("\n", 2);
            return {st.substr(found + 2), st.substr(2, found)};
        } else {
            return {};
        }
    }
};

constexpr auto comment =
    noam::either<std::string_view> (multiline_comment, singleline_comment);
} // namespace imp