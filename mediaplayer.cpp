/* vim: set et ts=4 sw=4: */

#include "mediaplayer.h"
#include <iostream>

MediaPlayer::MediaPlayer(QObject *parent) :
    QObject(parent)
{
    this->vlc_playing = false;
    this->vlc_ready = false;
#ifdef HAVE_DEBUG
    this->logErr = NULL;
#endif
}

MediaPlayer::~MediaPlayer()
{
    // libvlc_media_player_release(this->vlc_media_player);
    // libvlc_release(this->vlc_instance);
#ifdef HAVE_DEBUG
    fclose(this->logErr);
#endif
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
    const char *vlc_argv[] = {
#ifndef HAVE_DEBUG
        "--verbose=0",
#else
        "--verbose=2",
#endif
#ifdef Q_WS_MAC
	"--vout=macosx",
#endif
        "--no-video-title-show",
    };

    /* Debug release, redirect stderr to logfile */
#ifdef HAVE_DEBUG
    QString log = QString(QDir::tempPath() + QString(APPNAME) + QString("_err.log"));
    this->logErr = freopen(log.toLocal8Bit().data(), "w+", stderr);
    if(!this->logErr) {
        QMessageBox::critical(0,
                              QObject::tr(APPNAME),
                              QObject::tr("Cannot redirect standard error output to file '%1'.").arg(log)
                              );
    } else {
	QMessageBox::information(0, QObject::tr(APPNAME), QObject::tr("Standard error has been redirected to '%1'.").arg(log));
    }
#endif

    this->vlc_instance = libvlc_new(sizeof(vlc_argv) / sizeof(*vlc_argv), vlc_argv);
    if (!this->vlc_instance) {
        std::cerr << "[vlc_instance] VLC Error:" << libvlc_errmsg() << std::endl;
        return;
    }

    this->vlc_media = libvlc_media_new_path(this->vlc_instance, this->mediaFile.toStdString().c_str());
    if (!this->vlc_media) {
        std::cerr << "[vlc_media] VLC Error:" << libvlc_errmsg() << std::endl;
        return;
    }

    foreach(QWidget* vidWidget, this->videoTargets) {
        libvlc_media_player_t *vlc_mp = libvlc_media_player_new_from_media(this->vlc_media);
        if (!vlc_mp) {
            std::cerr << "[vlc_mp] VLC Error:" << libvlc_errmsg() << std::endl;
            return;
        }
#ifdef Q_WS_WIN
        libvlc_media_player_set_hwnd(
                vlc_mp,
                vidWidget->winId());
#endif
#ifdef Q_WS_MAC
        libvlc_media_player_set_nsobject(
                vlc_mp,
                (void*)(vidWidget->winId()));
#endif
#ifdef Q_WS_X11
    // QString win1 = QString("%1").arg(this->videoTargets[0]->winId());
    // QString win2 = QString("%1").arg(this->videoTargets[1]->winId());
    // QString clone = QString("--vout=clone{vout=x11{xid=%1},vout=x11{xid=%2}}").arg(win1, win2);
        libvlc_media_player_set_xwindow(
                vlc_mp,
                vidWidget->winId());
#endif
        this->vlc_media_players.append(vlc_mp);
    }
    libvlc_media_release(this->vlc_media);

    //        "--vout-filter=clone",
    //        "--clone-count=2",
    //        clone.toStdString().c_str(),

    //    std::cerr << clone.toStdString().c_str() << std::endl;

    this->vlc_ready = true;
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
        foreach(libvlc_media_player_t *vlc_mp, this->vlc_media_players) {
            libvlc_media_player_play(vlc_mp);
        }
        this->vlc_playing = true;
        emit playbackStarted();
    }
}

void MediaPlayer::pause()
{
    foreach(libvlc_media_player_t *vlc_mp, this->vlc_media_players) {
        libvlc_media_player_pause(vlc_mp);
    }
    this->vlc_playing = true;
    emit playbackPaused();
}

void MediaPlayer::stop()
{
    foreach(libvlc_media_player_t *vlc_mp, this->vlc_media_players) {
        libvlc_media_player_stop(vlc_mp);
    }
    this->vlc_playing = false;
    emit playbackStopped();
}
