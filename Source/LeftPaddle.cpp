/*
 ==============================================================================
 
 LeftPaddle.cpp
 Created: 27 Oct 2017 10:00:52pm
 Author:  Corey Ford
 
 ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"
#include "LeftPaddle.h"

//=Constructor and destructor methods=======================================
LeftPaddle::LeftPaddle()
{
    leftPaddle.setSize(20, 70);
    timerToggle = true;
    startTimer(250); //user presses button every half a seconds
}

LeftPaddle::~LeftPaddle(){}

//=Graphics Methods=========================================================
void LeftPaddle::paint (Graphics& g)
{
    g.setColour(Colours::white);
    g.drawRect(leftPaddle);
    g.fillRect(leftPaddle);
}

void LeftPaddle::resized(){}

//=Self-Made Methods=========================================================
void LeftPaddle::initialiseStartPosition(Rectangle<int> all)
{
    Rectangle<float> x = all.toFloat();
    x.removeFromLeft(x.getWidth() * 0.10);
    leftPaddle.setPosition(x.getX()+20.f, (x.getHeight()/2.f) - 35.f);
}

Rectangle<float> LeftPaddle::returnRectangle()
{
    return leftPaddle;
}

void LeftPaddle::AI(Rectangle<float> ball)
{
    const int aiSpeed = 5;
    
    if(timerToggle == 0) //simulating button press
    {
        //move paddle towards ball with a random error amount
        int randomError = (arc4random() % (8));
        if(ball.getY() < leftPaddle.getCentreY())
        {
            //paddle goes up
            leftPaddle.translate(0, -(aiSpeed+randomError));
        }
        else
        {
            //paddle goes down
            leftPaddle.translate(0, aiSpeed+randomError);
        }
    }
}

bool LeftPaddle::checkForGlitchWithBall(Rectangle<float> ball)
{
    static float startTime = Time::getMillisecondCounterHiRes();
    float elapsedTime = Time::getMillisecondCounterHiRes() - startTime;
    //std::cout << elapsedTime << std::endl; //usefull for debugging
    
    /*if the ball is inside the paddle & has been for too long
     ....i.e. the ball is stuck*/
    if(leftPaddle.intersects(ball) == true && elapsedTime > 5000.f)
    {
        startTime = Time::getMillisecondCounterHiRes();
        return true;
    }
    return false;
}

//=Timer Methods================================================================
void LeftPaddle::timerCallback()
{
    //simulates how long it takes the pretend "user" to press the button
    timerToggle = !timerToggle;
}
