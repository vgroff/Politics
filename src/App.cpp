#include "../include/App.hpp"


App::App(QWidget *parent) 
    : QMainWindow(parent) {
    resize(appHeight, appWidth);
    setWindowTitle(QApplication::translate("toplevel", "Top-level widget"));
}

void App::setupGUI() {
    innerWidget = QPointer<QWidget>(new QWidget(this));
    innerWidget->resize(appHeight, appWidth);
    mainSection = QPointer<MainSection>(new MainSection(appHeight, appWidth*3/4, &engine, innerWidget));

    textEditor = QPointer<QTextEdit>(new QTextEdit());

    topLevelLayout = QPointer<QVBoxLayout>(new QVBoxLayout());
    topLevelLayout->addWidget(mainSection);
    topLevelLayout->addWidget(textEditor);

    innerWidget->setLayout(topLevelLayout);
}

void App::setupLogger() {
    auto self = this;
    Log::setLogger([self](std::string text){
        self->log(text);
    });
}

void App::setupGame() {
    std::cout << std::setprecision(4);
    std::srand(38);
    engine.playTurn();
}

int App::setup() {
    setupGUI();
    setupLogger();
    setupGame();
    this->show();
    return 0;
}

void App::nextTurnButtonClicked() {
    engine.playTurn();
}

void App::log(std::string newLogText) {
    if (textEditor) {
        auto document = textEditor->document();
        auto text = document->toPlainText();
        document->setPlainText(text + newLogText.c_str() + "\n");
    }
}