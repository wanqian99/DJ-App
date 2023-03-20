/*
==============================================================================

DJAudioPlayer.cpp
Created: 13 Mar 2020 4:22:22pm
Author:  matthew

==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager) 
: formatManager(_formatManager)
{

}
DJAudioPlayer::~DJAudioPlayer()
{

}

/** tells source to prepare for playing */
void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) 
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

/** called repeatedly to fetch subsequent blocks of audio of data */
void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);

}

/** allows source to release anything it no longer needs after playback has stopped */
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

/** sets the audio file to play */
void DJAudioPlayer::loadURL(URL audioURL)
{
    //gets the name of the track that is current playing
    trackPlayingTitle = audioURL.getFileName();
    
    //creates a reader if the stream can be opened
    //else it returns a nullptr
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    
    if (reader != nullptr) // good file!
    {       
        std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, true));
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);             
        readerSource.reset (newSource.release());
        
        load = true;
    }
    
    //if the player was playing, start playing the new audio
    if(isPlaying())
    {
        start();
    }
}

/** sets the audio's volume by calling setGain on transportSource */
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else {
        transportSource.setGain(gain);
    }
}

/** sets the audio's speed by calling setResamplingRatio on resampleSource */
void DJAudioPlayer::setSpeed(double ratio)
{
  if (ratio < 0.1 || ratio > 5.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 10" << std::endl;
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

/** sets the position the audio should play from */
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

/** returns position (in seconds) the audio should play from */
void DJAudioPlayer::setPositionRelative(double pos)
{
     if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

/** starts playing the file */
void DJAudioPlayer::start()
{
    play = true;
    transportSource.start();
}

/** stops playing the file */
void DJAudioPlayer::stop()
{
    play = false;
    transportSource.stop();
}

/** rewind track by 5 seconds */
void DJAudioPlayer::rewind()
{
    //check if the position is more than 0 after rewinding 5secs
    //else set position to beginning of track
    if((transportSource.getCurrentPosition() - 5.0) > 0)
    {
        transportSource.setPosition(transportSource.getCurrentPosition() - 5.0);
    }
    else
    {
        transportSource.setPosition(0);
    }
}

/** forward track by 5 seconds */
void DJAudioPlayer::forward()
{
    //check if the position is less than track length after forwarding 5secs
    //else set position to end of track
    if((transportSource.getCurrentPosition() + 5.0) < transportSource.getLengthInSeconds())
    {
        transportSource.setPosition(transportSource.getCurrentPosition() + 5.0);
    }
    else
    {
        transportSource.setPosition(transportSource.getLengthInSeconds());
    }
}

/** check if audio is playing */
bool DJAudioPlayer::isPlaying()
{
    return play;
}

/** toggles the loop button */
void DJAudioPlayer::toggleLoop()
{
    if (loop) {
        loop = false;
    } else {
        loop = true;
    }
}

/** check if loop is on */
bool DJAudioPlayer::isLooping()
{
    return loop;
}

/** get the relative position of the playhead */
double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

/** track length left */
juce::String DJAudioPlayer::trackPlayingLeft()
{
    int trackLengthLeft = transportSource.getLengthInSeconds() - transportSource.getCurrentPosition();
    
    //minutes
    std::string trackLeftMins = std::to_string(trackLengthLeft / 60);
    
    //seconds
    std::string trackLeftSecs = std::to_string(trackLengthLeft % 60);
    
    if(std::stoi(trackLeftSecs) <= 9)
    {
        trackLeftSecs = "0" + std::to_string(trackLengthLeft % 60);
    }
    
    //returns track length left (mins & seconds)
    return trackLeftMins + ":" + trackLeftSecs;
}
