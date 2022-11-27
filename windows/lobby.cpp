#include "windows/lobby.h"
#include "ui_lobby.h"

lobby::lobby(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lobby)
{
    ui->setupUi(this);
    setupWindow();
}

lobby::~lobby()
{
    delete ui;
}

void lobby::setupWindow()
{
    // QPixmap maps the pixels of the background fot the palatte to brush it onto the
    // current ui.
    QPixmap bkgnd(":/res/fon");
    bkgnd = bkgnd.scaled(this->size());
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);
    this->setWindowTitle ("Rubber Contract Bridge");
}
