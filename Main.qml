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
    title: qsTr("MIPS-simulator")

    readonly property real dpScale: 1     // 分辨率: 范围(0.8 ~ 2)   还可以添加 PinchArea 指捏缩放
    readonly property real pixelPerDp: Screen.pixelDensity * 0.12
    function dp(x) {
        return x * pixelPerDp * dpScale;
    }

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
                    editor.newFile();
                }
            }
            MenuItem {
                text: qsTr("打开")
                onTriggered: {
                    fileDialog.open();
                }
            }
            MenuItem {
                text: qsTr("保存")
                onTriggered: {
                    fileDialog.save();
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
        currentIndex: 0
        TabButton {
            text: qsTr("编辑")
            onClicked: {
                editorTab.visible = true;
                consoleTab.visible = false;
            }
        }
        TabButton {
            text: qsTr("调试")
            onClicked: {
                editorTab.visible = false;
                consoleTab.visible = true;
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
    }

    FileDialog {
        id: fileDialog
        title: qsTr("打开文件")
        nameFilters: ["*.asm"]
        onAccepted: {
            editor.open(fileDialog.fileUrl);
        }
    }

    Component.onCompleted: {
        editor.newFile();
    }
}
