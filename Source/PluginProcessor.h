#pragma once
#include <JuceHeader.h>
#include <array>
struct Patch { const char* name; float drive, fm, detune, cutoff, movement, release, tone, width; int shape; float punch=0.f, rasp=0.f; };
extern const std::array<Patch,30> factoryPatches;
class BassforgeProcessor : public juce::AudioProcessor {
public:
 BassforgeProcessor();
 void prepareToPlay(double,int) override;
 void releaseResources() override {}
 bool isBusesLayoutSupported(const BusesLayout& l) const override {return l.getMainOutputChannelSet()==juce::AudioChannelSet::stereo();}
 void processBlock(juce::AudioBuffer<float>&,juce::MidiBuffer&) override;
 juce::AudioProcessorEditor* createEditor() override;
 bool hasEditor() const override {return true;}
 const juce::String getName() const override {return "BASSFORGE";}
 bool acceptsMidi() const override {return true;}
 bool producesMidi() const override {return false;}
 bool isMidiEffect() const override {return false;}
 double getTailLengthSeconds() const override {return 2.0;}
 int getNumPrograms() override {return 30;}
 int getCurrentProgram() override {return currentProgram;}
 void setCurrentProgram(int i) override;
 const juce::String getProgramName(int i) override;
 void changeProgramName(int,const juce::String&) override {}
 void getStateInformation(juce::MemoryBlock&) override;
 void setStateInformation(const void*,int) override;
 juce::AudioProcessorValueTreeState params;
 juce::MidiKeyboardState keyboardState;
 static juce::AudioProcessorValueTreeState::ParameterLayout layout();
 juce::var exportPreset(const juce::String& name) const;
 bool importPreset(const juce::var& preset, juce::String& error);
 bool validatePreset(const juce::var& preset,juce::String& error) const;
 void setPatchDisplayName(const juce::String& name) {patchDisplayName=name;}
 juce::String getPatchDisplayName() const {return patchDisplayName;}
 juce::String bankArchivePath; // Saved with project for automatic browser restoration.


private:
 struct Voice {bool active=false, releasing=false; int note=0; double phase=0, phase2=0, osc2phase=0, subPhase=0, lfo=0; float env=0, velocity=0, lpL=0,lpR=0; double age=0, bodyLP=0, charHP=0, charPrev=0; std::array<float,9> decim{}; int decimIndex=0; std::array<double,3> modPhase{}; std::array<float,3> modHold{};};
 std::array<Voice,12> voices{};
 double sampleRateHz=44100;
 int currentProgram=0;
 juce::String patchDisplayName="01 Reactor";
 public:
 static constexpr int fxCount=7;
 static const char* fxNames[fxCount];
 int getFxAt(int slot) const { return fxOrder[(size_t)slot].load(); }
 void moveFx(int slot, int delta);
 private:
 std::array<std::atomic<int>,fxCount> fxOrder{};
 std::array<float,2> lpFx{}, hpFx{}, chorusLp{}, compEnv{}, revLp{};
 std::array<float,2> eqLow{}, eqHigh{};
 std::array<std::vector<float>,2> delayBuffer, reverbBuffer, chorusBuffer;
 int delayPos=0, reverbPos=0, chorusPos=0;
 double fxClock=0.0;
 float processFx(int effect, float x, int ch, float intensity, float sampleIndex, float rate);
 void noteOn(int,float);
 void noteOff(int);
 JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BassforgeProcessor)
};
