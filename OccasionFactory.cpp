#include "OccasionFactory.h"

std::unique_ptr<Magazine> OccasionFactory::create(const std::string& type, const std::string& recipient) {
    std::string occasionLabel;

    if (type == "birthday") {
        occasionLabel = "Happy Birthday!";
    } else if (type == "farewell") {
        occasionLabel = "Farewell & Best Wishes";
    } else if (type == "anniversary") {
        occasionLabel = "Happy Anniversary!";
    } else {
        occasionLabel = "A Special Occasion";
    }

    return std::make_unique<Magazine>(recipient, occasionLabel);
}
