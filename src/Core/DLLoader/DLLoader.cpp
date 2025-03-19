// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** DLLoader
*/
#include "DLLoader/DLLoader.hpp"

template <typename T>
DLLoader<T>::~DLLoader()
{
    if (_handle)
        dlclose(_handle);
}

template <typename T>
T* DLLoader<T>::getInstance(const std::string& entryPointName)
{
    if (!_handle) {
        _handle = dlopen(_libPath.c_str(), RTLD_LAZY);
        if (!_handle) {
            std::cerr << "Error loading library "
                << _libPath << ": " << dlerror() << std::endl;
            return nullptr;
        }
    }
    dlerror();
    using creator_t = T* (*)();
    creator_t creator = reinterpret_cast<creator_t>(dlsym(_handle,
        entryPointName.c_str()));
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Error finding entry point "
            << entryPointName << ": " << dlsym_error << std::endl;
        return nullptr;
    }
    return creator();
}
