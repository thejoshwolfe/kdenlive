/*
Copyright (C) 2012  Till Theato <root@ttill.de>
This file is part of kdenlive. See www.kdenlive.org.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
*/

#ifndef CLIPCONTROLLER_H
#define CLIPCONTROLLER_H

#include "definitions.h"

#include <mlt++/Mlt.h>
#include <QString>
#include <QObject>
#include <QUrl>

class QPixmap;
class BinController;

/**
 * @class ClipController
 * @brief Provides a convenience wrapper around the project Bin clip producers.
 * It also holds a QList of track producers for the 'master' producer in case we
 * need to update or replace them
 */


class ClipController : public QObject , public QList<Mlt::Producer *>
{
public:
    /**
     * @brief Constructor.
     * @param bincontroller reference to the bincontroller
     * @param producer producer to create reference to
     */
    ClipController(BinController *bincontroller, Mlt::Producer &producer);
    /**
     * @brief Constructor used when opening a document and encountering a 
     * track producer before the master producer. The masterProducer MUST be set afterwards
     * @param bincontroller reference to the bincontroller
     */
    ClipController(BinController *bincontroller);
    virtual ~ClipController();
    
    /** @brief Returns true if the master producer is valid */
    bool isValid();
    
    /** @brief Replaces the master producer and (TODO) the track producers with an updated producer, for example a proxy */
    void updateProducer(Mlt::Producer *producer);
    
    /** @brief Append a track producer retrieved from document loading to out list */
    void appendTrackProducer(int track, Mlt::Producer &producer);
    
    /** @brief Returns a clone of our master producer */
    Mlt::Producer *masterProducer();
    
    /** @brief Returns the MLT's producer id */
    const QString clipId();
    
    /** @brief Returns the clip name (usually file name) */
    QString clipName() const;
    
    /** @brief Returns the clip's MLT resource */
    QUrl clipUrl() const;
    
    /** @brief Returns the clip's type as defined in definitions.h */
    ClipType clipType() const;
    
    /** @brief Returns the clip's duration */
    GenTime getPlaytime() const;
    /**
     * @brief Sets a property.
     * @param name name of the property
     * @param value the new value
     */
    void setProperty(const QString &name, const QString &value);
    void setProperty(const QString& name, int value);
    void setProperty(const QString& name, double value);
    
    /**
     * @brief Returns the value of a property.
     * @param name name o the property
     */
    QString property(const QString &name) const;
    int int_property(const QString &name) const;
    const QString getClipHash() const;

    /** @brief Returns the clip duration as a string like 00:00:02:01. */
    const QString getStringDuration();

    /**
     * @brief Returns a pixmap created from a frame of the producer.
     * @param position frame position
     * @param width width of the pixmap (only a guidance)
     * @param height height of the pixmap (only a guidance)
     */
    QPixmap pixmap(int position = 0, int width = 0, int height = 0);

    /** @brief Returns the MLT producer's service. */
    QString serviceName() const;
    
    /** @brief Returns the original master producer. */
    Mlt::Producer &originalProducer();
    
    /** @brief Returns the current profile's display aspect ratio. */
    double dar() const;
    
    /** @brief Get a clone of master producer for a specific track. Retrieve it if it already exists
     *  in our list, otherwise we create it. */
    Mlt::Producer *getTrackProducer(int track, PlaylistState::ClipState clipState = PlaylistState::Original, double speed = 1.0);
    
    /** @brief Sets the master producer for this clip when we build the controller without master clip. */
    void addMasterProducer(Mlt::Producer &producer);
    
    QList < CommentedTime > commentedSnapMarkers() const;
    GenTime findNextSnapMarker(const GenTime & currTime);
    GenTime findPreviousSnapMarker(const GenTime & currTime);
    QString deleteSnapMarker(const GenTime & time);
    void editSnapMarker(const GenTime & time, const QString &comment);
    void addSnapMarker(const CommentedTime &marker);
    QList < GenTime > snapMarkers() const;
    QString markerComment(const GenTime &t) const;
    CommentedTime markerAt(const GenTime &t) const;
    void setZone(const QPoint &zone);
    QPoint zone() const;
    bool hasLimitedDuration() const;

private:
    Mlt::Producer *m_masterProducer;
    QString m_service;
    GenTime m_duration;
    QUrl m_url;
    QString m_name;
    ClipType m_clipType;
    bool m_hasLimitedDuration;
    BinController *m_binController;
    /** A list of snap markers; these markers are added to a clips snap-to points, and are displayed as necessary. */
    QList < CommentedTime > m_snapMarkers;
    void getInfoForProducer();
};

#endif