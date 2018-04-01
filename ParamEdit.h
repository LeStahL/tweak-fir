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

#ifndef PARAM_EDIT_H
#define PARAM_EDIT_H

#include <qt5/QtWidgets/QWidget>
#include <qt5/QtCore/QString>

namespace Ui { class ParamEdit; }

class ParamEdit : public QWidget
{
    Q_OBJECT
    
public:
    ParamEdit(QWidget *parent = 0);
    virtual ~ParamEdit();
    
    void setText(QString text);
    QString text();
    
    void setValue(double val);
    double value();
    
private slots:
    void valueChanged();
    void knobTurned();
    void lowChanged();
    void highChanged();
    
private:
    Ui::ParamEdit *m_ui;
};

#endif
