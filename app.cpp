#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WPushButton.h>
#include <Wt/WLineEdit.h>
#include <Wt/WTextArea.h>
#include <fstream>
#include <cstdlib>
#include <memory>
#include <string>

class WishForApp : public Wt::WApplication {
public:
    WishForApp(const Wt::WEnvironment& env) : Wt::WApplication(env) {
        setTitle("WishFor - Storybook & Birthday Magazine");
        useStyleSheet("style.css");

        auto container = root();

        // 1. Hero Title Section
        auto hero = container->addWidget(std::make_unique<Wt::WContainerWidget>());
        hero->setStyleClass("hero-section");
        
        auto title = hero->addWidget(std::make_unique<Wt::WText>("<h1>Celebrate & Capture Memories</h1>"));
        title->setTextFormat(Wt::TextFormat::XHTML);
        
        auto subtitle = hero->addWidget(std::make_unique<Wt::WText>("<p>Create aesthetic photo magazines and download PDF for your loved ones.</p>"));
        subtitle->setTextFormat(Wt::TextFormat::XHTML);

        // 2. Dynamic Input Form Section
        auto formBox = container->addWidget(std::make_unique<Wt::WContainerWidget>());
        formBox->setStyleClass("wish-form-box");

        auto formTitle = formBox->addWidget(std::make_unique<Wt::WText>("<h3>Create Your Custom Wish & Magazine</h3>"));
        formTitle->setTextFormat(Wt::TextFormat::XHTML);

        // Author Name Input
        formBox->addWidget(std::make_unique<Wt::WText>("<label>Your Name / Author:</label>"));
        auto nameInput = formBox->addWidget(std::make_unique<Wt::WLineEdit>());
        nameInput->setPlaceholderText("e.g. Tanjir");

        // Picture Link Input
        formBox->addWidget(std::make_unique<Wt::WText>("<label>Picture URL:</label>"));
        auto imgInput = formBox->addWidget(std::make_unique<Wt::WLineEdit>());
        imgInput->setPlaceholderText("Paste image link (e.g. https://images.unsplash.com/photo-1534528741775-53994a69daeb)");

        // Wish Message Text Area
        formBox->addWidget(std::make_unique<Wt::WText>("<label>Your Wish Message:</label>"));
        auto msgInput = formBox->addWidget(std::make_unique<Wt::WTextArea>());
        msgInput->setPlaceholderText("Write your special wish here...");
        msgInput->setRows(3);

        // Action Buttons Bar
        auto btnContainer = formBox->addWidget(std::make_unique<Wt::WContainerWidget>());
        btnContainer->setStyleClass("action-bar");

        auto addBtn = btnContainer->addWidget(std::make_unique<Wt::WPushButton>("✨ Add to Magazine View"));
        addBtn->setStyleClass("btn-primary");

        auto pdfBtn = btnContainer->addWidget(std::make_unique<Wt::WPushButton>("📥 Generate & Download PDF"));
        pdfBtn->setStyleClass("btn-secondary");

        // 3. Magazine Live Preview Area
        auto magazineView = container->addWidget(std::make_unique<Wt::WContainerWidget>());
        magazineView->setStyleClass("magazine-viewport");

        // Dynamic State Storage
        auto currentName = std::make_shared<std::string>("Tanjir & Team");
        auto currentMsg = std::make_shared<std::string>("The best moments are those shared with the people we love—captured forever in pure elegance.");
        auto currentImg = std::make_shared<std::string>("https://images.unsplash.com/photo-1534528741775-53994a69daeb?q=80&w=800");

        // Render Initial Default Magazine View
        auto renderMagazineCard = [=](const std::string& name, const std::string& msg, const std::string& img) {
            std::string cardHtml = 
                "<div class='magazine-cover-spread'>"
                "  <div class='cover-portrait'>"
                "    <div class='cover-title-overlay'>VOGUE</div>"
                "    <img src='" + img + "' alt='Cover' />"
                "  </div>"
                "  <div class='spread-details'>"
                "    <h2 class='magazine-heading'>Birthday Magazine</h2>"
                "    <p class='magazine-subhead'>Special Limited Edition</p>"
                "    <hr class='divider' />"
                "    <div class='editorial-quote-block'>"
                "      <p class='quote-text'>\"" + msg + "\"</p>"
                "      <p class='quote-author'>— " + name + "</p>"
                "    </div>"
                "  </div>"
                "</div>";

            magazineView->clear();
            auto cardWidget = magazineView->addWidget(std::make_unique<Wt::WText>(cardHtml));
            cardWidget->setTextFormat(Wt::TextFormat::XHTML);
        };

        renderMagazineCard(*currentName, *currentMsg, *currentImg);

        // --- Event Listener 1: Add/Update Live Preview ---
        addBtn->clicked().connect([=]() {
            if (!nameInput->text().empty()) *currentName = nameInput->text().toUTF8();
            if (!msgInput->text().empty()) *currentMsg = msgInput->text().toUTF8();
            if (!imgInput->text().empty()) *currentImg = imgInput->text().toUTF8();

            renderMagazineCard(*currentName, *currentMsg, *currentImg);

            nameInput->setText("");
            imgInput->setText("");
            msgInput->setText("");
        });

        // --- Event Listener 2: Generate PDF Export ---
        pdfBtn->clicked().connect([=]() {
            std::ofstream htmlFile("magazine_export.html");
            htmlFile << "<!DOCTYPE html><html><head><style>"
                     << "body { font-family: 'Georgia', serif; padding: 40px; background: #faf8f5; color: #2c2c2c; }"
                     << ".container { max-width: 800px; margin: 0 auto; background: white; padding: 30px; border: 1px solid #e0e0e0; border-radius: 8px; }"
                     << "h1 { font-size: 28px; text-align: center; color: #1a1a1a; letter-spacing: 2px; border-bottom: 2px solid #7d9672; padding-bottom: 10px; }"
                     << ".content { margin-top: 30px; text-align: center; }"
                     << ".quote { font-style: italic; font-size: 20px; line-height: 1.6; color: #4a5568; margin: 20px 0; }"
                     << ".author { font-weight: bold; font-size: 16px; color: #7d9672; text-align: right; margin-top: 20px; }"
                     << "</style></head><body>"
                     << "<div class='container'>"
                     << "<h1>VOGUE : SPECIAL EDITION</h1>"
                     << "<div class='content'>"
                     << "<h2>Birthday Storybook Magazine</h2>"
                     << "<p class='quote'>\"" + *currentMsg + "\"</p>"
                     << "<p class='author'>— With love, " + *currentName + "</p>"
                     << "</div></div></body></html>";
            htmlFile.close();

            std::system("wkhtmltopdf magazine_export.html magazine.pdf || python3 -c \"import pdfkit; pdfkit.from_file('magazine_export.html', 'magazine.pdf')\" 2>/dev/null || true");

            pdfBtn->setText("✅ PDF Exported!");
            doJavaScript("window.open('/magazine.pdf', '_blank');");
        });
    }
};

int main(int argc, char **argv) {
    return Wt::WRun(argc, argv, [](const Wt::WEnvironment& env) {
        return std::make_unique<WishForApp>(env);
    });
}