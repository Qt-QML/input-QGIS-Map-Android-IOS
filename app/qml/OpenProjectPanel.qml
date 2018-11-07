import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QgsQuick 0.1 as QgsQuick
import lc 1.0
import "."  // import InputStyle singleton

Page {

    property int activeProjectIndex: -1
    property string activeProjectPath: __projectsModel.data(__projectsModel.index(activeProjectIndex), ProjectModel.Path)
    property string activeProjectName: __projectsModel.data(__projectsModel.index(activeProjectIndex), ProjectModel.Name)

    property real rowHeight: InputStyle.scale(InputStyle.buttonSize * 3)

    id: projectsPanel
    visible: false
    contentWidth: parent.width
    contentHeight: parent.height - rowHeight

    background: Rectangle {
        color: InputStyle.clrPanelMain
        opacity: InputStyle.panelOpacity
    }

    //    header: Rectangle {
    //        height: 48 // TODO bug???
    //        width: parent.width
    //        color: InputStyle.clrPanelMain
    //        Text {
    //            anchors.fill: parent
    //            text: "Projects"
    //            color: InputStyle.fontColor
    //            font.pixelSize: InputStyle.fontPixelSizeNormal
    //            verticalAlignment: Text.AlignVCenter
    //            horizontalAlignment: Text.AlignHCenter
    //        }
    //    }
    footer: Item {}

    contentChildren: [

        ColumnLayout {
            id: contentLayout
            anchors.fill: parent
            Layout.fillWidth: true
            spacing: 0

            // TODO redundat for NOW
            Rectangle {
                id: projectMenu
                color: InputStyle.panelBackground2
                Layout.fillWidth: true
                height: projectsPanel.rowHeight

                RowLayout {
                    anchors.fill: parent
                    Text {
                        text: "My projects"
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    Text {
                        text: "All projects"
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            ListView {
                id: listView
                model: __projectsModel
                height: projectsPanel.height - projectsPanel.rowHeight
                width: projectsPanel.width
                delegate: ItemDelegate {
                    id: itemDelegate
                    //text: name
                    width: listView.width
                    height: projectMenu.height

                    RowLayout {
                        id: row
                        anchors.fill: parent
                        spacing: 0

                        Rectangle {
                            id: iconContainer
                            height: row.height
                            width: height

                            Image {
                                anchors.margins: 20 // TODO @vsklencar
                                id: icon
                                anchors.fill: parent
                                source: 'file.svg'
                                fillMode: Image.PreserveAspectFit
                            }

                            ColorOverlay {
                                anchors.fill: icon
                                source: icon
                                color: InputStyle.fontColor
                            }

                        }

                        Rectangle {
                            id: textContainer
                            height: row.height
                            width: row.width - (iconContainer.width * 2)
                            Text {
                                anchors.fill: parent
                                text: name
                                leftPadding: 20
                                font.pointSize: 24
                                font.weight: Font.Bold
                                color: InputStyle.fontColor
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }

                            Text {
                                height: row.height/3
                                text: "Some info smaller font"
                                anchors.right: parent.right
                                anchors.bottom: parent.bottom
                                anchors.left: parent.left
                                leftPadding: 20
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter

                                font.pointSize: 12
                            }
                        }

                        Rectangle {
                            height: row.height
                            width: iconContainer.width

                            Button {
                                id: icon2
                                anchors.fill: parent
                                anchors.margins: 20
                                anchors.left: textContainer.right
                                text: "..."
                            }
                        }
                    }


                    onClicked: {
                        console.log("active project:", name, projectMenu.height, projectsPanel.rowHeight)
                        //projectsPanel.activeProjectIndex = index
                        // projectsPanel.visible = false
                    }
                }
            }
        }

    ]

}
