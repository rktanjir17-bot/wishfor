#include "MagazineState.h"
#include "Magazine.h"
#include "Exceptions.h"

void CollectingState::addPage(Magazine& m, std::unique_ptr<Page> page) {
    m.internalAddPage(std::move(page));
}

void CollectingState::lock(Magazine& m) {
    m.setState(std::make_unique<LockedState>());
}

void LockedState::addPage(Magazine& m, std::unique_ptr<Page> page) {
    throw MagazineLockedException();
}

void LockedState::lock(Magazine& m) {
    // already locked, no-op
}
