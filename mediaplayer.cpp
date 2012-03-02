#include "mediaplayer.h"
#include <iostream>

MediaPlayer::MediaPlayer(QObject *parent) :
    QObject(parent)
{
    this->vlc_playing = false;
    this->vlc_ready = false;
}

MediaPlayer::~MediaPlayer()
{
    // libvlc_media_player_release(this->vlc_media_player);
    // libvlc_release(this->vlc_instance);
}

MediaPlayer& MediaPlayer::getInstance(QObject *parent)
{
    static MediaPlayer instance(parent);
    return instance;
}

void MediaPlayer::setFile(QString file)
{
    if (!file.isEmpty()) {
        this->mediaFile = file;
    }
}

void MediaPlayer::pushTargetWidget(QWidget *widget)
{
    if (!this->videoTargets.contains(widget)) {
        this->videoTargets.append(widget);
    }
}

void MediaPlayer::preparePlayer()
{
#ifdef Q_WS_WIN
    libvlc_media_player_set_drawable(
                this->vlc_media_player,
                reinterpret_cast<unsigned int>(this->targetWidget->winId()));
#endif
#ifdef Q_WS_MAC
    libvlc_media_player_set_drawable(
                this->vlc_media_player,
                this->targetWidget->winId());
#endif
#ifdef Q_WS_X11
    QString win1 = QString("%1").arg(this->videoTargets[0]->winId());
    QString win2 = QString("%1").arg(this->videoTargets[1]->winId());
    QString clone = QString("--vout=clone{vout=x11{xid=%1},vout=x11{xid=%2}}").arg(win1, win2);
#endif

    const char *vlc_argv[] = {
        "--vout-filter=clone",
        "--clone-count=2",
        clone.toStdString().c_str(),
    };

    std::cerr << clone.toStdString().c_str() << std::endl;

    this->vlc_instance = libvlc_new(sizeof(vlc_argv) / sizeof(*vlc_argv), vlc_argv);
    this->vlc_media = libvlc_media_new_path(this->vlc_instance, this->mediaFile.toStdString().c_str());
    this->vlc_media_player = libvlc_media_player_new_from_media(this->vlc_media);
    libvlc_media_release(this->vlc_media);
}

void MediaPlayer::startPlayback()
{
    this->play();
}

void MediaPlayer::stopPlayback()
{
    this->stop();
}

void MediaPlayer::pausePlayback()
{
    this->pause();
}

void MediaPlayer::play()
{
    if (!this->vlc_ready) {
        this->preparePlayer();
    }

    if (!this->vlc_playing) {
        libvlc_media_player_play(this->vlc_media_player);
        this->vlc_playing = true;
        emit playbackStarted();
    }
}

void MediaPlayer::pause()
{
    libvlc_media_player_pause(this->vlc_media_player);
    this->vlc_playing = true;
    emit playbackPaused();
}

void MediaPlayer::stop()
{
    libvlc_media_player_stop(this->vlc_media_player);
    this->vlc_playing = false;
    emit playbackStopped();
}
