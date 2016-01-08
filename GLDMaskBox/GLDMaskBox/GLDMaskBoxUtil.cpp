#include "GLDMaskBoxUtil.h"
#include "GLDMaskBoxInfo.h"

#include <assert.h>

using namespace std;

namespace GlodonMask
{
    static shared_ptr<GLDMaskBoxInfo> g_GLDMaskBoxFactory;

    bool WINAPI initialize(const QString& xmlPath)
    {
        assert(!g_GLDMaskBoxFactory);
        g_GLDMaskBoxFactory.reset(new GLDMaskBoxInfo(xmlPath));
        return true;
    }

    bool WINAPI unInitialize()
    {
        g_GLDMaskBoxFactory->writeMaskBoxIDToFile();
        return true;
    }

    int WINAPI showMasks(const QString& id, QList<QWidget*> &wgtList)
    {
        g_GLDMaskBoxFactory->showMasks(id, wgtList);
        return 1;
    }

    bool WINAPI setMaskBoxColor(const QString & id, GLDMask::MASKCOLOR color)
    {
        g_GLDMaskBoxFactory->setMaskBoxColor(id, color);
        return true;
    }

    bool WINAPI setMaskBoxArrowColor(const QString& id, const QColor& color)
    {
        g_GLDMaskBoxFactory->setMaskBoxArrowColor(id, color);
        return true;
    }

    bool WINAPI setMaskArrowLineWidth(const QString& id, const int lineWidth)
    {
        g_GLDMaskBoxFactory->setMaskArrowLineWidth(id, lineWidth);
        return true;
    }
}