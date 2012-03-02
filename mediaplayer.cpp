#include "mediaplayer.h"

MediaPlayer::MediaPlayer(QObject *parent) :
    QObject(parent)
{
    this->vlc_playing = false;
    this->vlc_ready = false;
    this->vlc_instance = libvlc_new(0, NULL);
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
        this->vlc_media = libvlc_media_new_path(this->vlc_instance, file.toStdString().c_str());
        this->vlc_media_player = libvlc_media_player_new_from_media(this->vlc_media);
        libvlc_media_release(this->vlc_media);
    }
}

void MediaPlayer::setTargetWidget(QWidget *widget)
{
    this->targetWidget = widget;
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
    libvlc_media_player_set_xwindow(
                this->vlc_media_player,
                this->targetWidget->winId());
#endif
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
