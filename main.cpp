#include "inc/findlobbymanager.h"
#include "inc/mainclient.h"
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStringListModel>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/resources/others/icon.svg"));
    QQmlApplicationEngine engine;
    qmlRegisterType<MainClient>("MainClient", 1, 0, "Client");
    qmlRegisterType<PlayersList>("PlayersList", 1, 0, "PlayersList");
    qmlRegisterType<FindLobbyManager>("FindLobbyManage", 1, 0, "FindLobbyManager");
    engine.addImportPath("qrc:/qml");
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
