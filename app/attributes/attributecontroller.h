/***************************************************************************
 attributecontroller.h
  --------------------------------------
  Date                 : 20.4.2021
  Copyright            : (C) 2021 by Peter Petrik
  Email                : zilolv@gmail.com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ATTRIBUTECONTROLLER_H
#define ATTRIBUTECONTROLLER_H

#include <QSortFilterProxyModel>
#include <QObject>
#include <QVariant>
#include <memory>
#include <QMap>
#include <QVector>
#include <QUuid>


#include "featurelayerpair.h"
#include "attributedata.h"

#include "qgsfeature.h"
#include "qgseditformconfig.h"
#include "qgsexpressioncontext.h"
#include "qgsattributeeditorcontainer.h"

class AttributeFormModel;
class AttributeFormProxyModel;
class AttributeTabModel;
class AttributeTabProxyModel;
class QgsVectorLayer;
class RememberAttributesController;

/**
 * This is implementation of the controller between Attribute*Model
 * and the QML's .FeatureForm. It is responsible for creation of the
 * models, and CRUD management between QgsFeature and the form. It connects
 * the QgsFeature Fields on the backend side and the QML widgets on the user side
 *
 * Remember values functionality is done by RememberAttributes controller.
 *
 * \note QML Type: AttributeController
 */
class  AttributeController : public QObject
{
    Q_OBJECT

    /**
     * FeatureLayerPair for the model. Input for attributes model.
     */
    Q_PROPERTY( FeatureLayerPair featureLayerPair READ featureLayerPair WRITE setFeatureLayerPair NOTIFY featureLayerPairChanged )

    // this is never nullptr (a form with no tabs will be parsed as having a single tab)
    Q_PROPERTY( AttributeTabProxyModel *attributeTabProxyModel READ attributeTabProxyModel  NOTIFY attributeTabProxyModelChanged )

    //! Returns TRUE if form is in Drag&Drop mode and has any tabs
    Q_PROPERTY( bool hasTabs READ hasTabs NOTIFY hasTabsChanged )

    //! Returns TRUE if has any changes
    Q_PROPERTY( bool hasAnyChanges READ hasAnyChanges NOTIFY hasAnyChangesChanged )

    Q_PROPERTY( RememberAttributesController *rememberAttributesController READ rememberAttributesController WRITE setRememberAttributesController NOTIFY rememberAttributesChanged )

    //! Returns TRUE if all hard constraints defined on fields are satisfied with the current attribute values
    Q_PROPERTY( bool constraintsHardValid READ constraintsHardValid NOTIFY constraintsHardValidChanged )

    //! Returns TRUE if all soft constraints defined on fields are satisfied with the current attribute values
    Q_PROPERTY( bool constraintsSoftValid READ constraintsSoftValid NOTIFY constraintsSoftValidChanged )

  public:
    AttributeController( QObject *parent = nullptr );
    ~AttributeController() override;

    //! Restore clean/initial state: no layer, no feature!
    Q_INVOKABLE void reset();

    //! Gets current featureLayerPair
    FeatureLayerPair featureLayerPair() const;
    //! Sets current featureLayerPair
    void setFeatureLayerPair( const FeatureLayerPair &pair );

    bool constraintsHardValid() const;
    bool constraintsSoftValid() const;
    bool hasTabs() const;
    bool hasAnyChanges() const;

    AttributeTabProxyModel *attributeTabProxyModel() const;
    AttributeFormProxyModel *attributeFormProxyModelForTab( int tabRow ) const;

    Q_INVOKABLE bool deleteFeature();
    Q_INVOKABLE bool create();
    Q_INVOKABLE bool save();

    int tabCount() const;

    // for data modification use setForm*Value() functions
    const FormItem *formItem( const QUuid &id ) const;

    // for data modification use setTab*Value() functions;
    const TabItem *tabItem( int tabRow ) const;

    // Returns true if successful; otherwise returns false.
    bool setFormShouldRememberValue( const QUuid &id, bool shouldRememberValue );

    bool formShouldRememberValue( int fieldIndex ) const;

    // Returns true if successful; otherwise returns false.
    bool setFormValue( const QUuid &id, QVariant value );

    QVariant formValue( int fieldIndex ) const;

    RememberAttributesController *rememberAttributesController() const;
    void setRememberAttributesController( RememberAttributesController *rememberAttributes );

  signals:
    void hasAnyChangesChanged();
    void rememberAttributesChanged();
    void featureLayerPairChanged();
    void attributeTabProxyModelChanged();
    void constraintsHardValidChanged();
    void constraintsSoftValidChanged();
    void hasTabsChanged();

    void formDataChangedFailed( const QString &msg );
    void formDataChanged( QUuid uuid );
    void tabDataChanged( int id );
  private:
    void clearAll();

    void setHasAnyChanges( bool hasChanges );

    bool isValidTabId( int id ) const;
    bool isValidFormId( const QUuid &id ) const;

    bool startEditing();
    bool commit();

    void updateOnLayerChange();
    void updateOnFeatureChange();
    bool isNewFeature() const;

    // visibility & constrains & default vals
    void recalculateDerivedItems();
    bool recalculateDefaultValues( QSet<QUuid> &changedFormItems, QgsExpressionContext &context );

    // generate tab
    void createTab( QgsAttributeEditorContainer *container );

    // flatten groups and tabs within tabs/groups
    void flatten( QgsAttributeEditorContainer *container,
                  int parentTabRow,
                  const QString &parentVisibilityExpressions,
                  QVector<QUuid> &items );

    // Generates fake root tab for auto-layout
    QgsAttributeEditorContainer *autoLayoutTabContainer() const;

    bool mConstraintsHardValid = false;
    bool mConstraintsSoftValid = false;
    bool mHasAnyChanges = false;
    bool mHasTabs = false;

    FeatureLayerPair mFeatureLayerPair;
    std::unique_ptr<AttributeTabProxyModel> mAttributeTabProxyModel;
    QVector<AttributeFormProxyModel *> mAttributeFormProxyModelForTabItem;
    QMap<QUuid, std::shared_ptr<FormItem>> mFormItems; // order of fields in tab is in tab item
    QVector<std::shared_ptr<TabItem>> mTabItems; // order of tabs by tab row number

    RememberAttributesController *mRememberAttributesController = nullptr; // not owned

};
#endif // ATTRIBUTECONTROLLER_H