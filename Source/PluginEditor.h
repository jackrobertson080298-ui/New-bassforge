#pragma once
#include "PluginProcessor.h"
class BassforgeEditor : public juce::AudioProcessorEditor, private juce::Timer {
public:
 explicit BassforgeEditor(BassforgeProcessor&);
 ~BassforgeEditor() override;
 void paint(juce::Graphics&) override;
 void resized() override;
private:
 class ForgeLookAndFeel : public juce::LookAndFeel_V4 {
 public:
  ForgeLookAndFeel();
  void drawRotarySlider(juce::Graphics&,int,int,int,int,float,float,float,juce::Slider&) override;
  void drawButtonBackground(juce::Graphics&,juce::Button&,const juce::Colour&,bool,bool) override;
 };
 ForgeLookAndFeel theme;
 BassforgeProcessor& processor;
 juce::ComboBox presets;
 struct BrowserPatch { juce::String name; juce::var data; };
 std::vector<BrowserPatch> bankPatches;
 juce::String bankName;
 bool updatingBrowser=false;
 juce::Label currentPatchLabel;
 void rebuildBrowser(int selectedId);
 bool openBank(const juce::File& file, bool showErrors);
 std::array<juce::Slider,8> partialSliders;
 std::array<juce::Label,8> partialLabels;
 std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,8> partialAttachments;
 std::array<juce::Slider,3> advancedOscSliders;
 std::array<juce::Label,3> advancedOscLabels;
 std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,3> advancedOscAttachments;
 juce::ComboBox osc2Wave;
 std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc2WaveAttachment;
 void selectBrowserPatch(int id);
 void importAndDisplay(const juce::var& data, const juce::String& fallbackName);
 void timerCallback() override;

 juce::TextButton savePreset{"SAVE PRESET"}, loadPreset{"LOAD PRESET"}, loadPack{"LOAD PACK"};
 std::unique_ptr<juce::FileChooser> chooser;
 void loadPresetFile(const juce::File& file);
 void refreshControls();
 juce::Label title, subtitle;
 class ForgeKeyboard : public juce::MidiKeyboardComponent {
 public:
  explicit ForgeKeyboard(juce::MidiKeyboardState& state) : juce::MidiKeyboardComponent(state, horizontalKeyboard) {}
  void drawWhiteNote(int note, juce::Graphics& g, juce::Rectangle<float> area, bool down, bool over, juce::Colour, juce::Colour) override;
  void drawBlackNote(int note, juce::Graphics& g, juce::Rectangle<float> area, bool down, bool over, juce::Colour) override;
 };
 ForgeKeyboard keyboard{processor.keyboardState};
 std::array<juce::Slider,12> knobs;
 std::array<juce::Label,12> labels;
 std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,12> attachments;
 juce::ComboBox shape;
 std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> shapeAttachment;
 juce::TextButton synthTab{"SYNTH"}, fxTab{"FX RACK"}, lfoTab{"LFO"}, engineTab{"ENGINE"}, oscTab{"OSC"};
 int activePage=4;
 std::array<juce::ComboBox,3> lfoShape,lfoRate,lfoMode,lfoTarget;
 std::array<juce::Slider,3> lfoDepth,lfoKeytrack;
 std::array<juce::Label,3> lfoTitles;
 std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>,3> lfoShapeAttach,lfoRateAttach,lfoModeAttach,lfoTargetAttach;
 std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,3> lfoDepthAttach,lfoKeytrackAttach;
 std::array<juce::Slider,3> oscSliders;
 std::array<juce::Label,3> oscLabels;
 std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,3> oscAttachments;
 std::array<juce::ComboBox,1> oscWave;
 std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveAttachment;
 std::array<juce::Slider,5> engineSliders;
 std::array<juce::Label,5> engineLabels;
 std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,5> engineAttachments;
 juce::ToggleButton engineEnable{"DUAL LAYER ENGINE"};
 std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> engineEnableAttach;
 juce::ToggleButton precision{"PRECISION ENGINE"};
 std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> precisionAttach;
 juce::ToggleButton bypass{"BYPASS ALL FX"};
 std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttach;
 std::array<juce::Label,7> fxLabels;
 std::array<juce::ToggleButton,7> fxEnabled;
 std::array<juce::Slider,7> fxMix,fxAmount;
 std::array<juce::TextButton,7> fxUp,fxDown;
 std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>,7> fxEnabledAttach;
 std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,7> fxMixAttach,fxAmountAttach;
 bool showFx=false;
 void refreshFxLabels();
 void updatePage();
 JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BassforgeEditor)
};
