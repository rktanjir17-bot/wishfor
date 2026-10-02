#ifndef OCCASION_FACTORY_H
#define OCCASION_FACTORY_H

#include <memory>
#include <string>
#include "Magazine.h"

// Factory pattern: creates a Magazine pre-configured for an occasion
class OccasionFactory {
public:
    static std::unique_ptr<Magazine> create(const std::string& type, const std::string& recipient);
};

#endif
