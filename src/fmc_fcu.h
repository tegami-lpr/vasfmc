///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2005-2007 Alexander Wemmer 
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
///////////////////////////////////////////////////////////////////////////////

/*! \file    fmc_fcu.h
    \author  Alexander Wemmer, alex@wemmer.at
*/

#ifndef __FMC_FCU_H__
#define __FMC_FCU_H__

#include <QWidget>
#include <QPoint>
#include <QTime>
#include <QTimer>

#include "defines.h"
#include "config.h"

#include "fmc_fcu_defines.h"
#include "vas_widget.h"

class ConfigWidgetProvider;
class FMCControl;
class FMCMessage;

/////////////////////////////////////////////////////////////////////////////

class FMCFCUStyleBase : public VasWidget
{
    Q_OBJECT

public:
	
    FMCFCUStyleBase(const QString& style,
                    ConfigWidgetProvider* config_widget_provider,
                    Config* main_config,
                    const QString& fcu_config_filename,
                    FMCControl* fmc_control,
                    QWidget* parent,
                    Qt::WindowFlags fl);
       
    ~FMCFCUStyleBase() override;

    inline Config* config() { return m_fcu_config; }

    void incFontSize();
    void decFontSize();

signals:

    void signalRestart();

    void signalTimeUsed(const QString& name, uint millisecs);

public slots:

    virtual void slotRefresh() = 0;

    void slotProcessInput();

    //! Slot for receiving messages from message bus
    virtual void ReceiveMessage(FMCMessage *message);

protected:

    void setupDefaultConfig(const QString& style);
    void loadWindowGeometry();
    void saveWindowGeometry();
    void keyPressEvent(QKeyEvent *event) override = 0;

protected:

    ConfigWidgetProvider* m_config_widget_provider;
    Config* m_main_config;
    Config* m_fcu_config;
    FMCControl* m_fmc_control;

private:

    //! Hidden copy-constructor
    FMCFCUStyleBase(const FMCFCUStyleBase&);
    //! Hidden assignment operator
    const FMCFCUStyleBase& operator = (const FMCFCUStyleBase&);
};

/////////////////////////////////////////////////////////////////////////////

//! FMC fcu handler (restarter)
class FMCFCUHandler : public QObject
{
    Q_OBJECT

public:
    //! Standard Constructor
    FMCFCUHandler(ConfigWidgetProvider* config_widget_provider,
                  Config* main_config,
                  const QString& fcu_config_filename,
                  FMCControl* fmc_control);
    
    //! Destructor
    ~FMCFCUHandler() override
    {
        delete m_fcu;
    }
    
    inline void show() { if (m_fcu != nullptr) m_fcu->show(); }
    inline void hide() { if (m_fcu != nullptr) m_fcu->hide(); }

    inline bool isVisible()
    {
        if (m_fcu == nullptr) return false;
        return m_fcu->isVisible(); 
    }

    inline void close() { if (m_fcu != 0) m_fcu->close(); }

    FMCFCUStyleBase *fmcFcuBase()
    {
#if VASFMC_GAUGE
        // Protect m_fcu from being accessed during a restart (when it might be invalid)
        QMutexLocker locker(&m_fcu_mutex);
#endif
        return m_fcu;
    }

public slots:

    void slotRestartFCU();

    //! Slot for receiving messages from message bus
    void ReceiveMessage(FMCMessage *message);

protected slots:

    void slotTriggerFcuRestart()
    {
        QMetaObject::invokeMethod(this, &FMCFCUHandler::slotRestartFCU, Qt::QueuedConnection);
    }

protected:

    FMCFCUStyleBase* createFcu();

protected:

    ConfigWidgetProvider* m_config_widget_provider;
    Config* m_main_config;
    QString m_fcu_config_filename;
    FMCControl* m_fmc_control;
    FMCFCUStyleBase* m_fcu;
#if VASFMC_GAUGE
    QMutex m_fcu_mutex;
#endif

private:
    //! Hidden copy-constructor
    FMCFCUHandler(const FMCFCUHandler&);
    //! Hidden assignment operator
    const FMCFCUHandler& operator = (const FMCFCUHandler&);
};

#endif /* __FMC_FCU_H__ */

// End of file

