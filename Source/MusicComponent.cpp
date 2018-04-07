/*
  ==============================================================================

    MusicComponent.cpp
    Created: 1 Nov 2017 10:36:30am
    Author:  Corey Ford

  ==============================================================================
*/

/*Information about how values are re-interpreted musically in header file.*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MusicComponent.h"

#define major_scale 0
#define harmMinor_scale 1
#define phrygian_scale 2
#define chromatic_scale 3

//=Constructor and destructor methods===========================================
MusicComponent::MusicComponent()
{
    //populate scale arrays
    createMajorScale();
    createHarmMinorScale();
    createPhrygianScale();
    createChromaticScale();
    createApreggioRun();
}

MusicComponent::~MusicComponent(){}

//=High Melody Functions========================================================
double MusicComponent::getHighMelody(Rectangle<float> ball, Rectangle<float> rightPaddle)
{
    int note = 0; //integer so will purposley truncate later values
    
    //current range of rightPaddle Y co-ordinate ≈ -80 to 760
    //convert this to either options 0,1,2 or 3 for scales
    int scaleChoice = ((rightPaddle.getY() / 600.f) * 3);

    //current range of ball X co-ordinate ≈ 100 to 600

    switch(scaleChoice)
    {
        case major_scale:
            //convert note to ≈ range of scale index (0-14) for major array
            note = major[(int)((ball.getX() / 600.f) * 14)];
            break;
        case harmMinor_scale:
            //convert note to ≈ range of scale index (0-14) for harmonic minor array
            note = harmMinor[(int)((ball.getX() / 600.f) * 14)];
            break;
        case phrygian_scale:
            //convert note to ≈ range of scale index (0-14) for phryigian array
            note = phrygian[(int)((ball.getX() / 600.f) * 14)];
            break;
        case chromatic_scale:
            //convert note to ≈ range of scale index (0-23) for chromatic array
            note = chromatic[(int)((ball.getX() / 600.f) * 23)];
            break;
        default:
            //do nothing
            break;
    }
    
    //return midi value converted to frequency
    return (440.0*pow(2.0, (note-69.0)/12.0));
}

void MusicComponent::createMajorScale()
{
    //semitones for major scale across 2 8ves
    //2+2+1+2+2+2+1+2+2+1+2+2+2+1
    
    major[0] = 60; //set root
    for(int i  = 1; i < 15; i++)
    {
        if(i == 3 || i == 7 || i == 10 || i == 14)
        {
            major[i] = major[i - 1] + 1;
        }
        else
        {
            major[i] = major[i - 1] + 2;
        }
    }
}

void MusicComponent::createHarmMinorScale()
{
    //semitones for harmonic minor scale across 2 8ves
    //2+1+2+2+1+3+1+2+1+2+2+1+3+1
    harmMinor[0] = 60; //set root
    for(int i  = 1; i < 15; i++)
    {
        if(i == 2 || i == 5 || i == 7 || i == 9 || i == 12 || i == 14)
        {
            harmMinor[i] = harmMinor[i - 1] + 1;
        }
        else if(i == 6 || i == 13)
        {
            harmMinor[i] = harmMinor[i - 1] + 3;
        }
        else
        {
            harmMinor[i] = harmMinor[i - 1] + 2;
        }
    }
}

void MusicComponent::createPhrygianScale()
{
    //semitones for dominant phyrigian scale across 2 8ves
    //1+3+1+2+1+2+2+1+3+1+2+1+2+2
    phrygian[0] = 60; //set root
    for(int i  = 1; i < 15; i++)
    {
        if(i == 1 || i == 3 || i == 5 || i == 8 || i == 10 || i == 12)
        {
            phrygian[i] = phrygian[i - 1] + 1;
        }
        else if(i == 2 || i == 9)
        {
            phrygian[i] = phrygian[i - 1] + 3;
        }
        else
        {
            phrygian[i] = phrygian[i - 1] + 2;
        }
    }
}

void MusicComponent::createChromaticScale()
{
    //1 semi tone addition
    chromatic[0] = 60; //set root
    for(int i  = 1; i < 24; i++)
    {
        chromatic[i] = chromatic[i - 1] + 1;
    }
}

//=Low Melody Functions=========================================================
double MusicComponent::getLowMelody(Rectangle<float> ball)
{
    //upper bound for scale range ≈ 700
    //convert note to ≈ range of scale index (0-5) for apreggio array
    int note = apreggio[(int)((ball.getY() / 600.f) * 5.f)];

    //return midi value converted to frequency
    return (440.0*pow(2.0, (note-69.0)/12.0));
}

void MusicComponent::createApreggioRun()
{
    apreggio[0] = 50; //root note
    apreggio[1] = 54; //3rd
    apreggio[2] = 57; //5th
    for(int i = 3; i < 6; i++)
    {
        apreggio[i] = apreggio[i - 3] + 12;
    }
    
    //reverse array contents (as co-ordinates are counter intuitive)
    for(int i = 0; i < (3); i++)
    {
        int temporary = apreggio[i];
        apreggio[i] = apreggio[(5) - i];
        apreggio[(5) - i] = temporary;
    }
}

//=Low Melody Distortion========================================================
float MusicComponent::distort(Rectangle<float> paddle, float x)
{
    const int noOfSteps = 7;
    float y = 0;
    
    //current range of left paddle Y co-ordinate ≈ -80 to 760 (inverted)
    //convert this to either options 1 to no of steps
    float distortionAmount = ((600 - paddle.getY()) / 600.f) * (noOfSteps-1) + 2;
    
    //apply clipping
    y = x * (noOfSteps - distortionAmount);

    //clip out of bounds values
    if(y > 1.f)
        y = 1.f;
    if(y < -1.f)
        y = -1.f;
    
    return y;
}

//=Graphics Methods=============================================================
void MusicComponent::paint (Graphics& g){}
void MusicComponent::resized(){}
