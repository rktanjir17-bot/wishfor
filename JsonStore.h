#ifndef JSON_STORE_H
#define JSON_STORE_H

#include <string>
#include <memory>
#include "Magazine.h"

class JsonStore {
public:
    static void save(const Magazine& magazine);
    static std::shared_ptr<Magazine> load(const std::string& id);
    static bool exists(const std::string& id);
};

#endif
