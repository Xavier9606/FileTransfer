import QtQuick 2.0
import io.qt.examples.backend 1.0
import QtQuick.Controls 2.14

HomeForm {
    anchors.fill: parent


     mybutton.onClicked: backend.dataReceived(portSelf.text, portOut.text,ip.text,filePath.text)
     //textField1.on

    BackEnd {
        id: backend
    }

}
