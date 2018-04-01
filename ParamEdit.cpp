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

#include "ParamEdit.h"
#include "ui_ParamEdit.h"

ParamEdit::ParamEdit(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::ParamEdit)
{
    m_ui->setupUi(this);
}

ParamEdit::~ParamEdit()
{
    delete m_ui;
}

void ParamEdit::valueChanged()
{
    m_ui->dial->setValue(m_ui->doubleSpinBox->value());
}

void ParamEdit::knobTurned()
{
    m_ui->doubleSpinBox->setValue(m_ui->dial->value());
}

void ParamEdit::lowChanged()
{
    m_ui->dial->setMinimum(m_ui->lineEdit->text().toDouble());
    m_ui->dial->setSingleStep((m_ui->lineEdit_2->text().toDouble()-m_ui->lineEdit->text().toDouble())/100.);
    
    m_ui->doubleSpinBox->setMinimum(m_ui->lineEdit->text().toDouble());
    m_ui->doubleSpinBox->setSingleStep((m_ui->lineEdit_2->text().toDouble()-m_ui->lineEdit->text().toDouble())/100.);
}

void ParamEdit::highChanged()
{
    m_ui->dial->setMaximum(m_ui->lineEdit_2->text().toDouble());
    m_ui->dial->setSingleStep((m_ui->lineEdit_2->text().toDouble()-m_ui->lineEdit->text().toDouble())/100.);
    
    m_ui->doubleSpinBox->setMaximum(m_ui->lineEdit_2->text().toDouble());
    m_ui->doubleSpinBox->setSingleStep((m_ui->lineEdit_2->text().toDouble()-m_ui->lineEdit->text().toDouble())/100.);
}
