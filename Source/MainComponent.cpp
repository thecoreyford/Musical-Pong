/*
 ==============================================================================
 
 This file was auto-generated!
 Created: 31 Oct 2017 08:42:20pm
 Author:  Corey Ford
 
 //A read me file is sent as part of the zip file.
 
 //INFO: I originally demo-ed this using an animated app component, and then
 had to copy the classes across to a AudioAppComponent.
 Hence, how the main component was made after the child components.
  ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"
#include "Ball.h"
#include "RightPaddle.h"
#include "LeftPaddle.h"
#include "MusicComponent.h"

//==============================================================================
/*
 This component lives inside our window, and this is where you should put all
 your controls and content.
 */
class MainContentComponent   : public AudioAppComponent, public KeyListener, public Timer, public Slider::Listener
{
public:
    //==Constructor & Destructor================================================
    MainContentComponent()
    {
        setSize (750, 750);
        
        //set rectangles inital properties
        all = getLocalBounds();
        ball.initialiseStartPosition(all);
        rightPaddle.initialiseStartPosition(all);
        leftPaddle.initialiseStartPosition(all);
        
        //set volume control
        volume.setBounds(all.removeFromTop(80));
        volume.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        volume.setRange(0.0, 1.0, 0.01);
        volume.setValue(0.2);
        
        //set game control variables
        score = 0;
        roundNo = 1;
        startGame = false;
        pointWon = false;
        
        //add and make visible
        addAndMakeVisible(ball);
        addAndMakeVisible(rightPaddle);
        addAndMakeVisible(leftPaddle);
        addAndMakeVisible(volume);
        startTimer(33); //equivalent to setFramesPerSecond(30) when using an animated app component.
        
        //include key listener
        volume.addListener(this);
        addKeyListener(this);
        setWantsKeyboardFocus(true);
        hasKeyboardFocus(true);
        
        //set audio information
        setAudioChannels(2, 2);
    }
    
    ~MainContentComponent()
    {
        shutdownAudio();
    }
    
    //==Virtuals from KeyListener===============================================
    bool keyPressed(const KeyPress &key, Component* originatingComponent) override
    {
        //start game
        if(key == KeyPress::spaceKey && roundNo != 4)
        {
            if(startGame == false)
                ball.newRandomDirection();
            
            startGame = true;
        }
        
        //restart game
        if(key == KeyPress::returnKey && roundNo == 4)
        {
            startGame=false;
            roundNo = 1;
            score = 0;
            repaint();
        }
        
        //control right paddle
        const int moveAmount = 20;
        if(key == KeyPress::upKey)
            rightPaddle.doTranslation(-moveAmount);
        
        if(key == KeyPress::downKey)
            rightPaddle.doTranslation(moveAmount);
        
        return true;
    }
    
    //==Virtuals from Timer=====================================================
    void timerCallback() override
    {
        if(startGame == true && roundNo != 4)
        {
            //--------------do left paddle ai-------------//
            leftPaddle.AI(ball.getRectangle());
            if(leftPaddle.checkForGlitchWithBall(ball.returnRectangle()) == true)
            {
                ball.doCustomTranslation(5, 0);
            }
            
            //--------------do ball checks-------------//
            ball.doTranslation();
            ball.checkWallCollisions(all);
            ball.paddleCollisions(rightPaddle.returnRectangle(), leftPaddle.returnRectangle());
            
            //--------------do game control checks-------------//
            checkForPoints();
        }
        
        //always redraw -> hence works like an animated component
        repaint();
    }
    
    //==Virtuals from Slider Listener===========================================
    virtual void sliderValueChanged (Slider* slider) override
    {
        if(slider == &volume)
        {
            amplitude = volume.getValue();
            amplitude = amplitude * amplitude * amplitude;
        }
    }
    
    //==Audio Handling Functions================================================
    /*
        Code in this section derrived and inspired from (The Audio Programmer, 2017)
     
        Reference:
        -------------
        The Audio Programmer. (2017). Juce Tutorial 11- Basic Wavetable Synthesis in Juce. [Online Video]. 22 August 2017. 
        Available from: https://www.youtube.com/watch?v=Zz-gDMo_E8c. [Accessed: 4 November 2017].
     */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        //initialise audio variables
        wtPosRight = 0;
        wtPosLeft = 0;
        wtSize = 1024;
        sampRate = sampleRate;
        updateFrequency();
        
        //populate one cycle of a sine wave for each wave table
        for(int i = 0; i < wtSize; i++)
        {
            waveTableRight.insert(i, sin(2.0 * M_PI * i / wtSize));
            waveTableLeft.insert(i, sin(2.0 * M_PI * i / wtSize));
        }
    }
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        //get first position in buffer, so we can write to that speaker
        float* const leftSpeaker = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
        float* const rightSpeaker = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
        
        //cycle through samples
        for(int sample = 0; sample < bufferToFill.numSamples; ++sample) //count increases on this line
        {
            //do stuff at sample level
            leftSpeaker[sample] = waveTableLeft[(int)wtPosLeft];
            rightSpeaker[sample] = waveTableRight[(int)wtPosRight];
            
            //distort signal (left speaker only ∴ right speaker is more like a melody).
            leftSpeaker[sample] = music.distort(leftPaddle.returnRectangle(), leftSpeaker[sample]);
            
            //master gain (controlled by slider)
            leftSpeaker[sample]*=amplitude;
            rightSpeaker[sample]*=amplitude;
            
            //check for clipping
            jassert(leftSpeaker[sample] <= 1.0 && leftSpeaker[sample] >= -1.0);
            jassert(rightSpeaker[sample] <= 1.0 && rightSpeaker[sample] >= -1.0);
            
            updateFrequency();
        }
    }
    
    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.
        
        // For more details, see the help for AudioProcessor::releaseResources()
    }
    
    void updateFrequency()
    {
        //--------------do for the right input-------------//
        //get the high melody current freq for this sample...
        freqRight = music.getHighMelody(ball.returnRectangle(), rightPaddle.returnRectangle());
        //set the increment depending on the frequency value
        incrementRight = (freqRight * wtSize) / sampRate;
        //if position in wavetable is over the index, wrap around
        wtPosRight = fmod((wtPosRight + incrementRight), wtSize);
        
        //--------------do for the left input-------------//
        //get the low melody current freq for this sample...
        freqLeft = music.getLowMelody(ball.returnRectangle());
        //set the increment depending on the frequency value
        incrementLeft = (freqLeft * wtSize) / sampRate;
        //if position in wavetable is over the index, wrap around
        wtPosLeft = fmod((wtPosLeft + incrementLeft), wtSize);
    }
    
    //==Graphics Virtuals=======================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
        
        // You can add your drawing code here!
        ball.paint(g);
        rightPaddle.paint(g);
        leftPaddle.paint(g);
        volume.paint(g);
        
        //draw text for score
        String str = "Score: " + std::to_string(score);
        g.drawText(str, getWidth() - 80, 0, 60, 60, Justification::topRight);
        
        //draw text for game over
        if(roundNo == 4)
        {
            g.setColour(Colours::red);
            g.drawText("Game Over!", 0, 0, 200, 80, Justification::topLeft);
            g.drawText("Press Enter to restart game or close the application.", 0, 15, 500, 80, Justification::topLeft);
        }
        else
        {
            //draw text for round information
            if(startGame == false)
            {
                str = "Press spacebar to start round " + std::to_string(roundNo) + "/3!";
                g.drawText(str, 0, 0, 200, 80, Justification::topLeft);
                if(roundNo == 1)
                {
                    g.drawText("Use the up and down arrow keys to control the right paddle!", 0, 15, 500, 80, Justification::topLeft);
                }
            }
            else
            {
                str = "Round: " + std::to_string(roundNo) + "/3";
                g.drawText(str, 0, 0, 160, 60, Justification::topLeft);
            }
        }
    }
    
    void resized() override {}//NOTE: fixed size of screen regardless.
    
    //=Game Score Control Functions=============================================
    void checkForPoints()
    {
        if(ball.checkIfScore(all) == 1)
        {
            //user gained point
            startGame = false;
            ball.initialiseStartPosition(all);
            rightPaddle.initialiseStartPosition(all);
            leftPaddle.initialiseStartPosition(all);
            roundNo++;
            score++;
        }
        
        if(ball.checkIfScore(all) == -1)
        {
            //ai gained point
            startGame = false;
            ball.initialiseStartPosition(all);
            rightPaddle.initialiseStartPosition(all);
            leftPaddle.initialiseStartPosition(all);
            roundNo++;
            score--;
        }
    }
    
    void checkForGameOver()
    {
        if(roundNo == 4)
        {
            startGame = false;
            ball.initialiseStartPosition(all);
            rightPaddle.initialiseStartPosition(all);
            leftPaddle.initialiseStartPosition(all);
            removeKeyListener(this);
        }
    }
    
private:
    //=Private member variables=================================================
    Rectangle<int> all;
    
    //child components
    Ball ball;
    RightPaddle rightPaddle;
    LeftPaddle leftPaddle;
    
    //game control variables
    int score;
    int roundNo;
    bool startGame;
    bool pointWon;
    
    //audio members
    Array<float> waveTableRight;
    Array<float> waveTableLeft;
    double wtSize, sampRate, amplitude;
    double freqRight, wtPosRight, incrementRight;
    double freqLeft, wtPosLeft, incrementLeft;
    Slider volume;
    MusicComponent music;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
