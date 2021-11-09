#include <noam/combinators.hpp>
#include <noam/parser.hpp>
#include <noam/result_types.hpp>

namespace imp {
using noam::parser;
using noam::result;
using noam::state_t;

inline std::string filter_comments(std::string_view code) {
    std::string result(code.size(), '\0');
    size_t i = 0;
    int depth = 0;
    while (code.size() > 0) {
        if (code.starts_with("--")) {
            size_t end = code.find('\n');
            if (end == code.npos) {
                return result;
            } else {
                // Add a newline to the result, and remove the comment from code
                result[i++] = '\n';
                code.remove_prefix(end + 1);
            }
        } else if (code.starts_with("{-")) {
            code.remove_prefix(2);
            int depth = 1;
            while (depth > 0) {
                if (code.starts_with("-}")) {
                    code.remove_prefix(2);
                    depth--;
                } else if (code.starts_with("{-")) {
                    code.remove_prefix(2);
                    depth++;
                } else if (code.size() > 0) {
                    // Read to the next character
                    code.remove_prefix(1);
                } else {
                    // Return whatever the result was
                    return result;
                }
            }
        } else {
            result[i++] = code[0];
            code.remove_prefix(1);
        }
    }
    // Shorten result to be however far we read
    result.resize(i);
    return result;
}
constexpr parser multiline_comment {
    [](std::string_view st) -> result<std::string_view> {
        if (st.starts_with("{-")) {
            size_t found = st.find("-}", 2);
            if (found == st.npos) {
                return {};
            }
            return {st.substr(found + 2), st.substr(2, found - 2)};
        } else {
            return {};
        }
    }};

constexpr parser singleline_comment {
    [](std::string_view st) -> result<std::string_view> {
        if (st.starts_with("--")) {
            size_t found = st.find("\n", 2);
            if (found == st.npos) {
                return {};
            }
            return {st.substr(found + 1), st.substr(2, found - 2)};
        } else {
            return {};
        }
    }};

constexpr auto comment = noam::either<std::string_view>(
    multiline_comment,
    singleline_comment);


} // namespace imp
