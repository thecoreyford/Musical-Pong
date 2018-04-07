/*
  ==============================================================================

    LeftPaddle.h
    Created: 27 Oct 2017 10:00:52pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class LeftPaddle  : public Component, public Timer
{
public:
    //=Constructor and destructor methods=======================================
    LeftPaddle();
    ~LeftPaddle();

    //=Graphics Methods=========================================================
    void paint (Graphics&) override;
    void resized() override;

    //=Self-Made Methods=========================================================
    void initialiseStartPosition(Rectangle<int> all);
    Rectangle<float> returnRectangle();
    void AI(Rectangle<float> ball);
    bool checkForGlitchWithBall(Rectangle<float> ball);
    
    //=Timer Methods============================================================
    virtual void timerCallback() override;
    
private:
    Rectangle<float> leftPaddle;
    bool timerToggle; //simulates user button press time
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LeftPaddle)
};
