import QtQuick 2.0
import io.qt.examples.backend 1.0

HomeForm {
    anchors.fill: parent
    button.onClicked: messageDialog.show(qsTr("Button pressed"))
    BackEnd {
        id: backend
    }
//    button.onClicked: backend.dataReceived(textField.text, textField1.text,
//                                 textField2.text)
}
