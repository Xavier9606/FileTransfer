import QtQuick 2.14
import QtQuick.Controls 2.14

Page {
    id: page
    width: 600
    height: 400
    property alias button: button

    title: qsTr("Home")

    Label {
        text: qsTr("You are on the home page.")
        anchors.centerIn: parent
    }

    TextField {
        id: textField
        x: 52
        y: 70
        text: qsTr("IP")
    }

    TextField {
        id: textField1
        x: 52
        y: 125
        text: qsTr("PORT")
    }

    TextField {
        id: textField2
        x: 52
        y: 186
        text: qsTr("FILE PATH")
    }

    Button {
        id: button
        x: 52
        y: 256
        text: qsTr("Button")
    }
}
