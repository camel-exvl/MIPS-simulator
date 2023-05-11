import QtQuick
import QtQuick.Controls

Popup {
    function show(msg, color, timeout) {
        msgText.text = msg
        msgBg.color = color
        msgTimer.interval=timeout
        open()
        msgTimer.start()
    }

    anchors.centerIn: parent
    padding: dp(20)
    contentWidth: msgText.width
    contentHeight: msgText.height
    dim: false
    modal: false
    closePolicy: Popup.NoAutoClose
    background: Rectangle {
        id: msgBg
        radius: dp(5)
        opacity: 0.8
    }
    Text {
        id: msgText
        anchors.centerIn: parent
        visible: true
        color: "white"
        font.bold: true
        font.pixelSize: dp(36)
    }
    Timer {
        id: msgTimer
        onTriggered: {
            close()
        }
    }
}