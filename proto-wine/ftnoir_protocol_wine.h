#pragma once

#include "ui_ftnoir_winecontrols.h"
#include <QMessageBox>
#include <QLibrary>
#include <QProcess>
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>
#include <QFile>
#include "api/plugin-api.hpp"
#include "compat/shm.h"
#include "wine-shm.h"

class FTNoIR_Protocol : public IProtocol
{
public:
    FTNoIR_Protocol();
    ~FTNoIR_Protocol() override;

    bool correct() override;
    void pose(const double* headpose) override;
    QString game_name() override {
        QMutexLocker foo(&game_name_mutex);
        return connected_game;
    }
private:
    PortableLockedShm lck_shm;
    WineSHM* shm;
    QProcess wrapper;
    int gameid;
    QString connected_game;
    QMutex game_name_mutex;
};

class FTControls: public IProtocolDialog
{
    Q_OBJECT
public:
    FTControls();
    void register_protocol(IProtocol *) override {}
    void unregister_protocol() override {}

private:
    Ui::UICFTControls ui;

private slots:
    void doOK();
    void doCancel();
};

class FTNoIR_ProtocolDll : public Metadata
{
public:
    QString name() override { return QString("Wine -- Windows layer for Unix"); }
    QIcon icon() override { return QIcon(":/images/wine.png"); }
};
