//
// Created by rostislove on 01/08/22.
//

#ifndef CUTIEDB_CUTE_LITTLE_HPP
#define CUTIEDB_CUTE_LITTLE_HPP

#include <vector>
#include <string>
#include <map>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "nlohmann/json.hpp"

class cute_little {

public:
    struct error : public std::runtime_error {
        explicit error(const char* data) : std::runtime_error(data) { }
    };

    struct collection_exists_error : public error {
        explicit collection_exists_error(const char* data) : error(data) { }
    };

    struct collection_not_found_error : public error {
        explicit collection_not_found_error(const char* data) : error(data) { }
    };

    typedef nlohmann::json object;

    class collection {
    private:
        std::vector<cute_little::object> records;

    public:
        cute_little::object& add_object(const object& object) {
            records.push_back(object);
            return records[records.size() - 1];
        }

        cute_little::object& get_object(size_t index) {
            return records[index];
        }

        std::vector<cute_little::object>& DBG_get_records() {
            return records;
        }
    };

private:
    std::map<std::string, collection> collections;

public:
    cute_little() = default;

    collection& create_collection(const std::string& name) {
        if (collections.count(name) > 0) {
            throw cute_little::collection_exists_error("Collection already exists.");
        }
        collections.emplace(name, collection());
        return collections[name];
    }

    void remove_collection(const std::string& name) {
        if (collections.count(name) == 0) {
            throw cute_little::collection_not_found_error("Collection does not exist.");
        }
        collections.erase(name);
    }

    collection& get_collection(const std::string& name) {
        return collections[name];
    }

    void DBG_print() {
        for (auto& pair : collections) {
            std::cout << "Collection: " << pair.first << std::endl;
            for (const auto& object : pair.second.DBG_get_records()) {
                std::cout << "object: " << to_string(object) << std::endl;
            }
        }
    }

};


#endif //CUTIEDB_CUTE_LITTLE_HPP
