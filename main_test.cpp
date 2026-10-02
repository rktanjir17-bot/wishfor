#include <iostream>
#include "OccasionFactory.h"
#include "ContentBlock.h"
#include "Page.h"
#include "Exceptions.h"

int main() {
    auto magazine = OccasionFactory::create("birthday", "Sarah");

    magazine->addPage(std::make_unique<Page>(
        std::make_unique<MessageBlock>("Tanjir", "Happy Birthday! Best wishes.")));

    magazine->addPage(std::make_unique<Page>(
        std::make_unique<WishBlock>("Ayesha", "May all your dreams come true!")));

    std::cout << "Status: " << magazine->status() << "\n";
    std::cout << "Page count: " << magazine->pageCount() << "\n\n";

    magazine->lock();
    std::cout << "After locking, status: " << magazine->status() << "\n";

    try {
        magazine->addPage(std::make_unique<Page>(
            std::make_unique<MessageBlock>("Rafiq", "Too late!")));
    } catch (const MagazineLockedException& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }

    std::cout << "\n--- Rendered Magazine ---\n";
    std::cout << magazine->render() << "\n";

    return 0;
}
