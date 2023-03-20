/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager & 	formatManagerToUse,
                                 AudioThumbnailCache & 	cacheToUse) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse), 
                                 fileLoaded(false), 
                                 position(0)
                          
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

  audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
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

    g.setColour (Colour::fromRGB(205, 215, 255));
    if(fileLoaded)
    {
        audioThumb.drawChannel(g,
            getLocalBounds(),
            0,
            audioThumb.getTotalLength(),
            0,
            1.0f
        );
        g.setColour(Colours::mediumvioletred);
        g.drawRect(position * getWidth(), 0, getWidth() / 50, getHeight());
    }
    else 
    {
        g.setColour (Colours::white);
        g.setFont (20.0f);
        g.drawText ("File not loaded...", getLocalBounds(),
                  Justification::centred, true);   // draw some placeholder text

    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

/** loads the audio file */
void WaveformDisplay::loadURL(URL audioURL)
{
    audioThumb.clear();
    fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL));
    if (fileLoaded)
    {
        std::cout << "wfd: loaded! " << std::endl;
        repaint();
    }
    else {
        std::cout << "wfd: not loaded! " << std::endl;
    }
}

/** listens for changes and calls paint again */
void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    std::cout << "wfd: change received! " << std::endl;

    repaint();

}

/** set the relative position of the playhead */
void WaveformDisplay::setPositionRelative(double pos)
{
    if(pos >= 0)
    {
        if (pos != position)
        {
          position = pos;
          repaint();
        }
    }
}
