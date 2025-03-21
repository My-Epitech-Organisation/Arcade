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
        explicit DLLoader(const std::string& libPath) : _handle(nullptr),
            _libPath(libPath) {}
        ~DLLoader();
        T* getInstance(const std::string& entryPointName = "entryPoint");
    };

#endif  // SRC_CORE_DLLOADER_DLLOADER_HPP_
