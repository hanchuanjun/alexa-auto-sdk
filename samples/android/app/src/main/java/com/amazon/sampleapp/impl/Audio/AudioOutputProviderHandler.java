package com.amazon.sampleapp.impl.Audio;

import android.app.Activity;

import com.amazon.aace.audio.AudioOutput;
import com.amazon.aace.audio.AudioOutputProvider;
import com.amazon.sampleapp.impl.AlexaClient.AlexaClientHandler;
import com.amazon.sampleapp.impl.Logger.LoggerHandler;

import java.util.HashMap;

public class AudioOutputProviderHandler extends AudioOutputProvider
{
    private static final String sTag = AudioOutputProviderHandler.class.getSimpleName();

    private final Activity mActivity;
    private final LoggerHandler mLogger;
    private final AlexaClientHandler mAlexaClientHandler;

    private HashMap<String,AudioOutput> mAudioOutputMap;

    public AudioOutputProviderHandler(  Activity activity, LoggerHandler logger, AlexaClientHandler alexaClientHandler ) {
        mActivity = activity;
        mLogger = logger;
        mAlexaClientHandler = alexaClientHandler;
        mAudioOutputMap = new HashMap<>();
    }

    public AudioOutput getOutputChannel( String name ) {
        return mAudioOutputMap.containsKey( name ) ? mAudioOutputMap.get( name ) : null;
    }

    @Override
    public AudioOutput openChannel( String name, AudioOutputType type )
    {
        mLogger.postInfo( sTag, String.format( "openChannel[name=%s,type=%s]", name, type.toString() ) );

        AudioOutput audioOutputChannel = null;

        switch( type )
        {
            case COMMUNICATION:
                audioOutputChannel = new RawAudioOutputHandler( mActivity, mLogger, name );
                break;

            default:
                AudioOutputHandler audioOutputHandler = new AudioOutputHandler( mActivity, mLogger, name );
                audioOutputChannel = audioOutputHandler;
                mAlexaClientHandler.registerAuthStateObserver( audioOutputHandler );
                break;
        }

        mAudioOutputMap.put( name, audioOutputChannel );

        return audioOutputChannel;
    }
}
