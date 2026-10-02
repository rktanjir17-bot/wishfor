#ifndef MAGAZINE_STATE_H
#define MAGAZINE_STATE_H

#include <memory>
#include "Page.h"

class Magazine;  // forward declaration

// Abstract State — defines what actions are allowed in each state
class MagazineState {
public:
    virtual ~MagazineState() = default;
    virtual void addPage(Magazine& m, std::unique_ptr<Page> page) = 0;
    virtual void lock(Magazine& m) = 0;
    virtual std::string statusName() const = 0;
};

// Concrete State: pages can be added, magazine can be locked
class CollectingState : public MagazineState {
public:
    void addPage(Magazine& m, std::unique_ptr<Page> page) override;
    void lock(Magazine& m) override;
    std::string statusName() const override { return "Collecting"; }
};

// Concrete State: no more pages allowed
class LockedState : public MagazineState {
public:
    void addPage(Magazine& m, std::unique_ptr<Page> page) override;
    void lock(Magazine& m) override;
    std::string statusName() const override { return "Locked"; }
};

#endif
