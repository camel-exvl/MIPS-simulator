#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QFont>
#include "middle/fileprocess.h"
#include "middle/debug.h"
#include "middle/codetablemodel.h"
#include "middle/registertablemodel.h"
#include "middle/memorytablemodel.h"
#include "middle/tools.h"
#include "main.h"

System sys;

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    FileProcess fileProcess;
    Debug debug;
    CodeTableModel codeTableModel;
    RegisterTableModel registerTableModel;
    MemoryTableModel memoryTableModel;
    Tools tools;
    registerTableModel.initTable();
    engine.rootContext()->setContextProperty("fileProcess", &fileProcess);
    engine.rootContext()->setContextProperty("debug", &debug);
    engine.rootContext()->setContextProperty("codeTableModel", &codeTableModel);
    engine.rootContext()->setContextProperty("registerTableModel", &registerTableModel);
    engine.rootContext()->setContextProperty("memoryTableModel", &memoryTableModel);
    engine.rootContext()->setContextProperty("tools", &tools);
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
