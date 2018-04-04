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

#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(const QApplication &app, QWidget *parent)
    : QMainWindow(parent, 0)
    , m_ui(new Ui::MainWindow)
    , m_saved(true)
    , m_filename("")
{
    m_ui->setupUi(this);
    
    m_ui->attackWidget->setText("Attack");
    m_ui->decayWidget->setText("Decay");
    m_ui->sustainWidget->setText("Sustain");
    m_ui->releaseWidget->setText("Release");
    
    m_ui->widget->setText("Freq");
    
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    m_timer->start(1000./60.);
    
    reset();
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::updateShader()
{
    QListIterator<ParamEdit *> i(m_param_edits);
    while(i.hasNext())
        delete i.next();
    m_param_edits.clear();
    
    QList<int> contained_param_numbers;
    QString filter_string = m_ui->lineEdit->text();
    for(int i=0; i<filter_string.size(); ++i)
    {
        if(filter_string[i] == QChar('p'))
        {
            QString numstr = "";
            for(int d=1;i+d<filter_string.size();++d)
            {
                if(!filter_string[i+d].isDigit())break;
                numstr += filter_string[i+d];
            }
            
            if(numstr != "")
                contained_param_numbers.push_back(numstr.toInt());
        }
    }
    
    m_shader_source 
        = QString("#version 130\n")
        + QString("uniform float iAttack;")
        + QString("uniform float iDecay;")
        + QString("uniform float iSustain;")
        + QString("uniform float iRelease;")
        ;
    
    for(int i=0; i<contained_param_numbers.size(); ++i)
    {
        m_shader_source += QString("uniform float p") + QString::number(i) + QString(";");
    }
        
    m_shader_source = m_shader_source
        + QString("float th(float xlower, float xupper, float x)")
        + QString("{")
        + QString("return step(xlower,x)*step(x,xupper);")
        + QString("}")
        + QString("float adsr(float a, float d, float s, float r, float x, float xoff)")
        + QString("{")
        + QString("return th(0., a, x)*mix(0., 1., x/a)")
        + QString("+th(a, a+d, x)*mix(1., s, (x-a)/d)")
        + QString("+ th(a+d, xoff, x)*s")
        + QString("+ th(xoff, xoff+r, x)*mix(s, 0., (x-xoff)/r);")
        + QString("}")
        + QString("vec2 mainSound( float time )")
        + QString("{")
        + QString("float t1 = mod(time, .4);")
        + QString("float env = adsr(.1, .1, .8, .1, t1, .25);")
        + QString("float wave = 0.;")
        + QString("for(int i=0; i<100; ++i)")
        + QString("{")
        + QString("ret += exp(-5.e-1*float(i))");
    if(m_ui->radioButton->isChecked())
        m_shader_source += QString("*sin(440.*(t1-float(i)*1.e-2));");
    else if(m_ui->radioButton_2->isChecked())
        m_shader_source += QString("*floor(sin(440.*(t1-float(i)*1.e-2)));");
    else if(m_ui->radioButton_3->isChecked())
        m_shader_source += QString("*fract(sin(440.*(t1-float(i)*1.e-2)));");
    else if(m_ui->radioButton_4->isChecked())
        m_shader_source += QString("*fract(sin(dot(vec2(t1) ,vec2(12.9898,78.233)))*43758.5453)");
    m_shader_source = m_shader_source
        + QString("}")
        + QString("return vec2(  wave * env );")
        + QString("}")
        + QString("void main()")
        + QString("{")
        + QString("float t = iBlockOffset + ((gl_FragCoord.x-0.5) + (gl_FragCoord.y-0.5)*512.0)/iSampleRate;")
        + QString("vec2 y = iVolume * mainSound( t );")
        + QString("vec2 v  = floor((0.5+0.5*y)*65536.0);")
        + QString("vec2 vl = mod(v,256.0)/255.0;")
        + QString("vec2 vh = floor(v/256.0)/255.0;")
        + QString("gl_FragColor = vec4(vl.x,vh.x,vl.y,vh.y);")
        + QString("}")
        ;
 
    for(int i=0; i<contained_param_numbers.size(); ++i)
    {
        ParamEdit *edit = new ParamEdit(this);
        edit->setText(QString("p")+QString::number(contained_param_numbers[i]));
        m_param_edits.push_back(edit);
        m_ui->verticalLayout_6->addWidget(edit);
        m_ui->verticalLayout_6->update();
    }
    
    m_saved = false;
}

void MainWindow::tick()
{
}

void MainWindow::shaderExport()
{
    QString filename = QFileDialog::getSaveFileName(this, "Export...", "", "Fragment shader source (*.frag)");
    
    QFile f(filename);
    if(!f.open(QIODevice::WriteOnly))
        QMessageBox::critical(this, QString("Could not save file") + filename, f.errorString());
        
    QTextStream fo(&f);
    fo << m_shader_source;
    f.close();
}

void MainWindow::fileNew()
{
    if(!m_saved)
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "New without saving?", "Tool contains unsaved work. Save?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(reply == QMessageBox::Yes)
        {
            fileSave();
            reset();
        }
        else if(reply == QMessageBox::No)
            reset();
    }
    else reset();
}

void MainWindow::fileOpen()
{
    if(!m_saved)
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Open without saving?", "Tool contains unsaved work. Save?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(reply == QMessageBox::Yes)
        {
            fileSave();
            open();
        }
        else if(reply == QMessageBox::No)
            open();
    }
    else open();
}

void MainWindow::startPlay()
{
    
}

void MainWindow::fileExit()
{
    if(!m_saved)
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Exit without saving?", "Tool contains unsaved work. Save?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(reply == QMessageBox::Yes)
        {
            fileSave();
            close();
        }
        else if(reply == QMessageBox::No)
            close();
    }
    else close();
}

void MainWindow::fileSave()
{
    if(!m_saved)
    {
        if(m_filename.isEmpty())
        {
            fileSaveAs();
        }
        
        QFile f(m_filename);
        if(!f.open(QIODevice::WriteOnly))
            QMessageBox::critical(this, QString("Could not save file") + m_filename, f.errorString());
            
        QDataStream fo(&f);
        
        //adsr envelope / double
        fo << m_ui->attackWidget->lowerLimit() << m_ui->attackWidget->upperLimit() << m_ui->attackWidget->value();
        fo << m_ui->decayWidget->lowerLimit() << m_ui->decayWidget->upperLimit() << m_ui->decayWidget->value();
        fo << m_ui->sustainWidget->lowerLimit() << m_ui->sustainWidget->upperLimit() << m_ui->sustainWidget->value();
        fo << m_ui->releaseWidget->lowerLimit() << m_ui->releaseWidget->upperLimit() << m_ui->releaseWidget->value();
        
        //wave form / bool
        fo << m_ui->radioButton->isChecked() << m_ui->radioButton_2->isChecked() << m_ui->radioButton_3->isChecked() << m_ui->radioButton_4->isChecked();
        
        //playback settings / int 
        fo << m_ui->comboBox->currentIndex();
        
        //fir scaling function / QString
        fo << m_ui->lineEdit->text();
        
        //fir parameters: until end; double
        for(int i=0; i<m_param_edits.size(); ++i)
        {
            fo << m_param_edits[i]->lowerLimit() << m_param_edits[i]->upperLimit() << m_param_edits[i]->value();
        }
        
        f.close();
        
        m_saved = true;
    }
}

void MainWindow::fileSaveAs()
{
    m_filename = QFileDialog::getSaveFileName(this, "Save As...", "", "Binary (*)");
    
}

void MainWindow::setSaved(bool saved)
{
    m_saved = saved;
}

void MainWindow::reset()
{
    m_saved = true;
    m_filename = "";
    
    m_ui->radioButton->setChecked(true);
    
    m_ui->comboBox->setCurrentIndex(0);
    
    m_ui->attackWidget->setLowerLimit(0.);
    m_ui->attackWidget->setUpperLimit(100.);
    m_ui->attackWidget->setValue(0.);
    
    m_ui->decayWidget->setLowerLimit(0.);
    m_ui->decayWidget->setUpperLimit(100.);
    m_ui->decayWidget->setValue(0.);
    
    m_ui->sustainWidget->setLowerLimit(0.);
    m_ui->sustainWidget->setUpperLimit(100.);
    m_ui->sustainWidget->setValue(0.);
    
    m_ui->releaseWidget->setLowerLimit(0.);
    m_ui->releaseWidget->setUpperLimit(100.);
    m_ui->releaseWidget->setValue(0.);
    
    m_ui->lineEdit->setText("exp(-p0*t)");
    updateShader();
}

void MainWindow::open()
{
    m_filename = QFileDialog::getOpenFileName(this, "Open...", "", "Binary (*)");
    
    QFile f(m_filename);
    if(!f.open(QIODevice::WriteOnly))
        QMessageBox::critical(this, QString("Could not open file") + m_filename, f.errorString());
        
    QDataStream fo(&f);
    
    //adsr envelope / double
    fo << m_ui->attackWidget->lowerLimit() << m_ui->attackWidget->upperLimit() << m_ui->attackWidget->value();
    fo << m_ui->decayWidget->lowerLimit() << m_ui->decayWidget->upperLimit() << m_ui->decayWidget->value();
    fo << m_ui->sustainWidget->lowerLimit() << m_ui->sustainWidget->upperLimit() << m_ui->sustainWidget->value();
    fo << m_ui->releaseWidget->lowerLimit() << m_ui->releaseWidget->upperLimit() << m_ui->releaseWidget->value();
    
    //wave form / bool
    fo << m_ui->radioButton->isChecked() << m_ui->radioButton_2->isChecked() << m_ui->radioButton_3->isChecked() << m_ui->radioButton_4->isChecked();
    
    //playback settings / int 
    fo << m_ui->comboBox->currentIndex();
    
    //fir scaling function / QString
    fo << m_ui->lineEdit->text();
    
    //fir parameters: until end; double
    for(int i=0; i<m_param_edits.size(); ++i)
    {
        fo << m_param_edits[i]->lowerLimit() << m_param_edits[i]->upperLimit() << m_param_edits[i]->value();
    }
    
    f.close();
    
    m_saved = true;
}
