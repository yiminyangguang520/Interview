#ifndef GLDMASKBOXFACTORY_H
#define GLDMASKBOXFACTORY_H

#include "GLDMaskBox.h"

#include <windows.h>

#include <QHash>
#include <QStringList>
#include <QtXml/QDomElement>

namespace GlodonMask
{
    GLDMASKBOX_EXPORT bool WINAPI initialize(const QString& xmlPath);

    GLDMASKBOX_EXPORT int WINAPI showMasks(const QString& id, QList<QWidget*> &wgtList);

    GLDMASKBOX_EXPORT int WINAPI showMasks(const QString& id, QList<QAction*> &actList);

    GLDMASKBOX_EXPORT bool WINAPI setMaskBoxColor(const QString& id, GLDMask::MASKCOLOR color);

    GLDMASKBOX_EXPORT bool WINAPI setMaskBoxArrowColor(const QString& id, const QColor& color);

    GLDMASKBOX_EXPORT bool WINAPI setMaskArrowLineWidth(const QString& id, const int lineWidth);

    GLDMASKBOX_EXPORT bool WINAPI unInitialize();

    class GLDMaskBoxFactory
    {
    public:
        GLDMaskBoxFactory(const QString& xmlPath);
        ~GLDMaskBoxFactory();

    public:
        void showMasks(const QString& id, QList<QWidget*> &wgtList);
        void showMasks(const QString& id, QList<QAction*> &actList);

        void setMaskBoxColor(const QString& id, GLDMask::MASKCOLOR color);
        void setMaskBoxArrowColor(const QString& id, const QColor& color);
        void setMaskArrowLineWidth(const QString& id, const int lineWidth);

        void writeMaskBoxIDToFile();

    private:
        class InnerMaskBoxFactoryImpl;
        QScopedPointer<InnerMaskBoxFactoryImpl> d;
    };
}

#endif // GLDMASKBOXFACTORY_H