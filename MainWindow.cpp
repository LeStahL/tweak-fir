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

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(const QApplication &app, QWidget *parent)
    : QMainWindow(parent, 0)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    
    m_ui->attackWidget->setText("Attack");
    m_ui->decayWidget->setText("Decay");
    m_ui->sustainWidget->setText("Sustain");
    m_ui->releaseWidget->setText("Release");
}

MainWindow::~MainWindow()
{
    delete m_ui;
}
