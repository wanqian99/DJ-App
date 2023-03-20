/*
  ==============================================================================

    PlaylistComponent.h
    Created: 1 Jan 2022 1:04:36pm
    Author:  Gao WanQian

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DeckGUI.h"
#include <fstream>
#include <filesystem>
#include <sstream>

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public TableListBoxModel,
                           public Button::Listener,
                           public TextEditor::Listener,
                           public FileDragAndDropTarget
{
public:
    /** DeckGUI functions are used to load files from playlist */
    PlaylistComponent(DeckGUI* _deckGUI1, DeckGUI* _deckGUI2, AudioFormatManager& _formatManager);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /** return the number of rows currently in the table */
    int getNumRows() override;
    
    /** draw the background behind one of the rows in the table */
    void paintRowBackground(Graphics &, int rowNumber, int width,
                            int height, bool rowIsSelected) override;
    
    /** draw one of the cells */
    void paintCell(Graphics &, int rowNumber, int columnId, int width,
                   int height, bool rowIsSelected) override;

    /** used to create or update a custom component to go in a cell */
    Component* refreshComponentForCell(int rowNumber, int columnid, bool isRowSelected,
                                       Component *existingComponentToUpdate) override;
    
    void buttonClicked(Button* button) override;
    
    /** implement FileDragAndDropTarget */
    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override;
    
private:
    
    TableListBox tableComponent;
    
    TextButton savePlaylistButton{"Save Playlist"};
    TextButton loadPlaylistButton{"Load Playlist"};
    TextEditor searchBox;
    
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    
    /** vectors that contains track titles, url, and length */
    std::vector<juce::String> trackTitles;
    std::vector<URL> trackURL;
    std::vector<juce::String> trackLength;
    
    /** playlist component needs this to create a reader
     that gives access to the audio file metadata */
    AudioFormatManager& formatManager;
    
    /** calculates the length of the track with the audio file parsed in */
    std::string getTrackLength(File track);
    
    /** load the playlist */
    void loadPlaylist();
    /** saves the playlist */
    void savePlaylist();
    
    /** returns the row where the search track title is at */
    int searchText(juce::String inputText);
    
    /** calls searchText, and selects the row of the searched track title */
    void returnSearch(juce::String searchText);
    
    //set the file path to be the user's music directory
    //works cross-platform
    //saves playlist data into Playlist.csv
    std::string filepath = (juce::File::getSpecialLocation(juce::File::SpecialLocationType::userMusicDirectory).getFullPathName() + "/Playlist.csv").toStdString();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
