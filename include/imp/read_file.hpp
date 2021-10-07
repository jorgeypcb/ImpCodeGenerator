#pragma once

#include <filesystem>
#include <system_error>

#include <stdexcept>
#include <string>
#include <system_error>

#include <imp/util/scope_guard.hpp>

namespace imp {
namespace fs = std::filesystem;

std::string read_file(const fs::path& path) {
    FILE* input_file = fopen(path.c_str(), "r");

    // When on_exit gets destroyed, it will close the input file
    scope_guard on_exit {[=] {
        if (input_file)
            fclose(input_file);
    }};

    // If input_file is nullptr, we need to throw an exception
    if (input_file == nullptr) {
        throw std::system_error(errno, std::system_category(), path);
    }

    // Get the size of the file
    size_t file_size = fs::file_size(path);

    std::string res(file_size, '\0');
    ssize_t bytes_read = fread(res.data(), 1, file_size, input_file);

    if (bytes_read < res.size()) {
        if (int at_eof = feof(input_file)) {
            clearerr(input_file);
            throw std::runtime_error(
                "Unable to read all bytes in" + path.string()
                + ". EOF reached early.");
        }
        if (int error_code = ferror(input_file)) {
            clearerr(input_file);
            throw std::system_error(
                error_code,
                std::system_category(),
                "Error when reading " + path.string());
        }
    }

    return res;
}
} // namespace imp
