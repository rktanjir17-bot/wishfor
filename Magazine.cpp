#include "Magazine.h"
#include "ContentBlock.h"
#include <random>
#include <sstream>
static std::string generateId() {
    static const char chars[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, sizeof(chars) - 2);
    std::string id;
    for (int i = 0; i < 8; i++) id += chars[dist(gen)];
    return id;
}
Magazine::Magazine(const std::string& recipient, const std::string& occ)
    : id(generateId()), recipientName(recipient), occasion(occ), state(std::make_unique<CollectingState>()) {}
Magazine::Magazine(const std::string& existingId, const std::string& recipient, const std::string& occ, bool locked)
    : id(existingId), recipientName(recipient), occasion(occ), isLocked(locked) {
    if (locked) state = std::make_unique<LockedState>();
    else state = std::make_unique<CollectingState>();
}
void Magazine::addPage(std::unique_ptr<Page> page) {
    state->addPage(*this, std::move(page));
}
void Magazine::lock() {
    state->lock(*this);
    isLocked = true;
}
void Magazine::setState(std::unique_ptr<MagazineState> s) {
    state = std::move(s);
}
std::string Magazine::render() const {
    std::ostringstream out;
    out << "<div class='magazine'>";
    out << "<h1>For " << recipientName << "</h1>";
    out << "<h3>" << occasion << "</h3>";
    for (const auto& page : pages) out << page->render();
    out << "</div>";
    return out.str();
}
std::string Magazine::status() const {
    return state->statusName();
}
std::vector<std::pair<std::string, std::string>> Magazine::getPageRawData() const {
    std::vector<std::pair<std::string, std::string>> data;
    for (const auto& page : pages) {
        data.push_back({page->contributorName(), page->render()});
    }
    return data;
}
void Magazine::addRawPage(const std::string& contributor, const std::string& html) {
    auto block = std::make_unique<RawHtmlBlock>(contributor, html);
    pages.push_back(std::make_unique<Page>(std::move(block)));
}
