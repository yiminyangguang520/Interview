#pragma once

#include <QHash>
#include <QObject>
#include <QStringList>

class GLDMaskBox;

class GLDMaskBoxFactory : public QObject
{
    Q_OBJECT

public:
    static void initialize(const QString& xmlPath);

protected:
    GLDMaskBoxFactory(QObject* parent = nullptr);
    virtual ~GLDMaskBoxFactory();

private:
    QStringList parseIniFile(const QString& iniPath);

private:
    QHash<QString, GLDMaskBox> m_maskBoxHash;
};