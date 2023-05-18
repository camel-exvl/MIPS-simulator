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
    width: 1600
    height: 900
    visible: true
    title: qsTr("MIPS Simulator")

    readonly property real dpScale: 1     // 分辨率: 范围(0.8 ~ 2)   还可以添加 PinchArea 指捏缩放
    readonly property real pixelPerDp: Screen.pixelDensity * 0.12
    function dp(x) {
        return x * pixelPerDp * dpScale;
    }

    property url currentFile: ""
    property int currentLine: -1

    MessagePopup {
        id: message
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
        TabButton {
            text: qsTr("小工具")
            onClicked: {
                stackLayout.currentIndex = 2;
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
                anchors.margins: dp(10)
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

        Grid {
            id: debugTab
            columns: 2
            rows: 2
            Rectangle {
                id: commandRec
                height: parent.height / 3 * 2
                width: parent.width / 5 * 4
                border.color: "lightgrey"
                TableView {
                    id: commandTable
                    anchors.centerIn: parent
                    height: parent.height * 0.9
                    width: parent.width * 0.9
                    model: codeTableModel
                    boundsBehavior: Flickable.StopAtBounds
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
                                onClicked: codeTableModel.setBreakpoint(row, checked)
                                Connections {
                                    target: codeTableModel
                                    function onFail(err) {
                                        message.show(err, "red", 5000);
                                    }
                                }
                            }
                        }
                        DelegateChoice {
                            roleValue: CodeTableModel.String
                            delegate: Rectangle {
                                color: currentLine != -1 && row === currentLine ? "lightblue" : "white"
                                Label {
                                    anchors.centerIn: parent
                                    text: model.value
                                    // font.family: heading ? "Microsoft YaHei" : "Consolas"
                                    font.pixelSize: dp(30)
                                    horizontalAlignment: Text.AlignLeft
                                    width: parent.width
                                }
                            }
                        }
                    }
                }
            }
            Rectangle {
                id: registerRec
                height: parent.height / 3 * 2
                width: parent.width / 5
                border.color: "lightgrey"
                TableView {
                    id: registerTable
                    anchors.centerIn: parent
                    height: parent.height * 0.9
                    width: parent.width * 0.9
                    model: registerTableModel
                    boundsBehavior: Flickable.StopAtBounds
                    columnWidthProvider: function (column) {
                        switch (column) {
                        case 0:
                            return dp(100);
                        case 1:
                            return dp(100);
                        case 2:
                            return dp(200);
                        }
                    }
                    delegate: Rectangle {
                        Label {
                            anchors.centerIn: parent
                            text: model.value
                            font.pixelSize: dp(30)
                            horizontalAlignment: Text.AlignLeft
                            width: parent.width
                        }
                    }
                }
            }
            Rectangle {
                id: memoryRec
                height: parent.height / 3
                width: parent.width / 5 * 4
                border.color: "lightgrey"
                TableView {
                    id: memoryTable
                    anchors.centerIn: parent
                    height: parent.height * 0.9
                    width: parent.width * 0.9
                    model: memoryTableModel
                    boundsBehavior: Flickable.StopAtBounds
                    columnWidthProvider: function (column) {
                        switch (column) {
                        case 0:
                            return dp(250);
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                        case 6:
                        case 7:
                        case 8:
                        case 9:
                        case 10:
                            return dp(200);
                        }
                    }
                    delegate: Rectangle {
                        Label {
                            anchors.centerIn: parent
                            text: model.value
                            font.pixelSize: dp(30)
                            horizontalAlignment: Text.AlignLeft
                            width: parent.width
                        }
                    }
                }
            }
        }
        Column {
            id: toolTab
            spacing: 0
            Rectangle {
                id: intRec
                height: parent.height / 3
                width: parent.width
                border.color: "lightgrey"
                Column {
                    id: intColumn
                    anchors.fill: parent
                    spacing: dp(20)
                    anchors.top: parent.top
                    anchors.topMargin: dp(10)
                    anchors.left: parent.left
                    anchors.leftMargin: dp(10)
                    Label {
                        id: intLabel
                        text: qsTr("整数与补码转换")
                        font.pixelSize: dp(30)
                    }
                    Row {
                        id: intRow
                        spacing: dp(10)
                        Label {
                            id: intInputLabel
                            text: qsTr("输入：")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: dp(30)
                            height: intInputRec.height
                        }
                        ComboBox {
                            id: intCombo
                            anchors.verticalCenter: parent.verticalCenter
                            height: intInputRec.height
                            width: dp(300)
                            font.pixelSize: dp(30)
                            model: ["十进制整数", "整数补码"]
                        }
                        Rectangle {
                            id: intInputRec
                            border.color: "grey"
                            height: intInput.contentHeight + dp(20)
                            width: intInput.contentWidth < dp(200) ? dp(200) : intInput.contentWidth + dp(20)
                            TextInput {
                                id: intInput
                                anchors.fill: parent
                                anchors.margins: dp(10)
                                font.pixelSize: dp(30)
                            }
                        }
                        Button {
                            id: intButton
                            text: qsTr("确定")
                            anchors.verticalCenter: parent.verticalCenter
                            height: intInputRec.height
                            font.pixelSize: dp(30)
                            onClicked: {
                                var input = intInput.text;
                                if (input === "") {
                                    message.show("请输入数据", "red", 5000);
                                    return;
                                }
                                tools.intCalculate(intCombo.currentIndex, input);
                            }
                            Connections {
                                target: tools
                                function onSuccessIntCalculate(integer, text) {
                                    intInteger.text = integer;
                                    intComplete.text = text;
                                }
                                function onFail(err) {
                                    message.show(err, "red", 5000);
                                }
                            }
                        }
                    }
                    Row {
                        id: intIntegerRow
                        spacing: dp(10)
                        Label {
                            id: intIntegerLabel
                            text: qsTr("整数：")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: dp(30)
                            height: intIntegerRec.height
                        }
                        Rectangle {
                            id: intIntegerRec
                            border.color: "grey"
                            height: intInteger.contentHeight + dp(20)
                            width: intInteger.contentWidth < dp(200) ? dp(200) : intInteger.contentWidth + dp(20)
                            TextInput {
                                id: intInteger
                                readOnly: true
                                anchors.fill: parent
                                anchors.margins: dp(10)
                                font.pixelSize: dp(30)
                            }
                        }
                    }
                    Row {
                        id: intCompleteRow
                        spacing: dp(10)
                        Label {
                            id: intCompleteLabel
                            text: qsTr("补码：")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: dp(30)
                            height: intCompleteRec.height
                        }
                        Rectangle {
                            id: intCompleteRec
                            border.color: "grey"
                            height: intComplete.contentHeight + dp(20)
                            width: intComplete.contentWidth < dp(200) ? dp(200) : intComplete.contentWidth + dp(20)
                            TextInput {
                                id: intComplete
                                readOnly: true
                                anchors.fill: parent
                                anchors.margins: dp(10)
                                font.pixelSize: dp(30)
                            }
                        }
                    }
                }
            }
            Rectangle {
                id: floatRec
                height: parent.height / 3
                width: parent.width
                border.color: "lightgrey"
                Column {
                    id: floatColumn
                    anchors.fill: parent
                    spacing: dp(20)
                    anchors.top: parent.top
                    anchors.topMargin: dp(10)
                    anchors.left: parent.left
                    anchors.leftMargin: dp(10)
                    Label {
                        id: floatLabel
                        text: qsTr("浮点数表示")
                        font.pixelSize: dp(30)
                    }
                    Row {
                        id: floatRow
                        spacing: dp(10)
                        Label {
                            id: floatInputLabel
                            text: qsTr("输入：")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: dp(30)
                            height: floatInputRec.height
                        }
                        ComboBox {
                            id: floatCombo
                            anchors.verticalCenter: parent.verticalCenter
                            height: floatInputRec.height
                            width: dp(300)
                            font.pixelSize: dp(30)
                            model: ["十进制浮点数", "单精度浮点数", "双精度浮点数"]
                        }
                        Rectangle {
                            id: floatInputRec
                            border.color: "grey"
                            height: floatInput.contentHeight + dp(20)
                            width: floatInput.contentWidth < dp(200) ? dp(200) : floatInput.contentWidth + dp(20)
                            TextInput {
                                id: floatInput
                                anchors.fill: parent
                                anchors.margins: dp(10)
                                font.pixelSize: dp(30)
                            }
                        }
                        Button {
                            id: floatButton
                            text: qsTr("确定")
                            anchors.verticalCenter: parent.verticalCenter
                            height: floatInputRec.height
                            font.pixelSize: dp(30)
                            onClicked: {
                                var input = floatInput.text;
                                if (input === "") {
                                    message.show("请输入数据", "red", 5000);
                                    return;
                                }
                                tools.floatCalculate(floatCombo.currentIndex, input);
                            }
                            Connections {
                                target: tools
                                function onSuccessFloatCalculate(number, floatCode, doubleCode) {
                                    floatNumber.text = number;
                                    floatFloat.text = floatCode;
                                    floatDouble.text = doubleCode;
                                }
                                function onFail(err) {
                                    message.show(err, "red", 5000);
                                }
                            }
                        }
                    }
                    Row {
                        id: floatNumberRow
                        spacing: dp(10)
                        Label {
                            id: floatNumberLabel
                            text: qsTr("浮点数：")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: dp(30)
                            height: floatNumberRec.height
                        }
                        Rectangle {
                            id: floatNumberRec
                            border.color: "grey"
                            height: floatNumber.contentHeight + dp(20)
                            width: floatNumber.contentWidth < dp(200) ? dp(200) : floatNumber.contentWidth + dp(20)
                            TextInput {
                                id: floatNumber
                                readOnly: true
                                anchors.fill: parent
                                anchors.margins: dp(10)
                                font.pixelSize: dp(30)
                            }
                        }
                    }
                    Row {
                        id: floatFloatRow
                        spacing: dp(10)
                        Label {
                            id: floatFloatLabel
                            text: qsTr("单精度浮点数：")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: dp(30)
                            height: floatFloatRec.height
                        }
                        Rectangle {
                            id: floatFloatRec
                            border.color: "grey"
                            height: floatFloat.contentHeight + dp(20)
                            width: floatFloat.contentWidth < dp(200) ? dp(200) : floatFloat.contentWidth + dp(20)
                            TextInput {
                                id: floatFloat
                                readOnly: true
                                anchors.fill: parent
                                anchors.margins: dp(10)
                                font.pixelSize: dp(30)
                            }
                        }
                    }
                    Row {
                        id: floatDoubleRow
                        spacing: dp(10)
                        Label {
                            id: floatDoubleLabel
                            text: qsTr("双精度浮点数：")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: dp(30)
                            height: floatDoubleRec.height
                        }
                        Rectangle {
                            id: floatDoubleRec
                            border.color: "grey"
                            height: floatDouble.contentHeight + dp(20)
                            width: floatDouble.contentWidth < dp(200) ? dp(200) : floatDouble.contentWidth + dp(20)
                            TextInput {
                                id: floatDouble
                                readOnly: true
                                anchors.fill: parent
                                anchors.margins: dp(10)
                                font.pixelSize: dp(30)
                            }
                        }
                    }
                }
            }
            Rectangle {
                id: calRec
                height: parent.height / 3
                width: parent.width
                border.color: "lightgrey"
                Column {
                    id: calColumn
                    anchors.fill: parent
                    spacing: dp(20)
                    anchors.top: parent.top
                    anchors.topMargin: dp(10)
                    anchors.left: parent.left
                    anchors.leftMargin: dp(10)
                    Label {
                        id: calLabel
                        text: qsTr("计算器")
                        font.pixelSize: dp(30)
                    }
                    Row {
                        id: calRow
                        spacing: dp(10)
                        Label {
                            id: calInputLabel
                            text: qsTr("输入计算表达式：")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: dp(30)
                            height: calInputRec.height
                        }
                        Rectangle {
                            id: calInputRec
                            border.color: "grey"
                            height: calInput.contentHeight + dp(20)
                            width: calInput.contentWidth < dp(200) ? dp(200) : calInput.contentWidth + dp(20)
                            TextInput {
                                id: calInput
                                anchors.fill: parent
                                anchors.margins: dp(10)
                                font.pixelSize: dp(30)
                            }
                        }
                        Button {
                            id: calButton
                            text: qsTr("确定")
                            anchors.verticalCenter: parent.verticalCenter
                            height: calInputRec.height
                            font.pixelSize: dp(30)
                            onClicked: {
                                var input = calInput.text;
                                if (input === "") {
                                    message.show("请输入数据", "red", 5000);
                                    return;
                                }
                                tools.calculate(input);
                            }
                            Connections {
                                target: tools
                                function onSuccessCalculate(result) {
                                    calResult.text = result;
                                }
                            }
                        }
                    }
                    Row {
                        id: calResultRow
                        spacing: dp(10)
                        Label {
                            id: calResultLabel
                            text: qsTr("结果：")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: dp(30)
                            height: calResultRec.height
                        }
                        Rectangle {
                            id: calResultRec
                            border.color: "grey"
                            height: calResult.contentHeight + dp(20)
                            width: calResult.contentWidth < dp(200) ? dp(200) : calResult.contentWidth + dp(20)
                            TextInput {
                                id: calResult
                                readOnly: true
                                anchors.fill: parent
                                anchors.margins: dp(10)
                                font.pixelSize: dp(30)
                            }
                        }
                    }
                }
            }
        }
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
                    debug.assemble(edit.text, currentFile);
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
                text: qsTr("调试")
                onTriggered: {
                    if (currentFile === "") {
                        message.show(qsTr("请先保存/编译文件"), "red", 5000);
                        return;
                    }
                    codeTableModel.initTableFromBinFile(String(currentFile).replace(".asm", ".bin"));
                }
                Connections {
                    target: codeTableModel
                    function onSuccess() {
                        memoryTableModel.initTable();
                        tabBar.currentIndex = 1;
                        stackLayout.currentIndex = 1;
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
                        message.show(qsTr("请先保存/编译文件"), "red", 5000);
                        return;
                    }
                    codeTableModel.executeToNextBreakpoint();
                    registerTableModel.initTable();
                    memoryTableModel.initTable();
                }
                Connections {
                    target: codeTableModel
                    function onSuccessExecute(PC) {
                        currentLine = PC;
                    }
                    function onFail(err) {
                        message.show(err, "red", 5000);
                    }
                }
                Connections {
                    target: registerTableModel
                    function onFail(err) {
                        message.show(err, "red", 5000);
                    }
                }
                Connections {
                    target: memoryTableModel
                    function onFail(err) {
                        message.show(err, "red", 5000);
                    }
                }
            }
            MenuItem {
                text: qsTr("单步")
                onTriggered: {
                    if (currentFile === "") {
                        message.show(qsTr("请先保存/编译文件"), "red", 5000);
                        return;
                    }
                    codeTableModel.executeOneStep();
                    registerTableModel.initTable();
                    memoryTableModel.initTable();
                }
                Connections {
                    target: codeTableModel
                    function onSuccessExecute(PC) {
                        currentLine = PC;
                    }
                    function onFail(err) {
                        message.show(err, "red", 5000);
                    }
                }
                Connections {
                    target: registerTableModel
                    function onFail(err) {
                        message.show(err, "red", 5000);
                    }
                }
                Connections {
                    target: memoryTableModel
                    function onFail(err) {
                        message.show(err, "red", 5000);
                    }
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
                codeTableModel.initTableFromBinFile(currentFile);
            } else {
                message.show("不支持的文件格式", "red", 5000);
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
