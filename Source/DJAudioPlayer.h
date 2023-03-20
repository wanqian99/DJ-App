/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 13 Mar 2020 4:22:22pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include <string>
#include <algorithm>
#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public AudioSource {
  public:

    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    /** tells source to prepare for playing */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    /** called repeatedly to fetch subsequent blocks of audio of data */
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    /** allows source to release anything it no longer needs after playback has stopped */
    void releaseResources() override;

    /** sets the audio file to play */
    void loadURL(URL audioURL);
    /** sets the audio's volume */
    void setGain(double gain);
    /** sets the audio's speed */
    void setSpeed(double ratio);
    
    /** sets the position the audio should play from */
    void setPosition(double posInSecs);
    /** returns position (in seconds) the audio should play from */
    void setPositionRelative(double pos);
    
    /** starts playing the file */
    void start();
    /** stops playing the file */
    void stop();
    /** rewind track by 5 seconds */
    void rewind();
    /** forward track by 5 seconds */
    void forward();
    /** check if audio is playing */
    bool isPlaying();
    /** toggles the loop button */
    void toggleLoop();
    /** check if loop is on */
    bool isLooping();

    /** get the relative position of the playhead */
    double getPositionRelative();
    
    /** current track name */
    juce::String trackPlayingTitle;
    
    /** track length left */
    juce::String trackPlayingLeft();
    
    /** bool that signifies track is loaded into the player */
    bool load;
    
    //bool to signify if the player is playing
    bool play;
private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource; 
    ResamplingAudioSource resampleSource{&transportSource, false, 2};
    
    //bool to signify if loop is on or off
    bool loop;
};




