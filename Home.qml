import QtQuick 2.0
import io.qt.examples.backend 1.0
import QtQuick.Controls 2.14

HomeForm {
    anchors.fill: parent



     initButton.onClicked:      backend.init(portSelf.text);
     connectButton.onClicked:   backend.connect(portOut.text,ip.text);
     sendButton.onClicked:{
                                backend.dataReceived(portSelf.text, portOut.text,ip.text,filePath.text)
                                backend.send(filePath.text);
     }

     setButton.onClicked:       backend.setFileSavePath(fileSavePath.text);


    BackEnd {
        id: backend
    }

}
