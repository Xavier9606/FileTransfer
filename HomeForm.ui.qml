import QtQuick 2.14
import QtQuick.Controls 2.14

Page {
    id: page
    width: 600
    height: 400
    property alias portOut: portOut
    property alias ip: ip
    property alias filePath: filePath
    property alias portSelf: portSelf
    property alias mybutton: mybutton

    title: qsTr("Home")

    TextField {
        id: portSelf
        x: 95
        y: 67
        font.capitalization: Font.AllUppercase
        placeholderText: "Your port"
    }

    TextField {
        id: filePath
        x: 95
        y: 289
        width: 317
        height: 44
        text: qsTr("")
        placeholderText: "PATH"
    }

    Button {
        id: mybutton
        x: 444
        y: 287
        text: qsTr("Send")
    }

    TextField {
        id: ip
        x: 335
        y: 67
        placeholderText: "IP"
    }

    TextField {
        id: portOut
        x: 335
        y: 122
        text: qsTr("")
        placeholderText: "PORT"
    }

    Button {
        id: button
        x: 356
        y: 176
        text: qsTr("CONNECT")
    }

    Button {
        id: button1
        x: 121
        y: 122
        text: qsTr("START")
    }
}
