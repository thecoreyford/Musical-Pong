/*
  ==============================================================================

    RightPaddle.cpp
    Created: 26 Oct 2017 11:54:26pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "RightPaddle.h"

//=Constructor and destructor methods===========================================
RightPaddle::RightPaddle()
{
    rightPaddle.setSize(20, 70);
}

RightPaddle::~RightPaddle(){}

//=Graphics Methods=============================================================
void RightPaddle::paint (Graphics& g)
{
    g.setColour(Colours::white);
    g.drawRect(rightPaddle);
    g.fillRect(rightPaddle);
}

void RightPaddle::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}

//=Self-Made Methods============================================================
void RightPaddle::initialiseStartPosition(Rectangle<int> all)
{
    Rectangle<float> x = all.toFloat();
    x.removeFromLeft(x.getWidth() * 0.90);
    rightPaddle.setPosition(x.getX()-20.f, (x.getHeight()/2.f) - 35.f);
}

Rectangle<float> RightPaddle::returnRectangle()
{
    return rightPaddle;
}

void RightPaddle::doTranslation(float amount)
{
    rightPaddle.translate(0, amount);
}
