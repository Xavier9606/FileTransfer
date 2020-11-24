import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.13

Page {
    id: page
    width: 600
    height: 400
    property alias setButton: setButton
    property alias fileSavePath: fileSavePath
    property alias initButton: initButton
    property alias connectButton: connectButton
    property alias sendButton: sendButton
    property alias portOut: portOut
    property alias ip: ip
    property alias filePath: filePath
    property alias portSelf: portSelf

    title: qsTr("Home")

    TextField {
        id: portSelf
        x: 95
        y: 115
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
        id: sendButton
        x: 444
        y: 287
        text: qsTr("Send")
    }

    TextField {
        id: ip
        x: 335
        y: 115
        placeholderText: "IP"
    }

    TextField {
        id: portOut
        x: 335
        y: 170
        text: qsTr("")
        placeholderText: "PORT"
    }

    Button {
        id: connectButton
        x: 356
        y: 224
        text: qsTr("CONNECT")
    }

    Button {
        id: initButton
        x: 121
        y: 170
        text: qsTr("START")
    }

    TextField {
        id: fileSavePath
        x: 95
        y: 43
        width: 317
        height: 44
        text: qsTr("")
        font.capitalization: Font.MixedCase
        placeholderText: "SAVE PATH"
    }

    Button {
        id: setButton
        x: 444
        y: 43
        text: qsTr("SET")
    }
}
