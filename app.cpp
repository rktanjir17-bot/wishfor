#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WPushButton.h>

class WishForApp : public Wt::WApplication {
public:
    WishForApp(const Wt::WEnvironment& env) : Wt::WApplication(env) {
        setTitle("WishFor - Storybook & Birthday Magazine");
        useStyleSheet("style.css");

        auto container = root();

        // 1. Hero Section
        auto hero = container->addWidget(std::make_unique<Wt::WContainerWidget>());
        hero->setStyleClass("hero-section");
        
        // TextFormat::XHTML ব্যবহার করায় HTML ট্যাগগুলো রেন্ডার হবে
        auto title = hero->addWidget(std::make_unique<Wt::WText>(
            "<h1>Celebrate & Capture Memories</h1>", 
            Wt::TextFormat::XHTML
        ));
        title->setStyleClass("hero-title");
        
        auto subtitle = hero->addWidget(std::make_unique<Wt::WText>(
            "<p>Create aesthetic, editorial-style photo magazines for your loved ones in seconds.</p>", 
            Wt::TextFormat::XHTML
        ));
        subtitle->setStyleClass("hero-subtitle");

        // 2. Action Bar Buttons
        auto actionBar = container->addWidget(std::make_unique<Wt::WContainerWidget>());
        actionBar->setStyleClass("action-bar");

        auto createBtn = actionBar->addWidget(std::make_unique<Wt::WPushButton>("✨ Create New Magazine"));
        createBtn->setStyleClass("btn-primary");

        auto shareBtn = actionBar->addWidget(std::make_unique<Wt::WPushButton>("🔗 Share Link"));
        shareBtn->setStyleClass("btn-secondary");

        auto pdfBtn = actionBar->addWidget(std::make_unique<Wt::WPushButton>("📥 Download PDF"));
        pdfBtn->setStyleClass("btn-secondary");

        // 3. Magazine Cover Spread
        auto magazineView = container->addWidget(std::make_unique<Wt::WContainerWidget>());
        magazineView->setStyleClass("magazine-viewport");

        std::string coverHtml = 
            "<div class='magazine-cover-spread'>"
            "  <div class='cover-portrait'>"
            "    <div class='cover-title-overlay'>VOGUE</div>"
            "    <img src='https://images.unsplash.com/photo-1534528741775-53994a69daeb?q=80&w=800' alt='Cover' />"
            "  </div>"
            "  <div class='spread-details'>"
            "    <h2 class='magazine-heading'>Birthday Magazine</h2>"
            "    <p class='magazine-subhead'>Special Limited Edition</p>"
            "    <hr class='divider' />"
            "    <div class='editorial-quote-block'>"
            "      <p class='quote-text'>\"The best moments are those shared with the people we love—captured forever in pure elegance.\"</p>"
            "      <p class='quote-author'>— Tanjir & Team</p>"
            "    </div>"
            "  </div>"
            "</div>";

        magazineView->addWidget(std::make_unique<Wt::WText>(
            coverHtml, 
            Wt::TextFormat::XHTML
        ));

        // Interactive Actions
        shareBtn->clicked().connect([=]() {
            shareBtn->setText("✅ Link Copied!");
        });

        pdfBtn->clicked().connect([=]() {
            pdfBtn->setText("⏳ Generating PDF...");
        });
    }
};

int main(int argc, char **argv) {
    return Wt::WRun(argc, argv, [](const Wt::WEnvironment& env) {
        return std::make_unique<WishForApp>(env);
    });
}