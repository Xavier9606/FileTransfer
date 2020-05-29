import QtQuick 2.14
import QtQuick.Controls 2.14

Page {
    width: 600
    height: 400
    property alias label: label

    title: qsTr("Page 1")

    Label {
        id: label
        text: qsTr("You are on Page 1.")
        anchors.centerIn: parent
    }
}
