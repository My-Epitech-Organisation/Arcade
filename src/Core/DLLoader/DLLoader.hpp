// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** DLoader class
*/

#ifndef SRC_CORE_DLLOADER_DLLOADER_HPP_
    #define SRC_CORE_DLLOADER_DLLOADER_HPP_
    #include <dlfcn.h>
    #include <iostream>
    #include <string>
    #include <memory>
    #include "Shared/Exceptions/LibraryLoadException.hpp"

template <typename T>
class DLLoader {
 private:
    void* _handle;
    std::string _libPath;
    static const char* DEFAULT_ENTRY_POINT;

 public:
    explicit DLLoader(const std::string& libPath) : _handle(nullptr),
        _libPath(libPath) {}
    ~DLLoader();
    T* getInstance(const std::string& entryPointName);
    operator std::unique_ptr<T>() {
        return std::unique_ptr<T>(getInstance(DEFAULT_ENTRY_POINT));
    }
    void setLibPath(const std::string& libPath);
    std::unique_ptr<T> getInstanceUPtr(
        const std::string& entryPointName);
    const std::string& getLibPath() const {
        return _libPath;
    }
};

template <typename T>
const char* DLLoader<T>::DEFAULT_ENTRY_POINT = "entryPoint";

#include "DLLoader.tpp"
#endif  // SRC_CORE_DLLOADER_DLLOADER_HPP_
