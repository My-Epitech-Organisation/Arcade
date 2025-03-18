/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** DLLoader
*/

#ifndef DLLOADER_HPP_
    #define DLLOADER_HPP_
    #include <iostream>
    #include <string>
    #include <dlfcn.h>

    template <typename T>
    class DLLoader {
        private:
            void* _handle;
            std::string _libPath;
        public:
            DLLoader(const std::string& libPath) : _handle(nullptr), _libPath(libPath) {};
            ~DLLoader() {
                if (_handle)
                    dlclose(_handle);
            }
            T* getInstance(const std::string& entryPointName = "entryPoint") {
                if (!_handle) {
                    _handle = dlopen(_libPath.c_str(), RTLD_LAZY);
                    if (!_handle) {
                        std::cerr << "Error loading library " << _libPath << ": " << dlerror() << std::endl;
                        return nullptr;
                    }
                }
                dlerror();
                using creator_t = T* (*)();
                creator_t creator = reinterpret_cast<creator_t>(dlsym(_handle, entryPointName.c_str()));
                const char* dlsym_error = dlerror();
                if (dlsym_error) {
                    std::cerr << "Error finding entry point " << entryPointName << ": " << dlsym_error << std::endl;
                    return nullptr;
                }
                return creator();
            }
    };

#endif /* !DLLOADER_HPP_ */