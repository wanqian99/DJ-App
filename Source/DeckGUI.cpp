/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse)
                : player(_player), // assign _player to our player data member
                  waveformDisplay(formatManagerToUse, cacheToUse), // call the constructor on waveformDisplay
                  angle(0)
{
    //image for buttons
    auto loopImage = ImageCache::getFromMemory(BinaryData::loopbutton_png, BinaryData::loopbutton_pngSize);
    auto stopImage = ImageCache::getFromMemory(BinaryData::stopbutton_png, BinaryData::stopbutton_pngSize);
    auto rewindImage = ImageCache::getFromMemory(BinaryData::rewindbutton_png, BinaryData::rewindbutton_pngSize);
    auto playImage = ImageCache::getFromMemory(BinaryData::playbutton_png, BinaryData::playbutton_pngSize);
    auto pauseImage = ImageCache::getFromMemory(BinaryData::pausebutton_png, BinaryData::pausebutton_pngSize);
    auto forwardImage = ImageCache::getFromMemory(BinaryData::forwardbutton_png, BinaryData::forwardbutton_pngSize);
    
    //set images to buttons
    loopButton.setImages(true, true, true, loopImage, 1, Colours::white, Image(nullptr), 1, Colour::fromRGB(155, 175, 255), loopImage, 1, Colour::fromRGB(155, 175, 255));
    stopButton.setImages(true, true, true, stopImage, 1, Colours::white, Image(nullptr), 1, Colour::fromRGB(155, 175, 255), stopImage, 1, Colour::fromRGB(155, 175, 255));
    rewindButton.setImages(true, true, true, rewindImage, 1, Colours::white, Image(nullptr), 1, Colour::fromRGB(155, 175, 255), rewindImage, 1, Colour::fromRGB(155, 175, 255));
    playButton.setImages(true, true, true, playImage, 1, Colours::white, Image(nullptr), 1, Colour::fromRGB(155, 175, 255), pauseImage, 1, Colour::fromRGB(155, 175, 255));
    forwardButton.setImages(true, true, true, forwardImage, 1, Colours::white, Image(nullptr), 1, Colour::fromRGB(155, 175, 255), forwardImage, 1, Colour::fromRGB(155, 175, 255));

    //setup playbutton to auto toggle
    playButton.setClickingTogglesState(true);
    
    //make buttons visible
    addAndMakeVisible(loopButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(rewindButton);
    addAndMakeVisible(playButton);
    addAndMakeVisible(forwardButton);
    addAndMakeVisible(loadButton);
    
    //make sliders visible
    addAndMakeVisible(volSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(speedSlider);

    //make waveform component visible
    addAndMakeVisible(waveformDisplay);
    
    //setup slider styles
    volSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    volSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 25);
    volSlider.setColour(Slider::textBoxOutlineColourId, Colour::fromRGB(0, 15, 40));
    
    posSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    
    speedSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    speedSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 25);
    speedSlider.setColour(Slider::textBoxOutlineColourId, Colour::fromRGB(0, 15, 40));
    
    //setup slider colours
    getLookAndFeel().setColour(Slider::trackColourId, Colour::fromRGB(205, 215, 255));
    getLookAndFeel().setColour(Slider::thumbColourId, Colour::fromRGB(155, 175, 255));
    getLookAndFeel().setColour(Slider::rotarySliderFillColourId, Colour::fromRGB(205, 215, 255));
    
    //set text button colours
    getLookAndFeel().setColour(TextButton::buttonColourId, Colour::fromRGB(0, 25, 65));
    getLookAndFeel().setColour(TextButton::textColourOnId , Colours::black);
    
    //setup vol slider label
    addAndMakeVisible(volLabel);
    volLabel.setText("VOL", juce::dontSendNotification);
    volLabel.setJustificationType(Justification::centredTop);
    volLabel.attachToComponent(&volSlider, false);
    
    //no text box for position slider
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    
    //setup speed slider label
    addAndMakeVisible(speedLabel);
    speedLabel.setText("SPD", juce::dontSendNotification);
    speedLabel.setJustificationType(Justification::centredTop);
    speedLabel.attachToComponent(&speedSlider, false);

    //add listener to buttons
    loopButton.addListener(this);
    stopButton.addListener(this);
    rewindButton.addListener(this);
    playButton.addListener(this);
    forwardButton.addListener(this);
    loadButton.addListener(this);

    //add listener to sliders
    volSlider.addListener(this);
    posSlider.addListener(this);
    speedSlider.addListener(this);

    //set sliders' range
    //speed cannot be 0, therefore minimum value for the speed slider is set at 0.1
    volSlider.setRange(0.0, 1.0, 0.1);
    posSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.1, 5.0, 0.1);
    
    //set sliders' value
    volSlider.setValue(1.0);
    speedSlider.setValue(1.0);

    //100 means timerCallback is called every 100ms, an update rate of ten frames per second
    startTimer(100);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

/** called when something needs to be drawn */
void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    //background colour
    g.fillAll(Colour::fromRGB(0, 15, 40));
    
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("POSITION", getWidth()/2 - 40, getHeight() - 80, 80, 20,
                Justification::centred, true);
    
    //if file is loaded, calls the DJAudioPlayer to get the title and length left of the track that is currently playing
    if(player->load)
    {
        g.setColour (Colours::white);
        g.setFont (16.0f);
        //display track title
        g.drawText ("Track Title: " + player->trackPlayingTitle, getWidth()/5 + 10, getHeight()/5 + 15, 220, 20, Justification::left, true);
        //display track audio left
        g.drawText ("Track Left: " + player->trackPlayingLeft(), getWidth()/5 + 10, getHeight()/5 + 35, 220, 20, Justification::left, true);
    }
    //player not loaded
    else
    {
        g.setColour (Colours::white);
        g.setFont (16.0f);
        //display track title
        g.drawText ("Track Title: TRACK NOT LOADED", getWidth()/5 + 10, getHeight()/5 + 15, 220, 20, Justification::left, true);
        //display track audio left
        g.drawText ("Track Left: TRACK NOT LOADED", getWidth()/5 + 10, getHeight()/5 + 35, 220, 20, Justification::left, true);
    }
    
    //setup the turntable image
    auto turntable = ImageCache::getFromMemory(BinaryData::turntable_png, BinaryData::turntable_pngSize);
    
    //use AffineTransformation to translate, rotate and draw the image at the correct place
    g.addTransform(juce::AffineTransform::translation(getWidth()/2-turntable.getWidth()/2-1, 3*getHeight()/5-turntable.getHeight()/2));
    g.addTransform(juce::AffineTransform::rotation(angle, turntable.getWidth()/2, turntable.getHeight()/2));
    g.drawImageAt(turntable, 0, 0, false);
    
    //if the player isPlaying, increase the rotation angle
    if(player->isPlaying())
    {
        angle+=2;
    }
}

/** called when window changes size, or when window opens */
void DeckGUI::resized()
{
    double rowH = getHeight() / 10;
    double colW = getWidth() / 12;
    
    //layout the components
    waveformDisplay.setBounds(0, 0, getWidth(), rowH * 2);
    volSlider.setBounds(colW, rowH * 3.0, colW, rowH * 6.5);
    posSlider.setBounds(colW * 2.0, rowH * 3.5, colW * 8.0, rowH * 5.0);
    speedSlider.setBounds(colW * 10.0, rowH * 3.0, colW, rowH * 6.5);
    loopButton.setBounds(colW * 2.7, rowH * 8.5, colW * 0.6, rowH);
    stopButton.setBounds(colW * 3.8, rowH * 8.5, colW * 0.5, rowH);
    rewindButton.setBounds(colW * 4.8, rowH * 8.5, colW * 0.5, rowH);
    playButton.setBounds(colW * 5.8, rowH * 8.5, colW * 0.5, rowH);
    forwardButton.setBounds(colW * 6.8, rowH * 8.5, colW * 0.5, rowH);
    loadButton.setBounds(colW * 7.8, rowH * 8.5, colW * 2.0, rowH);
}

/** implement Button::Listener */
void DeckGUI::buttonClicked(Button* button)
{
    if (button == &loopButton)
    {
        repaint();
        player->toggleLoop();
        
        //set the loop button colours according to the loop state
        if(player->isLooping())
        {
            loopButton.setToggleState(true, juce::dontSendNotification);
        }
        else
        {
            loopButton.setToggleState(false, juce::dontSendNotification);
        }
    }
    
    if (button == &stopButton)
    {
        //stop playing and reset the position to the start of the track
        player->stop();
        player->setPosition(0);
        //reset the playbutton to playImage
        playButton.setToggleState(false, juce::dontSendNotification);
    }
    
    if (button == &rewindButton)
    {
        //rewind by 5 seconds
        player->rewind();
    }
    
    if (button == &playButton)
    {
        //togglestate true, start player, set to pause image
        if(button->getToggleState())
            player->start();
        else //togglestate false, stop player, set to play image
            player->stop();
    }
    
    if (button == &forwardButton)
    {
        //forward by 5 seconds
        player->forward();
    }
    
    if (button == &loadButton)
    {
        FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(URL{chooser.getResult()});
            waveformDisplay.loadURL(URL{chooser.getResult()});
        }
    }
}

/** implement Slider::Listener */
void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    
}

/** implement FileDragAndDropTarget */
bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

/** implement FileDragAndDropTarget */
void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
      URL fileURL = URL{File{files[0]}};
      player->loadURL(fileURL);
      
      //if the player is playing, start playing the new audio
      if(player->isPlaying())
      {
          player->start();
      }
  }
}

void DeckGUI::timerCallback()
{
    double pos = player->getPositionRelative();
    waveformDisplay.setPositionRelative(pos);
    //make the pos slider move as the audio plays
    if (pos >= 0.0) {
        posSlider.setValue(pos);
    }
    //if the pos reaches the end of the track amd the player isPlaying,
    //loop the audio if loop button is pressed
    if(pos >= 1.0 && player->isPlaying() && player->isLooping())
    {
        std::cout<<"looping"<<std::endl;
        player->setPosition(0);
        player->start();
    }
    //if the pos reaches the end of the track, the player loop state is false, and the playbutton state is true
    //set the play button state to false
    //set the play boolean to false
    if(pos >= 1.0 && !player->isLooping() && playButton.getToggleState())
    {
        playButton.setToggleState(false, juce::dontSendNotification);
        player->play = false;
    }
    
    if(player->isPlaying())
    {
        repaint();
    }
}
