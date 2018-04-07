/*
  ==============================================================================

    RightPaddle.h
    Created: 26 Oct 2017 11:54:26pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class RightPaddle : public Component
{
public:
    //=Constructor and destructor methods=======================================
    RightPaddle();
    ~RightPaddle();

    //=Graphics Methods=========================================================
    void paint (Graphics&) override;
    void resized() override;
    
    //=Self-Made Methods========================================================
    void initialiseStartPosition(Rectangle<int> all);
    Rectangle<float> returnRectangle();
    void doTranslation(float amount);
    
private:
    Rectangle<float> rightPaddle;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RightPaddle)
};
