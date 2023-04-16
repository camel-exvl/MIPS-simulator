#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "main.h"
#include "assembler.h"
#include "disassembler.h"
#include "systemsimulation.h"
#include "QFileDialog"

using namespace std;

System sys{};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle("MIPS模拟器");

    // System sys{};

    QMenuBar *menuBar = new QMenuBar(this);
    this->setMenuBar(menuBar);

    QMenu *disassembleMenu = new QMenu("反汇编", this);
    QAction *disassembleOpenAction = new QAction("打开文件", this);
    connect(disassembleOpenAction, &QAction::triggered, this, &MainWindow::disassembleOpenActionTriggered);
    disassembleMenu->addAction(disassembleOpenAction);

    menuBar->addMenu(disassembleMenu);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::disassembleOpenActionTriggered() {
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "", "汇编文件(*.bin)");
    if (fileName.isEmpty()) {
        return;
    }
    try {
        vector<string> res = disassemblerOpenFile(sys, fileName.toStdString());
        for (auto &i : res) {
            qDebug() << i.c_str();
        }
    } catch (const char *msg) {
        qDebug() << msg;
    }
}
