#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/fl_draw.H>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <fstream>

// Global input display
Fl_Input *display;
std::string current = "";
std::unordered_map<std::string, std::string> qa_map;

// Helper to lowercase a string for matching
std::string to_lower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(), ::tolower);
    return out;
}

// Load Q&A from qa.txt
void load_questions() {
    std::ifstream file("qa.txt");
    std::string line;
    while (std::getline(file, line)) {
        size_t sep = line.find('|');
        if (sep != std::string::npos) {
            std::string question = to_lower(line.substr(0, sep));
            std::string answer = line.substr(sep + 1);
            qa_map[question] = answer;
        }
    }
}

// Update the input display with text
void update_display(const std::string &text) {
    display->value(text.c_str());
}

// Answer the user question using the map
void answer_question() {
    std::string q = to_lower(display->value());
    auto it = qa_map.find(q);
    std::string answer = (it != qa_map.end()) ? it->second : "Sorry, I don't know the answer.";
    update_display(answer);
    current = "";
}

// Button click handler
void on_button_click(Fl_Widget *w, void *) {
    const char *label = w->label();
    if (std::string(label) == "C") {
        current = "";
        update_display("");
    } else if (std::string(label) == "=") {
        current = display->value();
        answer_question();
    }
}

// Custom Neon Button
class NeonButton : public Fl_Button {
public:
    NeonButton(int X, int Y, int W, int H, const char* L=0) : Fl_Button(X, Y, W, H, L) {}
    void draw() override {
        fl_draw_box(FL_ROUNDED_BOX, x(), y(), w(), h(), FL_DARK_CYAN);
        fl_color(FL_WHITE);
        fl_font(FL_HELVETICA_BOLD, 20);
        fl_draw(label(), x(), y(), w(), h(), FL_ALIGN_CENTER);
    }
};

// Handle keyboard inputs
int handle_key(int event) {
    if (event == FL_KEYDOWN) {
        int key = Fl::event_key();
        if (key == FL_Enter || key == FL_KP_Enter) {
            current = display->value();
            answer_question();
            return 1;
        }
        if (key == FL_Escape) {
            current = "";
            update_display("");
            return 1;
        }
    }
    return 0;
}

// Custom Window class to handle keys
class QAWindow : public Fl_Window {
public:
    QAWindow(int W, int H, const char* L=0) : Fl_Window(W, H, L) {}
    int handle(int event) override {
        if (handle_key(event)) return 1;
        return Fl_Window::handle(event);
    }
};

int main(int argc, char **argv) {
    load_questions();

    QAWindow *window = new QAWindow(700, 400, "AT07 - AI Assistant");
    window->color(fl_rgb_color(10, 15, 25));

    // Branding Logo (top-left)
    Fl_PNG_Image *logo = new Fl_PNG_Image("logo.jpeg");
    Fl_Box *logoBox = new Fl_Box(20, 20, 64, 64);
    logoBox->image(logo);
    logoBox->box(FL_NO_BOX);

    // Title Label
    Fl_Box *title = new Fl_Box(100, 30, 500, 40, "Welcome to AT07 - Your AI Assistant");
    title->labelsize(24);
    title->labelfont(FL_BOLD);
    title->labelcolor(FL_WHITE);
    title->box(FL_NO_BOX);

    // Input Box
    display = new Fl_Input(40, 100, 620, 60);
    display->textsize(22);
    display->color(fl_rgb_color(25, 35, 50));
    display->textcolor(FL_CYAN);
    display->box(FL_ROUNDED_BOX);

    // Buttons
    NeonButton *askBtn = new NeonButton(500, 180, 160, 60, "=");
    askBtn->callback(on_button_click);

    NeonButton *clearBtn = new NeonButton(320, 180, 160, 60, "C");
    clearBtn->callback(on_button_click);

    // Signature Label
    Fl_Box *credit = new Fl_Box(40, 330, 620, 30, "Made by Mr. Aayush Bhandari - Class 10, Shree Mavi Imiliya");
    credit->labelsize(14);
    credit->labelcolor(FL_DARK_CYAN);
    credit->box(FL_NO_BOX);

    window->end();
    window->show(argc, argv);
    return Fl::run();
}