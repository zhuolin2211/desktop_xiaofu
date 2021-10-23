#include "playaudio.h"
#include "QString"
#include "stdio.h"
#include "time.h"
#include "sys/stat.h"
Playaudio::Playaudio(const QString &filename)
{
    audio_file=filename;
    channels =2;
    sample_rate=48100;
    bufferFrames=512;
    device =0;
    offset=0;
    play_flg=0;
    play_flg_change=0;
    play_time=0;
    pthread_mutex_init(&lock, NULL);
    this->start();
}
void Playaudio::run()
{
    /*open file*/
    data.fd = fopen( audio_file.toLatin1(), "rb" );
      if ( !data.fd ) {
        std::cout << "Unable to find or open file!\n";
        my_exit(openfile_error);
      }
      play_time=this->get_totaltime_s();
      data.channels=channels;
      oParams.deviceId = device;
      oParams.nChannels = channels;
      oParams.firstChannel = offset;
      oParams.deviceId = dac.getDefaultOutputDevice();

      try {
          dac.openStream( &oParams, NULL, FORMAT, sample_rate, &bufferFrames, &stream_callback, (void *)&data );

        }
        catch ( RtAudioError& e ) {
          std::cout << '\n' << e.getMessage() << '\n' << std::endl;
          fclose( data.fd );
          dac.closeStream();
          my_exit(openstream_error);
        }

      while ( 1 ) {
          usleep( 50*1000 ); // wake every 100 ms to check if we're done
            emit progress_signal((dac.getStreamTime()/(double)this->play_time)*100);
            if(play_operation()<0)
            {
                dac.closeStream();
                emit paly_done();
                return ;
            }
        }

}

int Playaudio::stream_callback( void *outputBuffer, void * /*inputBuffer*/, unsigned int nBufferFrames,
            double /*streamTime*/, RtAudioStreamStatus /*status*/, void *data )
{
  OutputData *oData = (OutputData*) data;

  // In general, it's not a good idea to do file input in the audio
  // callback function but I'm doing it here because I don't know the
  // length of the file we are reading.
  unsigned int count = fread( outputBuffer, oData->channels * sizeof( MY_TYPE ), nBufferFrames, oData->fd);
  if ( count < nBufferFrames ) {
    unsigned int bytes = (nBufferFrames - count) * oData->channels * sizeof( MY_TYPE );
    unsigned int startByte = count * oData->channels * sizeof( MY_TYPE );
    memset( (char *)(outputBuffer)+startByte, 0, bytes );
    return 1;
  }

  return 0;
}

void Playaudio::my_exit(audio_error code)
{

    switch (code)
    {
    case no_error:
    {
        std::cout<<"play over\r\n";
        break;
    }
    case openfile_error:
    {
        std::cout<<"open file error \r\n";
        break;
    }
    case openstream_error:
    {
        std::cout<<"open stream error \r\n";
        break;
    }
    default:
    {
        std::cout<<"unknow error\r\n";
    }
    }
    play_flg=0;

}
void Playaudio::start_play(void)
{
        pthread_mutex_lock(&lock);
        if(play_flg!=1)
        {
            play_flg=1;
            play_flg_change=1;
        }
        pthread_mutex_unlock(&lock);
}
void Playaudio::suspend_play(void)
{
    pthread_mutex_lock(&lock);
    if(play_flg==1)
    {
        play_flg=2;
    play_flg_change=1;
    }
    pthread_mutex_unlock(&lock);
}
void Playaudio::stop_play(void)
{
    pthread_mutex_lock(&lock);
    if(play_flg!=0)
    {
    play_flg=0;
    play_flg_change=1;
    }
    pthread_mutex_unlock(&lock);
}
int Playaudio::play_operation(void)
{
    if(pthread_mutex_trylock(&lock)==0)
    {
        if(play_flg_change!=0)
        {
            play_flg_change=0;
           switch (play_flg)
           {
           case 0://stop
           {
               dac.abortStream();
               std::cout<<"stop play music\r\n";
               my_exit(no_error);
               pthread_mutex_unlock(&lock);
               return -1;
           }
           case 1://start
           {
               dac.startStream();
               std::cout<<"start play music\r\n";
               break;
           }
           case 2://suspend
           {
               dac.stopStream();
               std::cout<<"suspend play music\r\n";
               break;
           }
           }
        }
        else
        {
            if ( dac.isStreamRunning() == false  && (play_flg==1))
            {
                this->play_flg=0;
                my_exit(no_error);
                pthread_mutex_unlock(&lock);
                return -1;
            }

        }
        pthread_mutex_unlock(&lock);
    }
    return 0;
}

/**/
unsigned long Playaudio::get_StrameSize(void)
{
    unsigned long filesize = 0;
        struct stat statbuff;
        if(stat(this->audio_file.toLatin1(), &statbuff) >= 0)
            {
            filesize = statbuff.st_size;
        }
        return filesize;
}
unsigned long Playaudio::get_totaltime_s(void)
{
    unsigned long file_size;
    unsigned long totaltime=0;
    file_size=this->get_StrameSize();
    if(file_size>0)
    {
           totaltime=file_size/this->sample_rate/2/2;
    }
    return totaltime;
}
