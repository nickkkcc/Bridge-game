#ifndef LOBBY_H
#define LOBBY_H

#include <QWidget>

namespace Ui {
class lobby;
}

class lobby : public QWidget
{
    Q_OBJECT

public:
    explicit lobby(QWidget *parent = nullptr);
    ~lobby();

    void setupWindow();
private:
    Ui::lobby *ui;
};

#endif // LOBBY_H
