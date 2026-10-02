#include "ContentBlock.h"
#include <sstream>
int ContentBlock::nextId = 1;
ContentBlock::ContentBlock(const std::string& name) : contributorName(name) {
    id = nextId++;
}
MessageBlock::MessageBlock(const std::string& name, const std::string& msg)
    : ContentBlock(name), message(msg) {}
std::string MessageBlock::render() const {
    return "<div class='block message-block'><p><strong>" + contributorName +
           "</strong> says:</p><p>" + message + "</p></div>";
}
MemoryBlock::MemoryBlock(const std::string& name, const std::string& img, const std::string& cap)
    : ContentBlock(name), imagePath(img), caption(cap) {}
std::string MemoryBlock::render() const {
    return "<div class='block memory-block'><img src='" + imagePath +
           "' alt='memory' style='max-width:300px;'/><p>" + caption + "</p><p><em>- " + contributorName +
           "</em></p></div>";
}
WishBlock::WishBlock(const std::string& name, const std::string& w)
    : ContentBlock(name), wish(w) {}
std::string WishBlock::render() const {
    return "<div class='block wish-block'><p>&#127881; " + wish +
           "</p><p><em>- " + contributorName + "</em></p></div>";
}
std::string GalleryBlock::render() const {
    std::ostringstream out;
    out << "<div class='block gallery-block'>";
    out << "<div style='display:flex; flex-wrap:wrap; gap:8px;'>";
    for (const auto& path : imagePaths) {
        out << "<img src='" << path << "' alt='gallery' style='max-width:150px; max-height:150px; object-fit:cover;'/>";
    }
    out << "</div>";
    out << "<p>" << caption << "</p><p><em>- " << contributorName << "</em></p></div>";
    return out.str();
}
