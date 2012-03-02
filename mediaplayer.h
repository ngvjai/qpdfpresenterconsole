#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QWidget>
#include <vlc/vlc.h>

class MediaPlayer : public QObject
{
    Q_OBJECT

    bool vlc_ready;
    bool vlc_playing;
    libvlc_instance_t *vlc_instance;
    libvlc_media_player_t *vlc_media_player;
    libvlc_media_t *vlc_media;

    QList<QWidget*> widgets;

    void preparePlayer();

public:
    explicit MediaPlayer(QObject *parent = 0);
    ~MediaPlayer();
    static MediaPlayer& getInstance(QObject *parent);

    void setFile(QString file);
    void pushTargetWidget(QWidget* widget);

    void play();
    void pause();
    void stop();

signals:
    void playbackStarted();
    void playbackStopped();
    void playbackPaused();

public slots:
    void startPlayback();
    void stopPlayback();
    void pausePlayback();

};

#endif // MEDIAPLAYER_H
