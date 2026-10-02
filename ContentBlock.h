#ifndef CONTENT_BLOCK_H
#define CONTENT_BLOCK_H
#include <vector>

#include <string>
#include <memory>

// Abstract base class — demonstrates Abstraction + is the base for Polymorphism
class ContentBlock {
protected:
    static int nextId;         // static member — shared across all ContentBlocks
    int id;
    std::string contributorName;

public:
    ContentBlock(const std::string& name);
    virtual ~ContentBlock() = default;

    virtual std::string render() const = 0;   // pure virtual -> polymorphism
    virtual std::string typeName() const = 0;

    int getId() const { return id; }
    std::string getContributorName() const { return contributorName; }
};

// Derived class: a simple text message
class MessageBlock : public ContentBlock {
    std::string message;

public:
    MessageBlock(const std::string& name, const std::string& msg);
    std::string render() const override;
    std::string typeName() const override { return "Message"; }
};

// Derived class: a photo + caption memory
class MemoryBlock : public ContentBlock {
    std::string imagePath;
    std::string caption;

public:
    MemoryBlock(const std::string& name, const std::string& img, const std::string& cap);
    std::string render() const override;
    std::string typeName() const override { return "Memory"; }
};

// Derived class: a short wish/greeting
class WishBlock : public ContentBlock {
    std::string wish;

public:
    WishBlock(const std::string& name, const std::string& w);
    std::string render() const override;
    std::string typeName() const override { return "Wish"; }
};


class RawHtmlBlock : public ContentBlock {
    std::string html;
public:
    RawHtmlBlock(const std::string& name, const std::string& h) : ContentBlock(name), html(h) {}
    std::string render() const override { return html; }
    std::string typeName() const override { return "Raw"; }
};

class GalleryBlock : public ContentBlock {
    std::vector<std::string> imagePaths;
    std::string caption;
public:
    GalleryBlock(const std::string& name, const std::vector<std::string>& imgs, const std::string& cap)
        : ContentBlock(name), imagePaths(imgs), caption(cap) {}
    std::string render() const override;
    std::string typeName() const override { return "Gallery"; }
};
#endif
