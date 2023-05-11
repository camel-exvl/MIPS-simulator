#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QFont>
#include <QFontDatabase>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QQuickStyle::setStyle("Material");
    QFontDatabase::addApplicationFont(":MIPS-simulator/fonts/MesloLGM NF.ttf");
    QFont font("Microsoft YaHei", 24);
    app.setFont(font);

    app.setOrganizationName("MIPS-simulator");
    app.setOrganizationDomain("MIPS-simulator");
    app.setApplicationName("MIPS-simulator");
    const QUrl url(u"qrc:/MIPS-simulator/Main.qml"_qs);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
