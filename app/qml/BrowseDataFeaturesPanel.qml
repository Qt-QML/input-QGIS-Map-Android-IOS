/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

import QtQuick 2.0
import QtQuick.Controls 2.12

Item {
  id: root

  signal backButtonClicked()
  signal featureClicked( var featureId )
  signal addFeatureClicked()
  signal searchTextChanged( string text )

  property bool layerHasGeometry: true
  property string layerName: ""
  property int featuresCount: __featuresModel.featuresCount

  Page {
    id: featuresPage
    anchors.fill: parent

    header: PanelHeader {
      id: featuresPageHeader
      height: InputStyle.rowHeightHeader
      width: parent.width
      color: InputStyle.clrPanelMain
      rowHeight: InputStyle.rowHeightHeader
      titleText: layerName + " (" + featuresCount + ")"
      
      onBack: {
        searchBar.deactivate()
        root.backButtonClicked()
       }

      withBackButton: true
    }

    SearchBar {
      id: searchBar

      allowTimer: true
      onSearchTextChanged: {
        root.searchTextChanged( text )
      }
    }

    BrowseDataView {
      id: browseDataView
      width: parent.width
      height: parent.height - browseDataToolbar.height
      y: searchBar.height
      clip: true

      onFeatureClicked: root.featureClicked( featureId )
    }

    footer: BrowseDataToolbar {
      id: browseDataToolbar
      visible: !layerHasGeometry
      onAddButtonClicked: addFeatureClicked()
    }
  }
}
