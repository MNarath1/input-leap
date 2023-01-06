/*
 * InputLeap -- mouse and keyboard sharing utility
 * Copyright (C) 2012-2016 Symless Ltd.
 * Copyright (C) 2008 Volker Lanz (vl@fidra.de)
 *
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file LICENSE that should have accompanied this file.
 *
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "QBarrierApplication.h"
#include "MainWindow.h"

#include <QtCore>
#include <QtGui>

QBarrierApplication* QBarrierApplication::s_Instance = nullptr;

QBarrierApplication::QBarrierApplication(int& argc, char** argv) :
    QApplication(argc, argv)
{
    s_Instance = this;
}

QBarrierApplication::~QBarrierApplication() = default;

void QBarrierApplication::commitData(QSessionManager&)
{
    for (QWidget* widget : topLevelWidgets()) {
        MainWindow* mainWindow = qobject_cast<MainWindow*>(widget);
        if (mainWindow)
            mainWindow->saveSettings();
    }
}

QBarrierApplication* QBarrierApplication::getInstance()
{
    return s_Instance;
}

void QBarrierApplication::switchTranslator(QString lang)
{
    if (translator_) {
        removeTranslator(translator_.get());
        translator_.reset();
    }

    QResource locale(":/res/lang/gui_" + lang + ".qm");
    translator_ = std::make_unique<QTranslator>();
    translator_->load(locale.data(), locale.size());
    installTranslator(translator_.get());
}

void QBarrierApplication::setTranslator(QTranslator* translator)
{
    translator_.reset(translator);
    installTranslator(translator_.get());
}
