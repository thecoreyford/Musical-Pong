/*
  ==============================================================================

    MusicComponent.h
    Created: 1 Nov 2017 12:36:30am
    Author:  Corey Ford

  ==============================================================================
*/

/*
 The parameters from pong and how they are translated musically.
 //ball left-to-right (x axis)—> play pitch in a “MIDI esque high range”
 //ball up-down  (y axis)—> play “counter” pitch's in a lower range (using 
                                appregiatic scale)
 //left paddle up-down —> amount of distortion applied to the lower melody
                            (has a cool filtery, retro sound imo)
 //right paddle up-down —> set the scale for the ball's high melody
 
 //This class is not very scientific, so long as the audible result is in
 range then values are left as.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class MusicComponent    : public Component
{
public:
    //=Constructor and destructor methods=======================================
    MusicComponent();
    ~MusicComponent();

    //=High Melody Functions====================================================
    double getHighMelody(Rectangle<float> ball, Rectangle<float> rightPaddle);
    void createMajorScale();
    void createHarmMinorScale();
    void createPhrygianScale();
    void createChromaticScale();
    
    //=Low Melody Functions=====================================================
    double getLowMelody(Rectangle<float> ball);
    void createApreggioRun();
    
    //=Low Melody Distortion====================================================
    float distort(Rectangle<float> paddle, float x);

    //=Graphics Methods=========================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    int major[15], harmMinor[15], phrygian[15], chromatic[24];
    int apreggio[6];
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MusicComponent)
};
//==============================================================================
