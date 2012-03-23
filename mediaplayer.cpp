/* vim: set et ts=4 sw=4: */

#include "mediaplayer.h"
#include <iostream>

MediaPlayer::MediaPlayer(QObject *parent) :
    QObject(parent)
{
    this->vlc_playing = false;
    this->vlc_ready = false;
    this->targetToMute = NULL;
#ifdef HAVE_DEBUG
    this->logErr = NULL;
#endif
}

MediaPlayer::~MediaPlayer()
{
    // libvlc_media_player_release(this->vlc_media_player);
    // libvlc_release(this->vlc_instance);
#ifdef HAVE_DEBUG
    if (this->logErr) {
        fclose(this->logErr);
    }
#endif
}

MediaPlayer& MediaPlayer::getInstance(QObject *parent)
{
    static MediaPlayer instance(parent);
    return instance;
}

void MediaPlayer::setTargetToMute(QWidget *widget)
{
    this->targetToMute = widget;
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

void MediaPlayer::attachMediaPlayerToWidget(libvlc_media_player_t* vlc_mp, QWidget *widget)
{
    WId target = widget->winId();

#ifdef Q_WS_WIN
    libvlc_media_player_set_hwnd(vlc_mp, target);
#endif
#ifdef Q_WS_MAC
    libvlc_media_player_set_nsobject(vlc_mp, (void*)target);
#endif
#ifdef Q_WS_X11
    // QString win1 = QString("%1").arg(this->videoTargets[0]->winId());
    // QString win2 = QString("%1").arg(this->videoTargets[1]->winId());
    // QString clone = QString("--vout=clone{vout=x11{xid=%1},vout=x11{xid=%2}}").arg(win1, win2);
    libvlc_media_player_set_xwindow(vlc_mp, target);
#endif
}

void MediaPlayer::detachMediaPlayerFromWidget(libvlc_media_player_t* vlc_mp)
{
#ifdef Q_WS_WIN
    libvlc_media_player_set_hwnd(vlc_mp, NULL);
#endif
#ifdef Q_WS_MAC
    libvlc_media_player_set_nsobject(vlc_mp, NULL);
#endif
#ifdef Q_WS_X11
    libvlc_media_player_set_xwindow(vlc_mp, NULL);
#endif
}

void MediaPlayer::muteMediaPlayer(libvlc_media_player_t *vlc_mp, QWidget *widget)
{
    // found a target to mute!!
    if (widget == this->targetToMute) {
        std::cerr << "setting mute for " << widget << std::endl;
        libvlc_audio_set_mute(vlc_mp, 1);
    } else {
        std::cerr << "no mute for " << widget << std::endl;
    }
    std::cerr << "mute status for " << widget << " @" << vlc_mp << ": " << libvlc_audio_get_mute(vlc_mp) << std::endl;
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
        "--no-osd",
        "--text-renderer=tdummy",
    };

    //        "--vout-filter=clone",
    //        "--clone-count=2",
    //        clone.toStdString().c_str(),

    //    std::cerr << clone.toStdString().c_str() << std::endl;

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

    this->vlc_ready = true;
}

void MediaPlayer::prepareMedia()
{
    this->vlc_media_players.clear();
    this->vlc_event_managers.clear();

    this->vlc_media = libvlc_media_new_path(this->vlc_instance, this->mediaFile.toStdString().c_str());
    if (!this->vlc_media) {
        std::cerr << "[vlc_media] VLC Error:" << libvlc_errmsg() << std::endl;
        return;
    }

    int iter = 0;
    foreach(QWidget* vidWidget, this->videoTargets) {
        libvlc_media_player_t *vlc_mp = libvlc_media_player_new_from_media(this->vlc_media);
        if (!vlc_mp) {
            std::cerr << "[vlc_mp] VLC Error:" << libvlc_errmsg() << std::endl;
            return;
        }
        libvlc_event_manager_t *vlc_em = libvlc_media_player_event_manager(vlc_mp);
        if (!vlc_mp) {
            std::cerr << "[vlc_em] VLC Error:" << libvlc_errmsg() << std::endl;
            return;
        }
        this->vlc_media_players.append(vlc_mp);
        this->vlc_event_managers.append(vlc_em);
        iter++;
    }

    libvlc_media_release(this->vlc_media);

    foreach(libvlc_event_manager_t *vlc_em, this->vlc_event_managers) {
        libvlc_event_attach(vlc_em, libvlc_MediaPlayerTimeChanged, MediaPlayer::mediaEventCallback, this);
        libvlc_event_attach(vlc_em, libvlc_MediaPlayerPositionChanged, MediaPlayer::mediaEventCallback, this);
    }
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

void MediaPlayer::seekPlayback(float position)
{
    this->seek(position);
}

void MediaPlayer::volumePlayback(int volume)
{
    this->volume(volume);
}

void MediaPlayer::play()
{
    if (!this->vlc_ready) {
        this->preparePlayer();
    }

    if (!this->vlc_playing) {
        int iMediaPlayer = 0;
        this->prepareMedia();
        QWidget *w;
        foreach(libvlc_media_player_t *vlc_mp, this->vlc_media_players) {
            w = this->videoTargets[iMediaPlayer];
            this->attachMediaPlayerToWidget(vlc_mp, w);
            libvlc_media_player_play(vlc_mp);
#if 0
            this->muteMediaPlayer(vlc_mp, w);
#endif
            iMediaPlayer++;
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
        this->detachMediaPlayerFromWidget(vlc_mp);
    }

    foreach(libvlc_event_manager_t *vlc_em, this->vlc_event_managers) {
        libvlc_event_attach(vlc_em, libvlc_MediaPlayerTimeChanged, MediaPlayer::mediaEventCallback, this);
        libvlc_event_attach(vlc_em, libvlc_MediaPlayerPositionChanged, MediaPlayer::mediaEventCallback, this);
    }

    this->vlc_playing = false;
    emit playbackStopped();
}

void MediaPlayer::seek(float position)
{
    foreach(libvlc_media_player_t *vlc_mp, this->vlc_media_players) {
        libvlc_media_player_set_position(vlc_mp, position);
    }
    emit playbackSeeked();
}

void MediaPlayer::volume(int vol)
{
    foreach(libvlc_media_player_t *vlc_mp, this->vlc_media_players) {
        libvlc_audio_set_volume(vlc_mp, vol);
    }
    emit playbackVolumed();
}

int MediaPlayer::getVolume()
{
    return libvlc_audio_get_volume(this->vlc_media_players[0]);
}

void MediaPlayer::mediaEventCallback(const libvlc_event_t *ev, void *p)
{
    if (ev && p) {
        MediaPlayer* mp = (MediaPlayer*)p;

        if (ev->type == libvlc_MediaPlayerTimeChanged) {
            qint64 time = ev->u.media_player_time_changed.new_time;
            emit mp->mediaTimeChanged(time);
        }

        if (ev->type == libvlc_MediaPlayerPositionChanged) {
            float pos = ev->u.media_player_position_changed.new_position;
            emit mp->mediaPositionChanged(pos);
        }
    }
}
