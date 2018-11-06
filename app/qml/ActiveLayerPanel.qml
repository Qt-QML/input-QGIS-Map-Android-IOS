import QtQuick 2.7
import QtQuick.Controls 2.2

import lc 1.0
import QgsQuick 0.1 as QgsQuick
import "."  // import InputStyle singleton

Drawer {

    property int activeLayerIndex: 0
    property QgsQuick.VectorLayer activeVectorLayer: __layersModel.data(__layersModel.index(activeLayerIndex), LayersModel.VectorLayer)
    property string activeLayerName: __layersModel.data(__layersModel.index(activeLayerIndex), LayersModel.Name)

    id: activeLayerPanel
    visible: false
    modal: true
    interactive: true
    dragMargin: 0 // prevents opening the drawer by dragging.

    background: Rectangle {
        color: InputStyle.clrPanelBackground
        opacity: InputStyle.panelOpacity
    }

    Column {
        spacing: InputStyle.panelSpacing
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: InputStyle.panelSpacing * 2

        Text {
            text: "Active Layer"
            color: InputStyle.clrPanelMain
            font.pixelSize: InputStyle.fontPixelSizeBig
            font.bold: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }

        ListView {
            implicitWidth: parent.width
            implicitHeight: contentHeight
            model: __layersModel
            delegate: ItemDelegate {
                id: control
                text: name
                visible: isVector && !isReadOnly // show only vector and editable layers
                height: visible ? undefined : 0
                contentItem: Text {
                    color: index === activeLayerIndex ? InputStyle.clrPanelHighlight : InputStyle.clrPanelMain
                    rightPadding: control.spacing
                    text: control.text
                    font.pixelSize: InputStyle.fontPixelSizeNormal
                    elide: Text.ElideRight
                    visible: control.text
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    console.log("active layer:", name)
                    activeLayerIndex = index
                    activeLayerPanel.visible = false
                }
            }
        }
    }

}