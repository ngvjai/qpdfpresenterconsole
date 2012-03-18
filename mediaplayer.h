/* vim: set et ts=4 sw=4: */

#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QWidget>
#include <vlc/vlc.h>
#include "app.h"

#ifdef HAVE_DEBUG
#include <stdio.h>
#include <QDir>
#include <QMessageBox>
#endif

class MediaPlayer : public QObject
{
    Q_OBJECT

    bool vlc_ready;
    bool vlc_playing;
    libvlc_instance_t *vlc_instance;
    libvlc_media_t *vlc_media;
    QList<libvlc_event_manager_t*>vlc_event_managers;
    QList<libvlc_media_player_t*>vlc_media_players;
    QList<QWidget*> videoTargets;
    QWidget* targetToMute;
    QString mediaFile;

    void preparePlayer();
    void attachMediaPlayerToWidget(libvlc_media_player_t* vlc_mp, QWidget *widget);
    void detachMediaPlayerFromWidget(libvlc_media_player_t* vlc_mp);
    void muteMediaPlayer(libvlc_media_player_t* vlc_mp, QWidget *widget);

#ifdef HAVE_DEBUG
    FILE* logErr;
#endif

public:
    explicit MediaPlayer(QObject *parent = 0);
    ~MediaPlayer();
    static MediaPlayer& getInstance(QObject *parent);

    void setFile(QString file);
    void setTargetToMute(QWidget *widget);
    void pushTargetWidget(QWidget *widget);

    void play();
    void pause();
    void stop();
    void seek(float position);
    void volume(int vol);

    int getVolume();

    static void mediaEventCallback(const libvlc_event_t *ev, void *p);

signals:
    void playbackStarted();
    void playbackStopped();
    void playbackPaused();
    void playbackSeeked();
    void playbackVolumed();

    void mediaTimeChanged(qint64 milisectime);
    void mediaPositionChanged(float position);

public slots:
    void startPlayback();
    void stopPlayback();
    void pausePlayback();
    void seekPlayback(float position);
    void volumePlayback(int volume);

};

#endif // MEDIAPLAYER_H
