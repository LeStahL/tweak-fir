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
#include "MainWindow.h"

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
    
    ((MainWindow *)parent())->setSaved(false);
}

void ParamEdit::knobTurned()
{
    m_ui->doubleSpinBox->setValue(m_ui->dial->value());
    
    ((MainWindow *)parent())->setSaved(false);
}

void ParamEdit::updateControls()
{
    m_ui->dial->setMinimum(m_ui->lineEdit->text().toDouble());
    m_ui->dial->setMaximum(m_ui->lineEdit_2->text().toDouble());
    m_ui->dial->setSingleStep((m_ui->lineEdit_2->text().toDouble()-m_ui->lineEdit->text().toDouble())/100.);
    
    m_ui->doubleSpinBox->setMinimum(m_ui->lineEdit->text().toDouble());
    m_ui->doubleSpinBox->setMaximum(m_ui->lineEdit_2->text().toDouble());
    m_ui->doubleSpinBox->setSingleStep((m_ui->lineEdit_2->text().toDouble()-m_ui->lineEdit->text().toDouble())/100.);
}

void ParamEdit::lowChanged()
{
    updateControls();
    
    ((MainWindow *)parent())->setSaved(false);
}

void ParamEdit::highChanged()
{
    updateControls();
    
    ((MainWindow *)parent())->setSaved(false);
}

void ParamEdit::setText(QString text)
{
    m_ui->label->setText(text);
}

QString ParamEdit::text()
{
    return m_ui->label->text();
}

void ParamEdit::setValue(double value)
{
    m_ui->doubleSpinBox->setValue(value);
    m_ui->dial->setValue(value);
}

double ParamEdit::value()
{
    return m_ui->doubleSpinBox->value();
}

double ParamEdit::lowerLimit()
{
    return m_ui->lineEdit->text().toDouble();
}

double ParamEdit::upperLimit()
{
    return m_ui->lineEdit->text().toDouble();
}

void ParamEdit::setLowerLimit(double val)
{
    m_ui->lineEdit->setText(QString::number(val));
    updateControls();
}

void ParamEdit::setUpperLimit(double val)
{
    m_ui->lineEdit_2->setText(QString::number(val));
    updateControls();
}
