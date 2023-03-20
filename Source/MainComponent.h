/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    /** tells source to prepare for playing */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    /** called repeatedly to fetch subsequent blocks of audio of data */
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    /** allows source to release anything it no longer needs after playback has stopped */
    void releaseResources() override;

    //==============================================================================
    /** called when something needs to be drawn */
    void paint (Graphics& g) override;
    /** called when window changes size, or when window opens */
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
     
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100}; 

    /** deck 1 */
    DJAudioPlayer player1{formatManager};
    DeckGUI deckGUI1{&player1, formatManager, thumbCache}; 

    /** deck 2 */
    DJAudioPlayer player2{formatManager};
    DeckGUI deckGUI2{&player2, formatManager, thumbCache}; 

    /** allows mix audio from the 2 decks */
    MixerAudioSource mixerSource; 
    
    /** formatManager is used by playComponent to get the audio length */
    PlaylistComponent playlistComponent{&deckGUI1, &deckGUI2, formatManager};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
