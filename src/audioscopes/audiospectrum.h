/***************************************************************************
 *   Copyright (C) 2010 by Simon Andreas Eugster (simon.eu@gmail.com)      *
 *   This file is part of kdenlive. See www.kdenlive.org.                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

/**
   Displays a spectral power distribution of audio samples.
   The frequency distribution is calculated by means of a Fast Fourier Transformation.
   For more information see Wikipedia:FFT and the code comments.
*/

#ifndef AUDIOSPECTRUM_H
#define AUDIOSPECTRUM_H

#include <QtCore>
#include <QVector>
#include <QHash>

#include "abstractaudioscopewidget.h"
#include "ui_audiospectrum_ui.h"
#include "tools/kiss_fftr.h"

class AudioSpectrum_UI;
class AudioSpectrum : public AbstractAudioScopeWidget {
    Q_OBJECT

public:
    AudioSpectrum(QWidget *parent = 0);
    ~AudioSpectrum();

    // Implemented virtual methods
    QString widgetName() const;

    static const QString directions[]; // Mainly for debug output
    enum RescaleDirection { North, Northeast, East, Southeast };
    enum RescaleDimension { Min_dB, Max_dB, Max_Hz };


protected:
    ///// Implemented methods /////
    QRect scopeRect();
    QImage renderHUD(uint accelerationFactor);
    QImage renderAudioScope(uint accelerationFactor, const QVector<int16_t> audioFrame, const int freq, const int num_channels, const int num_samples);
    QImage renderBackground(uint accelerationFactor);
    bool isHUDDependingOnInput() const;
    bool isScopeDependingOnInput() const;
    bool isBackgroundDependingOnInput() const;
    virtual void readConfig();
    void writeConfig();

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::AudioSpectrum_UI *ui;
    QHash<QString, kiss_fftr_cfg> m_fftCfgs; // FFT cfg cache
    QHash<QString, QVector<float> > m_windowFunctions; // Window function cache

    QAction *m_aResetHz;

    /** Contains the plot only; m_scopeRect contains text and widgets as well */
    QRect m_innerScopeRect;

    /** Lower bound for the dB value to display */
    int m_dBmin;
    /** Upper bound (max: 0) */
    int m_dBmax;

    /** Maximum frequency (limited by the sampling rate if determined automatically).
        Stored for the painters. */
    uint m_freqMax;
    /** The user has chosen a custom frequency. */
    bool m_customFreq;


    /** Returns a signature for a kiss_fft configuration
        used as a hash in the cache */
    static const QString cfgSignature(const int size);


    ///// Movement detection /////
    const int m_rescaleMinDist;
    const float m_rescaleVerticalThreshold;

    bool m_rescaleActive;
    bool m_rescalePropertiesLocked;
    bool m_rescaleFirstRescaleDone;
    short m_rescaleScale;
    Qt::KeyboardModifiers m_rescaleModifiers;
    AudioSpectrum::RescaleDirection m_rescaleClockDirection;
    QPoint m_rescaleStartPoint;



private slots:
    void slotResetMaxFreq();

};

#endif // AUDIOSPECTRUM_H
