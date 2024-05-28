#if !defined GAIA_FILESYSTEM_HELPERS_HEADER_INCLUDED
#define GAIA_FILESYSTEM_HELPERS_HEADER_INCLUDED
#pragma once

#include <filesystem>
#include <iostream>
#include <optional>

namespace gaia {

namespace fs = std::filesystem;

std::optional<fs::path> find_file_in_directory(const fs::path& directory, const std::string& filename) {
    try {
        for (const auto& entry : fs::recursive_directory_iterator(directory)) {
            if (entry.path().filename() == filename) {
                return entry.path();
            }
        }
    } catch (const fs::filesystem_error& e) {
        if (e.code() == std::errc::permission_denied) {
            std::cerr << "Permission denied in directory: " << directory << std::endl;
            return std::nullopt;
        } else {
            std::cerr << "Error accessing directory: " << directory << ", Error: " << e.what() << std::endl;
        }
    }
    return std::nullopt;
}

std::optional<fs::path> find_file_recursively(const std::string& filename) {
    fs::path current_dir = fs::current_path();

    while (true) {
        if (auto result = find_file_in_directory(current_dir, filename); result) {
            return result;
        }

        fs::path parent_dir = current_dir.parent_path();

        // Stop if the parent directory is the root directory
        if (parent_dir == current_dir.root_path()) {
            return std::nullopt;
        }

        // Check if we have permission to access the parent directory
        std::error_code ec;
        fs::directory_entry parent_dir_entry(parent_dir, ec);
        if (ec) {
            if (ec == std::errc::permission_denied) {
                std::cerr << "Permission denied to access parent directory: " << parent_dir << std::endl;
                return std::nullopt;
            }
            std::cerr << "Error accessing parent directory: " << parent_dir << ", Error: " << ec.message() << std::endl;
            return std::nullopt;
        }

        // Move up one directory level
        current_dir = parent_dir;
    }
}

} // end namespace gaia

#endif