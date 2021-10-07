#include <filesystem>
#include <system_error>

#include <stdexcept>
#include <string>
#include <system_error>

namespace imp {
namespace fs = std::filesystem;

std::string read_file(const std::string& path) {
    FILE* input_file = fopen(path.c_str(), "r");

    // If input_file is nullptr, we need to throw an exception
    if (input_file == nullptr) {
        throw std::system_error(errno, std::system_category(),path);
    }

    // Get the size of the file
    size_t file_size = fs::file_size(path);

    std::string res(file_size, '\0');
    ssize_t bytes_read = fread(res.data(), 1, file_size, input_file);

    if (bytes_read < res.size()) {
        if (int at_eof = feof(input_file)) {
            clearerr(input_file);
            throw std::runtime_error(
                "Unable to read all bytes in" + path + ". EOF reached early.");
        }
        if (int error_code = ferror(input_file)) {
            clearerr(input_file);
            throw std::system_error(
                error_code,
                std::system_category(),
                "Error when reading " + path);
        }
    }
    fclose(input_file);

    return res;
}
} // namespace imp
