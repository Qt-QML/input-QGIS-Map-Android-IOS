/***************************************************************************
     testqgsquickidentifykit.cpp
     --------------------------------------
  Date                 : May 2018
  Copyright            : (C) 2018 by Viktor Sklencar
  Email                : vsklencar at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "testidentifykit.h"

#include <QObject>
#include <QApplication>
#include <QDesktopWidget>

#include "qgsapplication.h"
#include "qgsvectorlayer.h"
#include "qgsfeature.h"
#include "qgsgeometry.h"
#include "qgsvectordataprovider.h"

#include "qgsquickmapcanvasmap.h"
#include "identifykit.h"


void TestIdentifyKit::identifyOne()
{
  QgsCoordinateReferenceSystem crsGPS = QgsCoordinateReferenceSystem::fromEpsgId( 4326 );
  QVERIFY( crsGPS.authid() == "EPSG:4326" );

  QgsRectangle extent = QgsRectangle( -120, 23, -82, 47 );
  QgsQuickMapCanvasMap canvas;

  QgsVectorLayer *tempLayer = new QgsVectorLayer( QStringLiteral( "Point?crs=epsg:4326" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) );
  QVERIFY( tempLayer->isValid() );

  QgsQuickMapSettings *ms = canvas.mapSettings();
  ms->setDestinationCrs( crsGPS );
  ms->setExtent( extent );
  ms->setOutputSize( QSize( 1000, 500 ) );
  ms->setLayers( QList<QgsMapLayer *>() << tempLayer );

  IdentifyKit kit;
  kit.setMapSettings( ms );

  double pointX = -31.208;
  double pointY = 20.407999999999998;
  double pointX2 = pointX + 0.5;

  // add feature
  QgsFeature f1( tempLayer->dataProvider()->fields(), 1 );
  QgsPointXY point( pointX, pointY );
  QgsGeometry geom = QgsGeometry::fromPointXY( point ) ;
  f1.setGeometry( geom );

  // add another feature
  QgsFeature f2( tempLayer->dataProvider()->fields(), 1 );
  QgsPointXY point2( pointX2, pointY );
  QgsGeometry geom2 = QgsGeometry::fromPointXY( point2 ) ;
  f2.setGeometry( geom2 );

  tempLayer->dataProvider()->addFeatures( QgsFeatureList() << f1 << f2 );

  // exactly matches f1 point
  QgsPointXY screenPoint( 1954.0, 554.0 );
  FeatureLayerPair identifiedFeature = kit.identifyOne( screenPoint.toQPointF() );
  QVERIFY( identifiedFeature.isValid() );
  QVERIFY( identifiedFeature.feature().geometry().asPoint() == point );
}

void TestIdentifyKit::identifyOneDefinedVector()
{
  QgsCoordinateReferenceSystem crsGPS = QgsCoordinateReferenceSystem::fromEpsgId( 4326 );
  QVERIFY( crsGPS.authid() == "EPSG:4326" );

  QgsRectangle extent = QgsRectangle( -120, 23, -82, 47 );
  QgsQuickMapCanvasMap canvas;

  QgsVectorLayer *tempLayer = new QgsVectorLayer( QStringLiteral( "Point?crs=epsg:4326" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) );
  QVERIFY( tempLayer->isValid() );

  QgsVectorLayer *tempLayer2 = new QgsVectorLayer( QStringLiteral( "Point?crs=epsg:4326" ), QStringLiteral( "vl2" ), QStringLiteral( "memory" ) );
  QVERIFY( tempLayer->isValid() );

  QgsQuickMapSettings *ms = canvas.mapSettings();
  ms->setDestinationCrs( crsGPS );
  ms->setExtent( extent );
  ms->setOutputSize( QSize( 1000, 500 ) );
  ms->setLayers( QList<QgsMapLayer *>() << tempLayer );

  IdentifyKit kit;
  kit.setMapSettings( ms );

  double pointX = -31.208;
  double pointY = 20.407999999999998;
  double pointX2 = pointX + 0.5;

  // add feature
  QgsFeature f1( tempLayer->dataProvider()->fields(), 1 );
  QgsPointXY point( pointX, pointY );
  QgsGeometry geom = QgsGeometry::fromPointXY( point ) ;
  f1.setGeometry( geom );

  // add another feature
  QgsFeature f2( tempLayer2->dataProvider()->fields(), 1 );
  QgsPointXY point2( pointX2, pointY );
  QgsGeometry geom2 = QgsGeometry::fromPointXY( point2 ) ;
  f2.setGeometry( geom2 );

  tempLayer->dataProvider()->addFeatures( QgsFeatureList() << f1 );
  tempLayer2->dataProvider()->addFeatures( QgsFeatureList() << f2 );

  QgsPointXY screenPoint( 1954.0, 554.0 );
  FeatureLayerPair identifiedFeature = kit.identifyOne( screenPoint.toQPointF(), tempLayer2 );
  QVERIFY( identifiedFeature.isValid() );
  QVERIFY( identifiedFeature.feature().geometry().asPoint() == point2 );

}

void TestIdentifyKit::identifyInRadius()
{
  QgsCoordinateReferenceSystem crsGPS = QgsCoordinateReferenceSystem::fromEpsgId( 4326 );
  QVERIFY( crsGPS.authid() == "EPSG:4326" );

  QgsRectangle extent = QgsRectangle( -120, 23, -82, 47 );
  QgsQuickMapCanvasMap canvas;

  QgsVectorLayer *tempLayer = new QgsVectorLayer( QStringLiteral( "Point?crs=epsg:4326" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) );
  QVERIFY( tempLayer->isValid() );

  QgsQuickMapSettings *ms = canvas.mapSettings();
  ms->setDestinationCrs( crsGPS );
  ms->setExtent( extent );
  ms->setOutputSize( QSize( 1000, 500 ) );
  ms->setLayers( QList<QgsMapLayer *>() << tempLayer );

  IdentifyKit kit;
  kit.setMapSettings( ms );

  double pointX = -31.208;
  double pointY = 20.407999999999998;
  double pointX2 = pointX + 5;

  QgsFeature f1( tempLayer->dataProvider()->fields(), 1 );
  QgsPointXY point( pointX, pointY );
  QgsGeometry geom = QgsGeometry::fromPointXY( point ) ;
  f1.setGeometry( geom );

  QgsFeature f2( tempLayer->dataProvider()->fields(), 1 );
  QgsPointXY point2( pointX2, pointY );
  QgsGeometry geom2 = QgsGeometry::fromPointXY( point2 ) ;
  f2.setGeometry( geom2 );

  tempLayer->dataProvider()->addFeatures( QgsFeatureList() << f1 << f2 );

  kit.setSearchRadiusMm( 1.0 );
  QgsPointXY screenPoint( 1954.0, 554.0 );
  FeatureLayerPairs res = kit.identify( screenPoint.toQPointF() );
  QVERIFY( res.size() == 1 );

  kit.setSearchRadiusMm( 100.0 );
  res = kit.identify( screenPoint.toQPointF() );
  QVERIFY( res.size() == 2 );
}
