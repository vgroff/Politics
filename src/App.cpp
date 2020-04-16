#include "../include/App.hpp"
#include "../include/Engine.hpp"


QPointer<QTextEdit> App::messagesWidget = nullptr;

App::App(QWidget *parent) 
    : QMainWindow(parent) {
    resize(appHeight, appWidth);
    setWindowTitle(QApplication::translate("toplevel", "Top-level widget"));
}

int App::setup() {
    // Creates an instance of QApplication
    // std::cout << std::setprecision(4);
    // std::srand(38);
    // Engine e = Engine::testSetup();
    // for (int i = 0; i < 1; i++) {
    //     e.playTurn();
    // }

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
    messagesWidget = textEditor;

    topLevelLayout = QPointer<QVBoxLayout>(new QVBoxLayout());
    topLevelLayout->addWidget(mainSection);
    topLevelLayout->addWidget(bottomSection);
    topLevelLayout->addWidget(textEditor);

    App::log("next turn");

    QObject::connect(nextTurnButton, SIGNAL(clicked()), this, SLOT(nextTurnButtonClicked()));

    innerWidget->setLayout(topLevelLayout);

    this->show();
    return 0;
}

void App::nextTurnButtonClicked() {
    App::log("next turn");
}

void App::log(std::string newLogText) {
    if (messagesWidget) {
        auto document = messagesWidget->document();
        auto text = document->toPlainText();
        document->setPlainText(text + newLogText.c_str() + "\n");
    }
}