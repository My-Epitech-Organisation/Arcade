/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** DLoader implementation
*/

#include <stdexcept>

template <typename T>
DLLoader<T>::~DLLoader() {
    if (_handle != nullptr) {
        dlclose(_handle);
        _handle = nullptr;
    }
}

template <typename T>
T* DLLoader<T>::getInstance(const std::string& entryPointName) {
    if (_handle == nullptr) {
        _handle = dlopen(_libPath.c_str(), RTLD_LAZY);
        if (_handle == nullptr) {
            throw Arcade::LibraryLoadException("Error loading library: " + std::string(dlerror()));
        }
    }
    dlerror();
    using EntryPointFunc = T *(*)();
    EntryPointFunc entryPoint = reinterpret_cast<EntryPointFunc>(
        dlsym(_handle, entryPointName.c_str()));
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        throw Arcade::LibraryLoadException("Error finding entry point " + _libPath + ": " + dlsym_error);
    }
    return entryPoint();
}

template <typename T>
void DLLoader<T>::setLibPath(const std::string& libPath) {
    if (_handle != nullptr) {
        if (dlclose(_handle) != 0) {
            throw Arcade::LibraryLoadException("Error closing library: " + std::string(dlerror()));
        }
        _handle = nullptr;
    }
    _libPath = libPath;
}

template <typename T>
std::unique_ptr<T> DLLoader<T>::getInstanceUPtr(const std::string& entryPointName) {
    return std::unique_ptr<T>(getInstance(entryPointName));
}