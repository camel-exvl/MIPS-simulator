#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QFont>
#include "main.h"
#include "middle/fileprocess.h"
#include "middle/debug.h"
#include "middle/codetablemodel.h"
#include "middle/registertablemodel.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    System system;
    FileProcess fileProcess;
    Debug debug;
    CodeTableModel codeTableModel;
    RegisterTableModel registerTableModel;
    registerTableModel.initTable(system);
    engine.rootContext()->setContextProperty("system", QVariant::fromValue(system));
    engine.rootContext()->setContextProperty("fileProcess", &fileProcess);
    engine.rootContext()->setContextProperty("debug", &debug);
    engine.rootContext()->setContextProperty("codeTableModel", &codeTableModel);
    engine.rootContext()->setContextProperty("registerTableModel", &registerTableModel);
    qmlRegisterType<CodeTableModel>("CodeTableModel", 1, 0, "CodeTableModel");

    QQuickStyle::setStyle("Material");
    QFont font("Microsoft YaHei", 24);
    app.setFont(font);

    app.setOrganizationName("MIPS-simulator");
    app.setOrganizationDomain("MIPS-simulator");
    app.setApplicationName("MIPS-simulator");
    const QUrl url(u"qrc:/MIPS-simulator/front/Main.qml"_qs);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
