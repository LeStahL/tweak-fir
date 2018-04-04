/*
 * tweak-fir - QT tool to tweak shader filter parameters for sound rendering
 * 
 * Copyright (C) 2018  Alexander Kraus <nr4@z10.info>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qt5/QtWidgets/QMainWindow>
#include <qt5/QtWidgets/QWidget>
#include <qt5/QtCore/QList>
#include <qt5/QtCore/QTimer>

#include "ParamEdit.h"

namespace Ui {class MainWindow;}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(const QApplication &app, QWidget *parent=0);
    virtual ~MainWindow();
    
    void setSaved(bool saved);
    
private slots:
    void updateShader();
    void tick();
    void fileNew();
    void fileOpen();
    void startPlay();
    void fileExit();
    void fileSave();
    void fileSaveAs();
    void shaderExport();
    
private:
    Ui::MainWindow *m_ui;
    QList<ParamEdit *> m_param_edits;
    QString m_shader_source;
    QTimer *m_timer;
    bool m_saved;
    QString m_filename;
    
    void reset();
    void open();
};

#endif
