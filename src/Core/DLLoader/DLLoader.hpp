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

    template <typename T>
    class DLLoader {
     private:
        void* _handle;
        std::string _libPath;

     public:
        explicit DLLoader(const std::string& libPath) : _handle(nullptr)
             _libPath(libPath) {}
        ~DLLoader() {
            if (_handle)
                dlclose(_handle);
        }
        T* getInstance(const std::string& entryPointName = "entryPoint") {
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
    };

#endif  // SRC_CORE_DLLOADER_DLLOADER_HPP_
