/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }  

    //make the 2 deckGUIs visible
    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2);
    
    //make the playlistComponent visible
    addAndMakeVisible(playlistComponent);

    //call registerBasicFormats on formatManager to register basic audio formats that comes with JUCE
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
/** tells source to prepare for playing */
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    //prepares both players for playing
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    //mixes together the output of a set of other AudioSources
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    //adds player1 and player2 as input sources to the mixer
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

}

/** called repeatedly to fetch subsequent blocks of audio of data */
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    //gets the next block of audio data
    mixerSource.getNextAudioBlock(bufferToFill);
}

/** allows source to release anything it no longer needs after playback has stopped */
void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    //deckGUI 1 & 2 layout
    deckGUI1.setBounds(0, 0, getWidth()/2, 2*getHeight()/3);
    deckGUI2.setBounds(getWidth()/2, 0, getWidth()/2, 2*getHeight()/3);
    
    //playlistComponent layout
    playlistComponent.setBounds(0, 2*getHeight()/3, getWidth(), 1*getHeight()/3);
}

