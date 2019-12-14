#pragma once

#include <iostream>
#include <string>
#include <map>

namespace LORE {
    template <class T>
    class Library {
        public:
            Library(): m__objects() {}

            ~Library() {}

            bool has(std::string path) {
                return m__objects.find(path) != m__objects.end();
            }

            void add(std::string path, T* object) {
                std::string path_ = path;
                int i = 1;
                while(has(path_)) {
                    path_ = path + "_" + std::to_string(i);
                    i++;
                }
                std::cout << "Resulting path: " << path_ << std::endl;
                m__objects.insert(std::pair<std::string, T*>(path_, object));
            }

            T* get(std::string path) {
                if(has(path))
                    return m__objects.at(path);
                else
                    return nullptr;
            }

            void list() {
                for(auto it: m__objects)
                    std::cout << "[Library]: " << it.first << std::endl;
            }

            void flush() {
                for(auto it: m__objects)
                    delete(it.second);
                m__objects.clear();
            }

        private:
            std::map<std::string, T*> m__objects;
    };
}
