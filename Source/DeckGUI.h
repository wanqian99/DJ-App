/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget, 
                   public Timer
{
public:
    DeckGUI(DJAudioPlayer* player, 
           AudioFormatManager & 	formatManagerToUse,
           AudioThumbnailCache & 	cacheToUse);
    ~DeckGUI();

    /** called when something needs to be drawn */
    void paint (Graphics&) override;
    /** called when window changes size, or when window opens */
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (Button *) override;

    /** implement Slider::Listener */
    void sliderValueChanged (Slider *slider) override;

    /** implement FileDragAndDropTarget */
    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override; 

    void timerCallback() override;

    /** playlist component needs these to be public to access them */
    WaveformDisplay waveformDisplay;
    DJAudioPlayer* player;
    
private:

    ImageButton loopButton;
    ImageButton stopButton;
    ImageButton rewindButton;
    ImageButton playButton;
    ImageButton forwardButton;
    
    TextButton loadButton{"LOAD"};
  
    Slider volSlider; 
    Slider speedSlider;
    Slider posSlider;
    
    Label volLabel;
    Label speedLabel;
    Label posLabel;
    
    //used to rotate turntable
    double angle;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
