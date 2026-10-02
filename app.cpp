#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WTextArea.h>
#include <Wt/WComboBox.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WBreak.h>
#include <Wt/WFileUpload.h>
#include <Wt/WTimer.h>
#include <map>
#include <vector>
#include <ctime>
#include <fstream>

static bool copyFile(const std::string& src, const std::string& dst) {
    std::ifstream in(src, std::ios::binary);
    std::ofstream out(dst, std::ios::binary);
    if (!in || !out) return false;
    out << in.rdbuf();
    return true;
}
#include "OccasionFactory.h"
#include "Magazine.h"
#include "ContentBlock.h"
#include "Page.h"
#include "Exceptions.h"
#include "JsonStore.h"
static std::map<std::string, std::shared_ptr<Magazine>> magazineStore;
static std::shared_ptr<Magazine> findMagazine(const std::string& id) {
    auto it = magazineStore.find(id);
    if (it != magazineStore.end()) return it->second;
    if (JsonStore::exists(id)) {
        auto m = JsonStore::load(id);
        magazineStore[id] = m;
        return m;
    }
    return nullptr;
}
class WishForApp : public Wt::WApplication {
public:
    WishForApp(const Wt::WEnvironment& env) : Wt::WApplication(env) {
        setTitle("WishFor");
        useStyleSheet("style.css");
        root()->addWidget(std::make_unique<Wt::WText>("<h1>WishFor</h1>"));
        buildCreateSection();
        divider();
        buildAddPageSection();
        divider();
        buildGallerySection();
        divider();
        buildLockSection();
        divider();
        buildRevealSection();
    }
private:
    void divider() {
        root()->addWidget(std::make_unique<Wt::WBreak>());
        root()->addWidget(std::make_unique<Wt::WText>("<hr/>"));
    }
    void buildCreateSection() {
        root()->addWidget(std::make_unique<Wt::WText>("<h3>Create a Magazine</h3>"));
        root()->addWidget(std::make_unique<Wt::WText>("Recipient's Name: "));
        auto nameInput = root()->addWidget(std::make_unique<Wt::WLineEdit>());
        nameInput->setPlaceholderText("e.g. Sarah");
        root()->addWidget(std::make_unique<Wt::WBreak>());
        root()->addWidget(std::make_unique<Wt::WText>("Occasion: "));
        auto occasionBox = root()->addWidget(std::make_unique<Wt::WComboBox>());
        occasionBox->addItem("birthday");
        occasionBox->addItem("farewell");
        occasionBox->addItem("anniversary");
        root()->addWidget(std::make_unique<Wt::WBreak>());
        root()->addWidget(std::make_unique<Wt::WBreak>());
        auto createBtn = root()->addWidget(std::make_unique<Wt::WPushButton>("Create Magazine"));
        auto resultText = root()->addWidget(std::make_unique<Wt::WText>());
        createBtn->clicked().connect([=] {
            std::string recipient = nameInput->text().toUTF8();
            std::string occasion = occasionBox->currentText().toUTF8();
            if (recipient.empty()) {
                resultText->setText("<p style='color:red;'>Please enter a recipient name.</p>");
                return;
            }
            auto magazine = OccasionFactory::create(occasion, recipient);
            std::string id = magazine->getId();
            std::shared_ptr<Magazine> shared(magazine.release());
            magazineStore[id] = shared;
            JsonStore::save(*shared);
            resultText->setText(
                "<p style='color:green;'>Magazine created! Share this ID with contributors: <b>"
                + id + "</b></p>");
        });
    }
    void buildAddPageSection() {
        root()->addWidget(std::make_unique<Wt::WText>("<h3>Add a Page to a Magazine</h3>"));
        root()->addWidget(std::make_unique<Wt::WText>("Magazine ID: "));
        auto idInput = root()->addWidget(std::make_unique<Wt::WLineEdit>());
        idInput->setPlaceholderText("e.g. lbobm571");
        root()->addWidget(std::make_unique<Wt::WBreak>());
        root()->addWidget(std::make_unique<Wt::WText>("Your Name: "));
        auto nameInput = root()->addWidget(std::make_unique<Wt::WLineEdit>());
        nameInput->setPlaceholderText("e.g. Tanjir");
        root()->addWidget(std::make_unique<Wt::WBreak>());
        root()->addWidget(std::make_unique<Wt::WText>("Block Type: "));
        auto typeBox = root()->addWidget(std::make_unique<Wt::WComboBox>());
        typeBox->addItem("Message");
        typeBox->addItem("Wish");
        root()->addWidget(std::make_unique<Wt::WBreak>());
        root()->addWidget(std::make_unique<Wt::WText>("Your Text: "));
        root()->addWidget(std::make_unique<Wt::WBreak>());
        auto textArea = root()->addWidget(std::make_unique<Wt::WTextArea>());
        textArea->setColumns(50);
        textArea->setRows(4);
        root()->addWidget(std::make_unique<Wt::WBreak>());
        auto addBtn = root()->addWidget(std::make_unique<Wt::WPushButton>("Add Page"));
        auto addResultText = root()->addWidget(std::make_unique<Wt::WText>());
        addBtn->clicked().connect([=] {
            std::string magId = idInput->text().toUTF8();
            std::string contributor = nameInput->text().toUTF8();
            std::string type = typeBox->currentText().toUTF8();
            std::string text = textArea->text().toUTF8();
            auto mag = findMagazine(magId);
            if (!mag) {
                addResultText->setText("<p style='color:red;'>Magazine ID not found.</p>");
                return;
            }
            if (contributor.empty() || text.empty()) {
                addResultText->setText("<p style='color:red;'>Please fill in your name and text.</p>");
                return;
            }
            std::unique_ptr<ContentBlock> block;
            if (type == "Message") block = std::make_unique<MessageBlock>(contributor, text);
            else block = std::make_unique<WishBlock>(contributor, text);
            try {
                mag->addPage(std::make_unique<Page>(std::move(block)));
                JsonStore::save(*mag);
                addResultText->setText("<p style='color:green;'>Page added successfully!</p>");
            } catch (const MagazineLockedException& e) {
                addResultText->setText(std::string("<p style='color:red;'>") + e.what() + "</p>");
            }
        });
    }
    void buildGallerySection() {
        root()->addWidget(std::make_unique<Wt::WText>("<h3>Add Photos to a Magazine</h3>"));
        root()->addWidget(std::make_unique<Wt::WText>("<p>Select up to 3 photos to upload as a gallery page.</p>"));
        root()->addWidget(std::make_unique<Wt::WText>("Magazine ID: "));
        auto idInput = root()->addWidget(std::make_unique<Wt::WLineEdit>());
        idInput->setPlaceholderText("e.g. lbobm571");
        root()->addWidget(std::make_unique<Wt::WBreak>());
        root()->addWidget(std::make_unique<Wt::WText>("Your Name: "));
        auto nameInput = root()->addWidget(std::make_unique<Wt::WLineEdit>());
        nameInput->setPlaceholderText("e.g. Tanjir");
        root()->addWidget(std::make_unique<Wt::WBreak>());
        root()->addWidget(std::make_unique<Wt::WText>("Caption: "));
        auto captionInput = root()->addWidget(std::make_unique<Wt::WLineEdit>());
        captionInput->setPlaceholderText("e.g. Our favorite memories");
        root()->addWidget(std::make_unique<Wt::WBreak>());
        root()->addWidget(std::make_unique<Wt::WText>("Photo 1: "));
        auto upload1 = root()->addWidget(std::make_unique<Wt::WFileUpload>());
        upload1->setFileTextSize(30);
        root()->addWidget(std::make_unique<Wt::WBreak>());
        root()->addWidget(std::make_unique<Wt::WText>("Photo 2 (optional): "));
        auto upload2 = root()->addWidget(std::make_unique<Wt::WFileUpload>());
        upload2->setFileTextSize(30);
        root()->addWidget(std::make_unique<Wt::WBreak>());
        root()->addWidget(std::make_unique<Wt::WText>("Photo 3 (optional): "));
        auto upload3 = root()->addWidget(std::make_unique<Wt::WFileUpload>());
        upload3->setFileTextSize(30);
        root()->addWidget(std::make_unique<Wt::WBreak>());
        auto uploadBtn = root()->addWidget(std::make_unique<Wt::WPushButton>("Upload Photos"));
        auto uploadResultText = root()->addWidget(std::make_unique<Wt::WText>());
        auto uploadedPaths = std::make_shared<std::vector<std::string>>();
        upload1->uploaded().connect([=] {
            std::string dest1 = "uploads/img1_" + std::to_string(time(nullptr)) + ".jpg";
            if (copyFile(upload1->spoolFileName(), dest1)) {
                std::ifstream check(dest1, std::ios::binary | std::ios::ate);
                if (check.tellg() > 0) uploadedPaths->push_back(dest1);
            }
        });
        upload2->uploaded().connect([=] {
            std::string dest2 = "uploads/img2_" + std::to_string(time(nullptr)) + ".jpg";
            if (copyFile(upload2->spoolFileName(), dest2)) {
                std::ifstream check(dest2, std::ios::binary | std::ios::ate);
                if (check.tellg() > 0) uploadedPaths->push_back(dest2);
            }
        });
        upload3->uploaded().connect([=] {
            std::string dest3 = "uploads/img3_" + std::to_string(time(nullptr)) + ".jpg";
            if (copyFile(upload3->spoolFileName(), dest3)) {
                std::ifstream check(dest3, std::ios::binary | std::ios::ate);
                if (check.tellg() > 0) uploadedPaths->push_back(dest3);
            }
        });
        uploadBtn->clicked().connect([=] {
            uploadedPaths->clear();
            std::string magId = idInput->text().toUTF8();
            auto mag = findMagazine(magId);
            if (!mag) {
                uploadResultText->setText("<p style='color:red;'>Magazine ID not found.</p>");
                return;
            }
            upload1->upload();
            upload2->upload();
            upload3->upload();
            auto timer = std::make_shared<Wt::WTimer>();
            timer->setInterval(std::chrono::milliseconds(600));
            timer->setSingleShot(true);
            timer->timeout().connect([=] {
                std::string contributor = nameInput->text().toUTF8();
                std::string caption = captionInput->text().toUTF8();
                if (contributor.empty()) return;
                std::vector<std::string> finalPaths;
                for (auto& p : *uploadedPaths) finalPaths.push_back(p);
                if (finalPaths.empty()) {
                    uploadResultText->setText("<p style='color:red;'>No photos were uploaded.</p>");
                    return;
                }
                try {
                    auto block = std::make_unique<GalleryBlock>(contributor, finalPaths, caption);
                    mag->addPage(std::make_unique<Page>(std::move(block)));
                    JsonStore::save(*mag);
                    uploadResultText->setText("<p style='color:green;'>Photos uploaded and added!</p>");
                } catch (const MagazineLockedException& e) {
                    uploadResultText->setText(std::string("<p style='color:red;'>") + e.what() + "</p>");
                }
            });
            timer->start();
        });
    }
    void buildLockSection() {
        root()->addWidget(std::make_unique<Wt::WText>("<h3>Lock a Magazine</h3>"));
        root()->addWidget(std::make_unique<Wt::WText>("<p>Once locked, no more pages can be added.</p>"));
        root()->addWidget(std::make_unique<Wt::WText>("Magazine ID: "));
        auto idInput = root()->addWidget(std::make_unique<Wt::WLineEdit>());
        idInput->setPlaceholderText("e.g. lbobm571");
        root()->addWidget(std::make_unique<Wt::WBreak>());
        auto lockBtn = root()->addWidget(std::make_unique<Wt::WPushButton>("Lock Magazine"));
        auto lockResultText = root()->addWidget(std::make_unique<Wt::WText>());
        lockBtn->clicked().connect([=] {
            std::string magId = idInput->text().toUTF8();
            auto mag = findMagazine(magId);
            if (!mag) {
                lockResultText->setText("<p style='color:red;'>Magazine ID not found.</p>");
                return;
            }
            mag->lock();
            JsonStore::save(*mag);
            lockResultText->setText("<p style='color:green;'>Magazine locked! Status: " + mag->status() + "</p>");
        });
    }
    void buildRevealSection() {
        root()->addWidget(std::make_unique<Wt::WText>("<h3>Reveal a Magazine</h3>"));
        root()->addWidget(std::make_unique<Wt::WText>("Magazine ID: "));
        auto idInput = root()->addWidget(std::make_unique<Wt::WLineEdit>());
        idInput->setPlaceholderText("e.g. lbobm571");
        root()->addWidget(std::make_unique<Wt::WBreak>());
        auto revealBtn = root()->addWidget(std::make_unique<Wt::WPushButton>("Reveal Magazine"));
        root()->addWidget(std::make_unique<Wt::WBreak>());
        auto revealArea = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
        revealBtn->clicked().connect([=] {
            revealArea->clear();
            std::string magId = idInput->text().toUTF8();
            auto mag = findMagazine(magId);
            if (!mag) {
                revealArea->addWidget(std::make_unique<Wt::WText>("<p style='color:red;'>Magazine ID not found.</p>"));
                return;
            }
            auto revealText = revealArea->addWidget(std::make_unique<Wt::WText>(mag->render()));
            revealText->setTextFormat(Wt::TextFormat::UnsafeXHTML);
        });
    }
};
std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment& env) {
    return std::make_unique<WishForApp>(env);
}
int main(int argc, char** argv) {
    return Wt::WRun(argc, argv, &createApplication);
}
