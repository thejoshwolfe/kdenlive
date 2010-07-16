/***************************************************************************
 *   Copyright (C) 2010 by Simon Andreas Eugster (simon.eu@gmail.com)      *
 *   This file is part of kdenlive. See www.kdenlive.org.                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "colorplaneexport.h"
#include <QDebug>
#include <KMessageBox>

const QString EXTENSION_PNG = ".png";
const int SCALE_RANGE = 80;

ColorPlaneExport::ColorPlaneExport(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    m_colorTools = new ColorTools();

    tResX->setText("800");
    tResY->setText("800");

    cbColorspace->addItem(i18n("YUV"), QVariant(CPE_YUV));
    cbColorspace->addItem(i18n("Modified YUV (Chroma)"), QVariant(CPE_YUV_MOD));

    sliderColor->setSliderPosition(128);

    // 0  -> 1
    // 50 -> 0.5
    // 80 -> 0.2
    sliderScaling->setInvertedAppearance(true);
    sliderScaling->setRange(0, 80);
    sliderScaling->setSliderPosition(50);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(slotExportPlane()));
    connect(tResX, SIGNAL(textChanged(QString)), this, SLOT(slotValidate()));
    connect(tResY, SIGNAL(textChanged(QString)), this, SLOT(slotValidate()));
    connect(kurlrequester, SIGNAL(textChanged(QString)), this, SLOT(slotValidate()));
    connect(sliderColor, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateDisplays()));
    connect(sliderScaling, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateDisplays()));

    kurlrequester->setText("/tmp/yuv-plane.png");

    slotColormodeChanged();
    slotValidate();
}

ColorPlaneExport::~ColorPlaneExport()
{
    delete m_colorTools;
}

void ColorPlaneExport::slotUpdateDisplays()
{
    m_scaling = 1 - (float)sliderScaling->value()/100;

    lblScaleNr->setText("0..." + QString::number(m_scaling, 'f', 2));
    lblColNr->setText(QString::number(sliderColor->value()));

    lblSize->setText(i18n("%1 px", QVariant(tResX->text()).toInt()*QVariant(tResY->text()).toInt()));
}

void ColorPlaneExport::slotValidate()
{
    bool ok;
    int nr;

    nr = QVariant(tResX->text()).toInt(&ok);
    ok = ok && nr > 0;
    if (ok) {
        nr = QVariant(tResY->text()).toInt(&ok);
        ok = ok && nr > 0;
    }
    if (ok) {
        ok = kurlrequester->text().trimmed().length() > 0;
        qDebug() << "File given: " << ok;
    }

    if (ok) {
        buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    } else {
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel);
    }

    slotUpdateDisplays();
}

void ColorPlaneExport::slotExportPlane()
{
    qDebug() << "Exporting plane now to " <<  kurlrequester->text();
    QString lower = kurlrequester->text().toLower();
    qDebug() << "Lower: " << lower;
    if (!lower.endsWith(".png") && !lower.endsWith(".jpg") && !lower.endsWith(".tif") && !lower.endsWith(".tiff")) {
        if (KMessageBox::questionYesNo(this, i18n("File has no extension. Add extension (%1)?", EXTENSION_PNG)) == KMessageBox::Yes) {
            kurlrequester->setText(kurlrequester->text() + ".png");
        }
    }
    QImage img;
    QSize size(QVariant(tResX->text()).toInt(), QVariant(tResY->text()).toInt());
    switch (cbColorspace->itemData(cbColorspace->currentIndex()).toInt()) {
    case CPE_YUV:
        img = m_colorTools->yuvColorWheel(size, sliderColor->value(), m_scaling, false, false);
        break;
    case CPE_YUV_MOD:
        img = m_colorTools->yuvColorWheel(size, sliderColor->value(), m_scaling, true, false);
        break;
    }
    img.save(kurlrequester->text());
}

void ColorPlaneExport::slotColormodeChanged()
{
    switch (cbColorspace->itemData(cbColorspace->currentIndex()).toInt()) {
    case CPE_YUV:
    case CPE_YUV_MOD:
        sliderColor->setVisible(true);
        sliderColor->setRange(0,255);
        break;
    default:
        sliderColor->setVisible(false);
        break;
    }
}
