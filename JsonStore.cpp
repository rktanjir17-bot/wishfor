#include "JsonStore.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <sys/stat.h>
using json = nlohmann::json;
static const std::string DATA_DIR = "data";
static void ensureDataDir() {
    mkdir(DATA_DIR.c_str(), 0755);
}
void JsonStore::save(const Magazine& magazine) {
    ensureDataDir();
    json j;
    j["id"] = magazine.getId();
    j["recipientName"] = magazine.getRecipientName();
    j["occasion"] = magazine.getOccasion();
    j["isLocked"] = magazine.getIsLocked();
    json pagesArr = json::array();
    for (const auto& p : magazine.getPageRawData()) {
        json pageObj;
        pageObj["contributor"] = p.first;
        pageObj["html"] = p.second;
        pagesArr.push_back(pageObj);
    }
    j["pages"] = pagesArr;
    std::string path = DATA_DIR + "/" + magazine.getId() + ".json";
    std::ofstream file(path);
    file << j.dump(2);
    file.close();
}
bool JsonStore::exists(const std::string& id) {
    std::string path = DATA_DIR + "/" + id + ".json";
    std::ifstream file(path);
    return file.good();
}
std::shared_ptr<Magazine> JsonStore::load(const std::string& id) {
    std::string path = DATA_DIR + "/" + id + ".json";
    std::ifstream file(path);
    if (!file.good()) return nullptr;
    json j;
    file >> j;
    auto magazine = std::make_shared<Magazine>(
        j["id"].get<std::string>(),
        j["recipientName"].get<std::string>(),
        j["occasion"].get<std::string>(),
        j["isLocked"].get<bool>()
    );
    for (const auto& pageObj : j["pages"]) {
        magazine->addRawPage(
            pageObj["contributor"].get<std::string>(),
            pageObj["html"].get<std::string>()
        );
    }
    return magazine;
}
