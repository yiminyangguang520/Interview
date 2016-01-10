#include "GLDMaskBoxUtil.h"
#include "GLDMaskBoxInfo.h"

#include <assert.h>

using namespace std;

namespace GlodonMask
{
    static shared_ptr<GLDMaskBoxInfo> g_GLDMaskBoxInfo;

    bool WINAPI initialize(const QString& xmlPath)
    {
        if (xmlPath == "")
        {
            return false;
        }

        assert(!g_GLDMaskBoxInfo);
        g_GLDMaskBoxInfo.reset(new GLDMaskBoxInfo(xmlPath));
        return true;
    }

    bool WINAPI unInitialize()
    {
        assert(!g_GLDMaskBoxInfo);
        g_GLDMaskBoxInfo->writeMaskBoxIDToFile();
        return true;
    }

    STATUS WINAPI showMasks(const QString& id, QList<QWidget*> &wgtList)
    {
        // todo ����״̬��
        assert(!g_GLDMaskBoxInfo);
        return g_GLDMaskBoxInfo->showMasks(id, wgtList);
    }

    STATUS WINAPI showMasks(const QString& id, QList<QAction*> &wgtList)
    {
        assert(!g_GLDMaskBoxInfo);
        return g_GLDMaskBoxInfo->showMasks(id, wgtList);
    }

    bool WINAPI setMaskBoxColor(const QString & id, GLDMask::MASKCOLOR color)
    {
        assert(!g_GLDMaskBoxInfo);
        return g_GLDMaskBoxInfo->setMaskBoxColor(id, color);
    }

    bool WINAPI setMaskBoxArrowColor(const QString& id, const QColor& color)
    {
        assert(!g_GLDMaskBoxInfo);
        return g_GLDMaskBoxInfo->setMaskBoxArrowColor(id, color);
    }

    bool WINAPI setMaskArrowLineWidth(const QString& id, const int lineWidth)
    {
        assert(!g_GLDMaskBoxInfo);
        return g_GLDMaskBoxInfo->setMaskArrowLineWidth(id, lineWidth);
    }
}