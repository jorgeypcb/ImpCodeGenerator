#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <system_error>

// adapted from https://www.delftstack.com/howto/cpp/read-file-into-string-cpp/
namespace imp {
std::string read_file(const std::string& path) {
    struct stat sb {};
    std::string res;

    FILE* input_file = fopen(path.c_str(), "r");
    if (input_file == nullptr) {
        std::system_error(errno);
    }

    stat(path.c_str(), &sb);
    res.resize(sb.st_size);
    fread(res.data(), sb.st_size, 1, input_file);
    fclose(input_file);

    return res;
}
} // namespace imp
