#include "Page.h"

Page::Page(std::unique_ptr<ContentBlock> b) : block(std::move(b)) {}

std::string Page::render() const {
    return block->render();   // polymorphism in action
}

std::string Page::contributorName() const {
    return block->getContributorName();
}
