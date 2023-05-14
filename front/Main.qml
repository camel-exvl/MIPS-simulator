import QtQuick
import QtQuick.Window
import QtCore
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

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
                }
            }
            MenuItem {
                text: qsTr("运行")
                onTriggered: {
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
                ListView {
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
                tabBar.currentIndex = 0;
                stackLayout.currentIndex = 0;
            } else if (suffix === "bin") {
                tabBar.currentIndex = 1;
                stackLayout.currentIndex = 1;
            } else {
                message.show("不支持的文件格式");
            }
            fileProcess.openFile(selectedFile);
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
            function onSaveFileSuccess() {
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
