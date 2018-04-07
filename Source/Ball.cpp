/*
  ==============================================================================

    Ball.cpp
    Created: 27 Oct 2017 10:43:21pm
    Author:  Corey Ford

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Ball.h"

//Constructor and destructor methods ===========================================
Ball::Ball()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    //set ball position
    ball.setSize(20, 20);
    
    //set ball physics variables
    delta.setX(5);
    delta.setY((float)(arc4random() % 11) - 5.f);
}

Ball::~Ball(){}

//=Graphics Methods=========================================================
void Ball::paint (Graphics& g)
{
    g.setColour(Colours::red);
    g.drawEllipse(ball, 10.f);
    g.fillEllipse(ball);
}

void Ball::resized(){}

//=Self-Made Methods=========================================================
void Ball::initialiseStartPosition(Rectangle<int> all)
{
    ball.setPosition(all.getWidth()/2.f, all.getHeight()/2.f);
}

Rectangle<float> Ball::getRectangle()
{
    return ball;
}

int Ball::checkIfScore(Rectangle<int> all)
{
    //NOTE: the user controlled paddle is on the right hand side
    //create areas of possible collision (past the paddle's)
    Rectangle<float> x = all.toFloat();
    
    Rectangle<float> leftScoreArea = x;
    leftScoreArea.removeFromRight(x.getWidth() * 0.87);
    Rectangle<float> rightScoreArea = x;
    rightScoreArea.removeFromLeft(x.getWidth() * 0.90);
    
    //see if player looses (i.e. ball on the right)
    if(rightScoreArea.intersects(ball) == true)
    {
        // cout<<"Loose!"<<endl;
        return (-1);
    }
    
    //see if player wins(i.e. ball on the left)
    if(leftScoreArea.intersects(ball) == true)
    {
        //cout<<"Win!"<<endl;
        return +1;
    }
    
    return 0;
}

void Ball::checkWallCollisions(Rectangle<int> all)
{
    //get the top and bottom regions
    Rectangle<float> y = all.toFloat();
    Rectangle<float> top = y.removeFromBottom(y.getHeight() * 0.01);
    Rectangle<float> bottom = y.removeFromTop(y.getHeight() * 0.01);
    
    //if ball collides with these regions
    if(top.intersects(ball) == true || bottom.intersects(ball) == true)
    {
        //jassert(false);
        //invert the y movement
        delta.setY(delta.getY() * -1);
    }
}

void Ball::paddleCollisions(Rectangle<float> rightPaddle, Rectangle<float> leftPaddle)
{
    //ball collides with a paddle
    if(rightPaddle.intersects(ball) == true || leftPaddle.intersects(ball) == true)
    {
        //invert the x movement
        delta.setX(delta.getX() * -1);
        
        //pick a new random y movement
        delta.setY((float)(arc4random() % 11) - 5.f);
    }
}

void Ball::doTranslation()
{
    ball.translate(delta.getX(), delta.getY());
}

void Ball::doCustomTranslation(int x, int y)
{
    ball.translate(x,y);
}

Rectangle<float> Ball::returnRectangle()
{
    return ball;
}

void Ball::newRandomDirection()
{
    delta.setY((float)(arc4random() % 11) - 5.f);
}
