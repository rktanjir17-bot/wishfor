#include "ContentBlock.h"
#include <sstream>

int ContentBlock::nextId = 1;

ContentBlock::ContentBlock(const std::string& name) : contributorName(name) {
    id = nextId++;
}

MessageBlock::MessageBlock(const std::string& name, const std::string& msg)
    : ContentBlock(name), message(msg) {}

std::string MessageBlock::render() const {
    return "<div class='editorial-quote-block'>"
           "<p class='quote-text'>\"" + message + "\"</p>"
           "<p class='quote-author'>— " + contributorName + "</p>"
           "</div>";
}

MemoryBlock::MemoryBlock(const std::string& name, const std::string& img, const std::string& cap)
    : ContentBlock(name), imagePath(img), caption(cap) {}

std::string MemoryBlock::render() const {
    return "<div class='polaroid-card'>"
           "<img src='" + imagePath + "' alt='Memory' />"
           "<p class='polaroid-caption'>" + caption + "</p>"
           "<small style='display:block; text-align:right; color:#888;'>By " + contributorName + "</small>"
           "</div>";
}

WishBlock::WishBlock(const std::string& name, const std::string& w)
    : ContentBlock(name), wish(w) {}

std::string WishBlock::render() const {
    return "<div class='wish-card-storybook'>"
           "<span style='font-size:1.5rem;'>✨</span>"
           "<p class='wish-content'>" + wish + "</p>"
           "<p class='wish-from'>With love, " + contributorName + "</p>"
           "</div>";
}

std::string GalleryBlock::render() const {
    std::ostringstream out;
    out << "<div class='memory-grid'>";
    for (const auto& path : imagePaths) {
        out << "<div class='polaroid-card'><img src='" << path << "' alt='Gallery Image'/></div>";
    }
    out << "</div>";
    if (!caption.empty()) {
        out << "<p class='polaroid-caption' style='grid-column: 1/-1;'>" << caption << " (by " << contributorName << ")</p>";
    }
    return out.str();
}