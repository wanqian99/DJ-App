/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 1 Jan 2022 1:04:36pm
    Author:  Gao WanQian

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
/** DeckGUI functions are used to load files from playlist */
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1, DeckGUI* _deckGUI2, AudioFormatManager& _formatManager)
                                    : deckGUI1(_deckGUI1), deckGUI2(_deckGUI2), formatManager(_formatManager)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(savePlaylistButton);
    addAndMakeVisible(loadPlaylistButton);
    addAndMakeVisible(searchBox);

    tableComponent.getHeader().addColumn("Track title", 1, 400);
    tableComponent.getHeader().addColumn("Track Length", 2, 100);
    tableComponent.getHeader().addColumn("Deck1", 3, 100);
    tableComponent.getHeader().addColumn("Deck2", 4, 100);
    tableComponent.getHeader().addColumn("Remove", 5, 100);
    tableComponent.setModel(this);
    
    savePlaylistButton.addListener(this);
    loadPlaylistButton.addListener(this);
    searchBox.addListener(this);
    
    //setup search box, call function to return searched track
    searchBox.setTextToShowWhenEmpty("Search for track...", juce::Colours::white);
    //when there is input in the text editor, call returnSearch, with input parameter as the text in the searchBox
    searchBox.onTextChange = [this] {
                                        returnSearch(searchBox.getText());
                                    };
    
    //table component background and text colour
    getLookAndFeel().setColour(ListBox::backgroundColourId, Colour::fromRGB(0, 15, 40));
    getLookAndFeel().setColour(ListBox::textColourId,Colours::white);
    
    loadPlaylist();
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    //background colour
    g.fillAll(Colour::fromRGB(0, 15, 40));
    
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    loadPlaylistButton.setBounds(10, getHeight()-40, 100, 30);
    savePlaylistButton.setBounds(120, getHeight()-40, 100, 30);
    searchBox.setBounds(getWidth()-210, getHeight()-35, 200, 20);
    
    tableComponent.setBounds(0, 0, getWidth(), getHeight()-45);
    tableComponent.setBounds(0, 0, getWidth(), getHeight()-45);
}

/** return the number of rows currently in the table */
int PlaylistComponent::getNumRows()
{
    return trackTitles.size();
}

/** draw the background behind one of the rows in the table */
void PlaylistComponent::paintRowBackground(Graphics & g, int rowNumber, int width,
                                           int height, bool rowIsSelected)
{
    //set background colour of the cell to light purple when selected,
    //and dark blue when unselected
    if(rowIsSelected)
    {
        g.fillAll(Colour::fromRGB(205, 215, 255));
    }
    else{
        g.fillAll(Colour::fromRGB(0, 25, 65));
    }
}

/** draw one of the cells */
void PlaylistComponent::paintCell(Graphics & g, int rowNumber, int columnId, int width,
                                  int height, bool rowIsSelected)
{
    //set text colour to black when unselected, and white when selected
    if(rowIsSelected)
    {
        g.setColour(Colours::black);
    }
    else
    {
        g.setColour(Colours::white);
    }
    // track title column
    if(columnId == 1)
    {
        g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);
    }
    //track length column
    if(columnId == 2)
    {
        g.drawText(trackLength[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);
    }
}

/** used to create or update a custom component to go in a cell */
//this functions specifies what happens to each cell, whether you have a special component or not.
//if u want a special component, create it and store it into existingComponentToUpdate variable, and than return it
//cos its a pointer, it means that we can push into its memory,
//and then if you dont want a custom component, just return the variable as it is
Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnid, bool isRowSelected,
                                                      Component *existingComponentToUpdate)
{
    //columnid == 3 : deck1 column
    //columnid == 4 : deck2 column
    if(columnid == 3 || columnid == 4)
    {
        //havent create component yet
        if(existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{"LOAD"};
            std::string rowid{std::to_string(rowNumber)};
            std::string colid{std::to_string(columnid)};
            //to get a unique id made of 2 strings added together
            btn->setComponentID((rowid + colid));
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    //remove column
    if(columnid == 5)
    {
        if(existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{"REMOVE"};
            std::string rowid{std::to_string(rowNumber)};
            std::string colid{std::to_string(columnid)};
            btn->setComponentID((rowid + colid));
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button)
{
    if(button == &loadPlaylistButton)
    {
        FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            //push the track title into vector
            trackTitles.push_back(File{chooser.getResult()}.getFileNameWithoutExtension());
            //push the track URL into vector
            trackURL.push_back(URL{chooser.getResult()}.toString(false));
            //push the track length into vector
            trackLength.push_back(getTrackLength(File{chooser.getResult()}));
        }
        tableComponent.updateContent();
    }
    else if(button == &savePlaylistButton)
    {
        //calls savePlaylist to save the tracks that are loaded into the playlist
        //so that playlist is restored when the user restarts the application
        savePlaylist();
    }
    
    //if cell buttons are clicked
    else
    {
        //get first index of button id which is the row
        int row = std::stoi(button->getComponentID().toStdString().substr(0,1));
        //get second index of button id which is the column
        int col = std::stoi(button->getComponentID().toStdString().substr(1,2));
        
        //load to deck1
        if(col == 3)
        {
            //load the url into deck1 player and waveform, and sets the waveform position
            deckGUI1->player->loadURL(URL{trackURL[row]});
            deckGUI1->waveformDisplay.loadURL(URL{trackURL[row]});
            deckGUI1->waveformDisplay.setPositionRelative(deckGUI1->player->getPositionRelative());
            
            //calls paint so that the track title and length is displayed immediately as the track is loaded in
            deckGUI1->repaint();
        }
        //load to deck2
        if(col == 4)
        {
            //load the url into deck2 player and waveform, and sets the waveform position
            deckGUI2->player->loadURL(URL{trackURL[row]});
            deckGUI2->waveformDisplay.loadURL(URL{trackURL[row]});
            deckGUI2->waveformDisplay.setPositionRelative(deckGUI2->player->getPositionRelative());
            
            //calls repaint so that the track title and length is displayed immediately as the track is loaded in
            deckGUI2->repaint();
        }
        //remove audio file
        if(col == 5)
        {
            //remove the title, url, length from the vectors
            trackTitles.erase(trackTitles.begin() + row);
            trackURL.erase(trackURL.begin() + row);
            trackLength.erase(trackLength.begin() + row);
            tableComponent.updateContent();
        }
    }
}

/** implement FileDragAndDropTarget */
bool PlaylistComponent::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true;
}

/** implement FileDragAndDropTarget */
void PlaylistComponent::filesDropped (const StringArray &files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped" << std::endl;
    //load all files into playlist
    if(files.size() >= 1)
    {
        for(int i = 0; i < files.size(); ++i)
        {
            //push the track title into vector
            trackTitles.push_back(File{files[i]}.getFileNameWithoutExtension());
            //push the track URL into vector
            trackURL.push_back(URL{File{files[i]}}.toString(false));
            //push the track length into vector
            trackLength.push_back(getTrackLength(File{files[i]}));
        }
        //update the table
        tableComponent.updateContent();
    }
}

/** calculates the length of the track with the audio file parsed in */
std::string PlaylistComponent::getTrackLength(File track)
{
    //creates a reader and calculates the audio length in seconds and minutes
    auto* reader = formatManager.createReaderFor(File{track});
    int audioLength = (reader->lengthInSamples/reader->sampleRate);
    
    //minutes
    std::string audioLengthMins = std::to_string(audioLength / 60);
    
    //seconds
    std::string audioLengthSecs = std::to_string(audioLength % 60);
    
    if(std::stoi(audioLengthSecs) <= 9)
    {
        audioLengthSecs = "0" + std::to_string(audioLength % 60);
    }
    
    //returns length (mins & seconds)
    return audioLengthMins + ":" + audioLengthSecs;
}

/** load the playlist */
void PlaylistComponent::loadPlaylist()
{
    //temporary vectors for metadata of track
    std::string _trackTitles;
    std::string _trackURL;
    std::string _trackLength;
    
    std::string line;

    //open playlist for reading file
    std::fstream playlist;
    playlist.open(filepath, std::fstream::in);

    if(playlist.is_open())
    {
        while(getline(playlist ,line))
        {
            //use stringstream to look through the lines in the csv file
            std::stringstream ss(line);
            
            //save tracktitles (first part of the line) into vector
            getline(ss, _trackTitles, ',');
            trackTitles.push_back(_trackTitles);

            //save trackurl (second part of the line) into vector
            getline(ss, _trackURL, ',');
            trackURL.push_back(URL(_trackURL));

            //save tracklength (last part of the line) into vector
            getline(ss, _trackLength, ',');
            trackLength.push_back(_trackLength);
        }
    }

    playlist.close();
}

/** save the playlist */
void PlaylistComponent::savePlaylist()
{
    //open playlist for writing
    std::fstream playlist;
    playlist.open(filepath, std::fstream::out);
    
    //save the strings in the vector to the csv file
    for(int i = 0; i < trackURL.size(); ++i)
    {
        playlist << trackTitles[i] << "," << trackURL[i].toString(false) << "," << trackLength[i] << "\n";
    }

    playlist.close();
}

/** returns the row where the search track title is at */
int PlaylistComponent::searchText(juce::String inputText)
{
    //creates a vector of strings which is an iterator, that searches through the trackTitles vector that matches the inputText
    std::vector<juce::String>::iterator iter = std::find_if(trackTitles.begin(),
                                                            trackTitles.end(),
                                                            [&inputText](const juce::String& tracks) {
                                                                return tracks.contains(inputText);
                                                            }
                                                            );
    
    //if the iterator have not reached the end of trackTitles vector,
    //get the distance between the start of trackTitles vector and the iter
    //this would than return the row where the searched track is at
    //else, return 0
    if(iter != trackTitles.end())
    {
        int rowNumber = std::distance(trackTitles.begin(), iter);
        return rowNumber;
    }
    else
    {
        return 0;
    }
}

/** calls searchText, and selects the row of the searched track title */
void PlaylistComponent::returnSearch(juce::String inputText)
{
    //if there is an inputText, calls searchText function and parse the inputText in
    //select the row in the table where the searched track is at
    //otherwise ddeselect all rows
    if(inputText != "")
    {
        int rowNumber = searchText(inputText);
        tableComponent.selectRow(rowNumber);
    }
    else
    {
        tableComponent.deselectAllRows();
    }
}
