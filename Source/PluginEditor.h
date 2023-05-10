/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/PanningWindow.h"
#include "GUI/Data/CustomColors.h"
#include "GUI/LAF/ButtonLAF.h"
#include "GUI/LAF/VerticalSliderLAF.h"

//==============================================================================
/**
*/
class QuasoLocalizationPannerAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener, public juce::Label::Listener
{
public:
    QuasoLocalizationPannerAudioProcessorEditor (QuasoLocalizationPannerAudioProcessor&);
    ~QuasoLocalizationPannerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    //===============================================================================
    void buttonClicked(juce::Button* button) override;
    void labelTextChanged(juce::Label* labelThatHasChanged) override;
    //===============================================================================
    void setTextButtonProperties();
    void setSliderProperties();
    void setGroupProperties();
    void setPanningWindowProperties();
    void attachComponents();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    QuasoLocalizationPannerAudioProcessor& audioProcessor;

    //Draggable Components
    DraggableComponent _dragComp1;
    DraggableComponent _dragComp2;
    DraggableComponent _dragComp3;
    DraggableComponent _dragComp4;
    DraggableComponent _dragComp5;
    DraggableComponent _dragComp6;
    DraggableComponent _dragComp7;
    DraggableComponent _dragComp8;
    DraggableComponent _dragComp9;
    DraggableComponent _dragComp10;
    DraggableComponent _dragComp11;
    DraggableComponent _dragComp12;
    DraggableComponent _dragComp13;
    DraggableComponent _dragComp14;
    DraggableComponent _dragComp15;
    DraggableComponent _dragComp16;
    std::vector<DraggableComponent*> _dragComponents = 
    { 
      &_dragComp1, &_dragComp2, &_dragComp3, &_dragComp4,
      &_dragComp5, &_dragComp6, &_dragComp7, &_dragComp8,
      &_dragComp9, &_dragComp10, &_dragComp11, &_dragComp12,
      &_dragComp13, &_dragComp14, &_dragComp15, &_dragComp16
    };

    //Panning Window
    PanningWindow panningWindow;

    //Custom Look&Feel for the buttons
    ButtonLAF buttonLookAndFeel;

    //Buttons Group
    juce::GroupComponent buttonsGroup;

    //Text Buttons
    juce::TextButton textButton1;
    juce::TextButton textButton2;
    juce::TextButton textButton3;
    juce::TextButton textButton4;
    juce::TextButton textButton5;
    juce::TextButton textButton6;
    juce::TextButton textButton7;
    juce::TextButton textButton8;
    juce::TextButton textButton9;
    juce::TextButton textButton10;
    juce::TextButton textButton11;
    juce::TextButton textButton12;
    juce::TextButton textButton13;
    juce::TextButton textButton14;
    juce::TextButton textButton15;
    juce::TextButton textButton16;
    std::vector<juce::TextButton*> textButtons
    { 
      &textButton1, &textButton2, &textButton3 , &textButton4,
      &textButton5, &textButton6, &textButton7 , &textButton8,
      &textButton9, &textButton10,&textButton11, &textButton12,
      &textButton13, &textButton14,&textButton15, &textButton16
    };
    std::vector< std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>> textButtonsAttach;

    //Logo Image
    juce::Image logoImage;

    //Output Slider
    VerticalSliderLAF verticalSliderLookAndFeel;
    juce::Slider outputSlider;
    juce::Label outputSliderLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputSliderAttach;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (QuasoLocalizationPannerAudioProcessorEditor)
};
