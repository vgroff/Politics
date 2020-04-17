#include "../include/App.hpp"


App::App(QWidget *parent) 
    : QMainWindow(parent) {
    resize(appHeight, appWidth);
    setWindowTitle(QApplication::translate("toplevel", "Top-level widget"));
}

void App::setupGUI() {
    innerWidget = QPointer<QWidget>(new QWidget(this));
    innerWidget->resize(appHeight, appWidth);
    mainSection = QPointer<QWidget>(new QWidget(innerWidget));
    bottomSection = QPointer<QWidget>(new QWidget(innerWidget));

    nextTurnButton = QPointer<QPushButton>(new QPushButton("Next turn", bottomSection));

    textEditor = QPointer<QTextEdit>(new QTextEdit());

    mainSection->setMinimumSize(appHeight, appWidth*2/3);
    mainSection->setStyleSheet("background:white;");
    bottomSection->setMinimumSize(appHeight, appWidth/10);
    bottomSection->setStyleSheet("background:blue;");

    topLevelLayout = QPointer<QVBoxLayout>(new QVBoxLayout());
    topLevelLayout->addWidget(mainSection);
    topLevelLayout->addWidget(bottomSection);
    topLevelLayout->addWidget(textEditor);

    QObject::connect(nextTurnButton, SIGNAL(clicked()), this, SLOT(nextTurnButtonClicked()));

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