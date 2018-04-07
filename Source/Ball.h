/*
  ==============================================================================

    Ball.h
    Created: 27 Oct 2017 10:43:21pm
    Author:  Corey Ford

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class Ball    : public Component
{
public:
    //=Constructor and destructor methods=======================================
    Ball();
    ~Ball();

    //=Graphics Methods=========================================================
    void paint (Graphics&) override;
    void resized() override;
    
    //=Self-Made Methods=========================================================
    void initialiseStartPosition(Rectangle<int> all);
    Rectangle<float> getRectangle();
    int checkIfScore(Rectangle<int> all);
    void checkWallCollisions(Rectangle<int> all);
    void paddleCollisions(Rectangle<float> rightPaddle, Rectangle<float> leftPaddle);
    void doTranslation();
    void doCustomTranslation(int x, int y);
    Rectangle<float> returnRectangle();
    void newRandomDirection();

private:
    Rectangle<float> ball;
    Point<int> delta;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Ball)
};
