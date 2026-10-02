#ifndef PAGE_H
#define PAGE_H

#include <memory>
#include "ContentBlock.h"

// Composition: a Page HAS-A ContentBlock
class Page {
    std::unique_ptr<ContentBlock> block;

public:
    Page(std::unique_ptr<ContentBlock> b);
    std::string render() const;
    std::string contributorName() const;
};

#endif
