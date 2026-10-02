#ifndef MAGAZINE_H
#define MAGAZINE_H
#include <string>
#include <vector>
#include <memory>
#include "Page.h"
#include "MagazineState.h"
class Magazine {
    std::string id;
    std::string recipientName;
    std::string occasion;
    std::vector<std::unique_ptr<Page>> pages;
    std::unique_ptr<MagazineState> state;
    bool isLocked = false;
public:
    Magazine(const std::string& recipient, const std::string& occ);
    Magazine(const std::string& existingId, const std::string& recipient, const std::string& occ, bool locked);
    void addPage(std::unique_ptr<Page> page);
    void lock();
    void setState(std::unique_ptr<MagazineState> s);
    void internalAddPage(std::unique_ptr<Page> page) { pages.push_back(std::move(page)); }
    std::string render() const;
    std::string status() const;
    std::string getRecipientName() const { return recipientName; }
    std::string getOccasion() const { return occasion; }
    std::string getId() const { return id; }
    size_t pageCount() const { return pages.size(); }
    bool getIsLocked() const { return isLocked; }
    void setIsLocked(bool locked) { isLocked = locked; }
    std::vector<std::pair<std::string, std::string>> getPageRawData() const;
    void addRawPage(const std::string& contributor, const std::string& html);
};
#endif
