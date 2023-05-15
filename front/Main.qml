import QtQuick
import QtQuick.Window
import QtCore
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import Qt.labs.qmlmodels
import CodeTableModel 1.0

Window {
    id: window
    width: 1024
    height: 768
    visible: true
    title: qsTr("MIPS Simulator")

    readonly property real dpScale: 1     // 分辨率: 范围(0.8 ~ 2)   还可以添加 PinchArea 指捏缩放
    readonly property real pixelPerDp: Screen.pixelDensity * 0.12
    function dp(x) {
        return x * pixelPerDp * dpScale;
    }

    property url currentFile: ""

    MessagePopup {
        id: message
    }

    MenuBar {
        id: menuBar
        width: window.width
        Menu {
            title: qsTr("文件")
            MenuItem {
                text: qsTr("新建")
                onTriggered: {
                    window.title = qsTr("MIPS Simulator");
                    currentFile = "";
                    edit.text = "";
                }
            }
            MenuItem {
                text: qsTr("打开")
                onTriggered: {
                    fileOpenDialog.open();
                }
            }
            MenuItem {
                text: qsTr("保存")
                onTriggered: {
                    fileSaveDialog.open();
                }
            }
            MenuItem {
                text: qsTr("退出")
                onTriggered: {
                    Qt.quit();
                }
            }
        }
        Menu {
            title: qsTr("编辑")
            MenuItem {
                text: qsTr("撤销")
                onTriggered: {
                    edit.undo();
                }
            }
            MenuItem {
                text: qsTr("恢复")
                onTriggered: {
                    edit.redo();
                }
            }
            MenuItem {
                text: qsTr("剪切")
                onTriggered: {
                    edit.cut();
                }
            }
            MenuItem {
                text: qsTr("复制")
                onTriggered: {
                    edit.copy();
                }
            }
            MenuItem {
                text: qsTr("粘贴")
                onTriggered: {
                    edit.paste();
                }
            }
            MenuItem {
                text: qsTr("全选")
                onTriggered: {
                    edit.selectAll();
                }
            }
        }
        Menu {
            title: qsTr("运行")
            MenuItem {
                text: qsTr("编译")
                onTriggered: {
                    if (currentFile === "") {
                        message.show(qsTr("请先保存文件"), "red", 5000);
                        return;
                    }
                    debug.assemble(system, edit.text, currentFile);
                }
                Connections {
                    target: debug
                    function onSuccess() {
                        message.show(qsTr("编译成功"), "green", 5000);
                    }
                    function onFail(err) {
                        message.show(err, "red", 5000);
                    }
                }
            }
            MenuItem {
                text: qsTr("运行")
                onTriggered: {
                    if (currentFile === "") {
                        message.show(qsTr("请先保存文件"), "red", 5000);
                        return;
                    }
                    codeTableModel.initTableFromBinFile(system, currentFile);
                }
                Connections {
                    target: codeTableModel
                    function onSuccess() {
                        tabBar.currentIndex = 1;
                        stackLayout.currentIndex = 1;
                    }
                    function onFail(err) {
                        message.show(err, "red", 5000);
                    }
                }
            }
            MenuItem {
                text: qsTr("单步")
                onTriggered: {
                }
            }
        }
        Menu {
            title: qsTr("小工具")
            // 小工具
            MenuItem {
            }
        }
        Menu {
            title: qsTr("帮助")
            MenuItem {
                text: qsTr("关于")
                onTriggered: {
                    aboutDialog.open();
                }
            }
        }
    }

    TabBar {
        id: tabBar
        anchors.top: menuBar.bottom
        width: window.width
        height: dp(30)
        TabButton {
            text: qsTr("编辑")
            onClicked: {
                stackLayout.currentIndex = 0;
            }
        }
        TabButton {
            text: qsTr("调试")
            onClicked: {
                stackLayout.currentIndex = 1;
            }
        }
    }

    StackLayout {
        id: stackLayout
        anchors.top: tabBar.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.topMargin: tabBar.contentHeight

        Rectangle {
            id: editorTab
            border.color: "lightgrey"
            Flickable {
                id: flick
                anchors.fill: parent
                contentWidth: edit.contentWidth
                contentHeight: edit.contentHeight
                clip: true

                function ensureVisible(r) {
                    if (contentX >= r.x)
                        contentX = r.x;
                    else if (contentX + width <= r.x + r.width)
                        contentX = r.x + r.width - width;
                    if (contentY >= r.y)
                        contentY = r.y;
                    else if (contentY + height <= r.y + r.height)
                        contentY = r.y + r.height - height;
                }

                TextEdit {
                    id: edit
                    width: flick.width
                    height: flick.height
                    focus: true
                    wrapMode: TextEdit.Wrap
                    font.family: "Consolas"
                    font.pixelSize: dp(30)
                    selectByMouse: true
                    onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
                }
            }
        }

        Rectangle {
            id: debugTab
            visible: false

            Rectangle {
                id: commandRec
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                width: parent.width / 3 * 2
                border.color: "lightgrey"
                TableView {
                    anchors.fill: parent
                    model: codeTableModel
                    columnWidthProvider: function (column) {
                        switch (column) {
                        case 0:
                            return dp(80);
                        case 1:
                            return dp(200);
                        case 2:
                            return dp(200);
                        case 3:
                            return dp(300);
                        }
                    }
                    delegate: DelegateChooser {
                        role: "type"
                        DelegateChoice {
                            roleValue: CodeTableModel.Boolean
                            delegate: CheckBox {
                                checked: model.value
                            }
                        }
                        DelegateChoice {
                            roleValue: CodeTableModel.String
                            delegate: Rectangle {
                                Label {
                                    anchors.centerIn: parent
                                    text: model.value
                                    // font.family: heading ? "Microsoft YaHei" : "Consolas"
                                    font.pixelSize: dp(30)
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                }
                            }
                            // MouseArea {
                            //     anchors.fill: parent
                            //     onClicked: {
                            //         // print value from clicked cell
                            //         var idx = tableModel.index(row, column);
                            //         console.log("Clicked cell: ", tableModel.data(idx));

                            //         // print values from all cells in a row
                            //         console.log("Clicked row: ");
                            //         for (var i = 0; i < tableModel.columnCount(); i++) {
                            //             var idx2 = tableModel.index(row, i);
                            //             var data = tableModel.data(idx2);
                            //             console.log(data);
                            //         }
                            //     }
                            // }
                        }
                    }
                }
            }

            Rectangle {
                id: registerRec
                anchors.top: parent.top
                anchors.left: commandRec.right
                anchors.bottom: parent.bottom
                width: parent.width / 3
                border.color: "lightgrey"
            }
        }
    }

    FileDialog {
        id: fileOpenDialog
        title: qsTr("打开文件")
        nameFilters: ["源文件 (*.asm)", "二进制文件 (*.bin)"]
        currentFolder: "./"
        fileMode: FileDialog.OpenFile
        onAccepted: {
            var suffix = selectedFile.toString().split(".").pop();
            if (suffix === "asm") {
                fileProcess.openFile(selectedFile);
            } else if (suffix === "bin") {
                codeTableModel.initTableFromBinFile(system, currentFile);
            } else {
                message.show("不支持的文件格式");
            }
        }
        Connections {
            target: fileProcess
            function onFail(err) {
                message.show(err, "red", 5000);
            }
            function onOpenFileSuccess(text) {
                currentFile = fileOpenDialog.selectedFile;
                window.title = "MIPS Simulator - " + fileProcess.getFileName(currentFile);
                edit.text = text;
                tabBar.currentIndex = 0;
                stackLayout.currentIndex = 0;
            }
        }
        Connections {
            target: codeTableModel
            function onSuccess() {
                currentFile = fileOpenDialog.selectedFile;
                window.title = "MIPS Simulator - " + fileProcess.getFileName(currentFile);
                tabBar.currentIndex = 1;
                stackLayout.currentIndex = 1;
            }
            function onFail(err) {
                message.show(err, "red", 5000);
            }
        }
    }

    FileDialog {
        id: fileSaveDialog
        title: qsTr("保存文件")
        nameFilters: ["源文件 (*.asm)"]
        currentFolder: "./"
        fileMode: FileDialog.SaveFile
        onAccepted: {
            fileProcess.saveFile(selectedFile, edit.text);
        }
        Connections {
            target: fileProcess
            function onFail(err) {
                message.show(err, "red", 5000);
            }
            function onSuccess() {
                currentFile = fileSaveDialog.selectedFile;
                window.title = "MIPS Simulator - " + fileProcess.getFileName(currentFile);
            }
        }
    }

    Component.onCompleted: {
        edit.clear();
        tabBar.currentIndex = 0;
        stackLayout.currentIndex = 0;
    }
}
