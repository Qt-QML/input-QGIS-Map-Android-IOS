﻿/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "../components"
import ".."
import lc 1.0

Page {
  id: root

  property var mapSettings
  property string coordinatesInDegrees: __inputUtils.degreesString( __positionKit.positionCoordinate )

  property real cellWidth: root.width * 0.4

  signal back()

  focus: true

  Keys.onReleased: {
    if ( event.key === Qt.Key_Back || event.key === Qt.Key_Escape ) {
      event.accepted = true
      root.back()
    }
  }

  MapPosition {
    id: mapPositioning

    positionKit: __positionKit
    mapSettings: root.mapSettings
  }

  header: PanelHeader {

    titleText: qsTr( "GPS info" )

    height: InputStyle.rowHeightHeader
    rowHeight: InputStyle.rowHeightHeader

    color: InputStyle.clrPanelMain

    onBack: root.back()
    withBackButton: true
  }

  ScrollView {
    id: scrollPage

    anchors.fill: parent
    contentWidth: availableWidth // to only scroll vertically

    spacing: InputStyle.panelSpacing

    background: Rectangle {
      anchors.fill: parent
      color: InputStyle.clrPanelMain
    }

    GridLayout {
      id: gridItem

      columns: 2

      anchors {
        fill: parent
        leftMargin: 2 * InputStyle.formSpacing
        rightMargin: InputStyle.formSpacing
        topMargin: InputStyle.formSpacings
      }

      TextRowWithTitle {
        id: longitude

        Layout.fillWidth: true

        titleText: qsTr( "Longitude" )
        text: {
          if ( !__positionKit.hasPosition ) {
            return qsTr( "Loading data from GPS" ) // if you do not have position yet
          }
          root.coordinatesInDegrees.split(", ")[0]
        }
      }

      TextRowWithTitle {
        id: latitude

        Layout.fillWidth: true

        titleText: qsTr( "Latitude" )
        text: {
          if ( !__positionKit.hasPosition ) {
            return qsTr( "Loading data from GPS" ) // if you do not have position yet
          }
          root.coordinatesInDegrees.split(", ")[1]
        }
      }

      TextRowWithTitle {
        id: projectX

        Layout.fillWidth: true

        titleText: qsTr( "X" )
        text: {
          if ( !__positionKit.hasPosition ) {
            return qsTr( "Loading data from GPS" ) // if you do not have projected position yet
          }
          __inputUtils.formatNumber( mapPositioning.mapPosition.x, 2 )
        }
      }

      TextRowWithTitle {
        id: projectY

        Layout.fillWidth: true

        titleText: qsTr( "Y" )
        text: {
          if ( !__positionKit.hasPosition ) {
            return qsTr( "Loading data from GPS" ) // if you do not have projected position yet
          }
          __inputUtils.formatNumber( mapPositioning.mapPosition.y, 2 )
        }
      }

      TextRowWithTitle {
        id: horizontalAccuracy

        Layout.fillWidth: true

        titleText: qsTr( "Horizontal accuracy" )
        text: {
          if ( !__positionKit.hasPosition ) {
            return qsTr( "Loading data from GPS" ) // if you do not have position yet
          }

          __positionKit.horizontalAccuracy < 0 ? qsTr( "N/A" ) : ( __inputUtils.formatNumber( __positionKit.horizontalAccuracy, 2 ) + " m" )
        }
      }

      TextRowWithTitle {
        id: verticalAccuracy

        Layout.fillWidth: true

        titleText: qsTr( "Vertical accuracy" )
        text: {
          if ( !__positionKit.hasPosition ) {
            return qsTr( "Loading data from GPS" ) // if you do not have position yet
          }

          __positionKit.verticalAccuracy < 0 ? qsTr( "N/A" ) : __inputUtils.formatNumber( __positionKit.verticalAccuracy, 2 ) + " m"
        }
      }

      TextRowWithTitle {
        id: altitude

        Layout.fillWidth: true
        Layout.row: 3
        Layout.column: 0

        titleText: qsTr( "Altitude" )
        text: {
          if ( !__positionKit.hasPosition ) {
            return qsTr( "Loading data from GPS" ) // if you do not have position yet
          }
          __inputUtils.formatNumber( __positionKit.altitude, 2 ) + " m"
        }
      }

      TextRowWithTitle {
        id: sattelites

        Layout.fillWidth: true
        Layout.row: 4
        Layout.column: 0

        titleText: qsTr( "Satellites (in use/view)" )
        text: {
          if ( __positionKit.satellitesUsed < 0 || __positionKit.satellitesVisible < 0 )
          {
            return qsTr( "Loading data from GPS" )
          }

          __positionKit.satellitesUsed + "/" + __positionKit.satellitesVisible
        }
      }

      TextRowWithTitle {
        id: speed

        Layout.fillWidth: true
        Layout.row: 5
        Layout.column: 0

        titleText: qsTr( "Speed" )
        text: __positionKit.speed < 0 ? qsTr( "Loading data from GPS" ) : __inputUtils.formatNumber( __positionKit.speed, 2 ) + " km/h"
      }

      TextRowWithTitle {
        id: lastFix

        Layout.fillWidth: true
        Layout.row: 6
        Layout.column: 0

        titleText: qsTr( "Last fix" )
        text: __positionKit.lastRead ? __positionKit.lastRead.toLocaleTimeString( Qt.locale() ) : qsTr( "Date not available" )
      }
    }
  }
}
