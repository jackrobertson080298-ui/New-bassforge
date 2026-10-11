#include "PluginEditor.h"
namespace {
const auto bg=juce::Colour(0xff0b0f13), panel=juce::Colour(0xff181e23), stroke=juce::Colour(0xff333c42), accent=juce::Colour(0xff42c6c1), muted=juce::Colour(0xff93a2a9), white=juce::Colour(0xffe9f0f2);
void sectionText(juce::Graphics& g, const juce::String& text, juce::Rectangle<int> area) {
 g.setColour(muted); g.setFont(juce::Font(juce::FontOptions(12.f,juce::Font::bold))); g.drawText(text,area,juce::Justification::centredLeft);
}
void panelBox(juce::Graphics& g, juce::Rectangle<float> r) {
 g.setColour(panel);g.fillRoundedRectangle(r,13.f);g.setColour(stroke);g.drawRoundedRectangle(r,13.f,1.f);
}
}
void BassforgeEditor::ForgeKeyboard::drawWhiteNote(int note, juce::Graphics& g, juce::Rectangle<float> area, bool down, bool over, juce::Colour, juce::Colour) {
 g.setColour(down?accent:(over?juce::Colour(0xffd7e4e8):juce::Colour(0xffc1cbd5)));
 g.fillRoundedRectangle(area.reduced(1.f,1.f),3.f);
 g.setColour(juce::Colour(0xff657383));g.drawRoundedRectangle(area.reduced(1.f,1.f),3.f,0.7f);
 if(note%12==0){g.setColour(juce::Colour(0xff405060));g.setFont(juce::Font(juce::FontOptions(10.f,juce::Font::bold)));g.drawText(juce::MidiMessage::getMidiNoteName(note,true,true,3),area.removeFromBottom(17.f),juce::Justification::centred);}
}
void BassforgeEditor::ForgeKeyboard::drawBlackNote(int, juce::Graphics& g, juce::Rectangle<float> area, bool down, bool over, juce::Colour) {
 g.setColour(down?accent:(over?juce::Colour(0xff43566a):juce::Colour(0xff202b39)));
 g.fillRoundedRectangle(area.reduced(.5f,0.f),3.f);g.setColour(juce::Colour(0xff070b10));g.drawRoundedRectangle(area.reduced(.5f,0.f),3.f,1.f);
}
BassforgeEditor::ForgeLookAndFeel::ForgeLookAndFeel(){
 setColour(juce::ComboBox::backgroundColourId,juce::Colour(0xff20292e));
 setColour(juce::ComboBox::outlineColourId,stroke);
 setColour(juce::ComboBox::textColourId,juce::Colours::white);
 setColour(juce::PopupMenu::backgroundColourId,panel);
 setColour(juce::PopupMenu::textColourId,juce::Colours::white);
 setColour(juce::TextButton::textColourOffId,juce::Colours::white);
 setColour(juce::Slider::textBoxTextColourId,muted);
 setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::transparentBlack);
 setColour(juce::Slider::textBoxBackgroundColourId,juce::Colours::transparentBlack);
 setColour(juce::ToggleButton::textColourId,muted);
}
void BassforgeEditor::ForgeLookAndFeel::drawRotarySlider(juce::Graphics& g,int x,int y,int width,int height,float pos,float start,float end,juce::Slider&){
 const float cx=x+width*.5f,cy=y+height*.43f,r=juce::jmin(width,height)*.275f;
 g.setColour(juce::Colour(0xff090e17));g.fillEllipse(cx-r-6,cy-r-6,(r+6)*2,(r+6)*2);
 juce::Path ring;ring.addCentredArc(cx,cy,r+5,r+5,0,start,end,true);
 g.setColour(juce::Colour(0xff354153));g.strokePath(ring,juce::PathStrokeType(4.f,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
 juce::Path value;value.addCentredArc(cx,cy,r+5,r+5,0,start,start+pos*(end-start),true);
 g.setColour(juce::Colour(0xff42c6c1));g.strokePath(value,juce::PathStrokeType(4.f,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
 juce::ColourGradient metal(juce::Colour(0xff4b5b6b),cx-r,cy-r,juce::Colour(0xff151d2a),cx+r,cy+r,false);
 g.setGradientFill(metal);g.fillEllipse(cx-r,cy-r,2*r,2*r);
 g.setColour(juce::Colour(0xff758495));g.drawEllipse(cx-r,cy-r,2*r,2*r,1.f);
 float a=start+pos*(end-start);g.setColour(juce::Colour(0xffc8fffa));g.drawLine(cx+std::sin(a)*r*.34f,cy-std::cos(a)*r*.34f,cx+std::sin(a)*r*.8f,cy-std::cos(a)*r*.8f,3.f);
}
void BassforgeEditor::ForgeLookAndFeel::drawButtonBackground(juce::Graphics& g,juce::Button& b,const juce::Colour&,bool over,bool down){
 auto r=b.getLocalBounds().toFloat().reduced(.5f);auto selected=b.getToggleState();
 g.setColour(selected?juce::Colour(0xff17453f):(down?juce::Colour(0xff2a4c58):over?juce::Colour(0xff253949):juce::Colour(0xff1a2937)));
 g.fillRoundedRectangle(r,7.f);g.setColour(selected?accent:stroke);g.drawRoundedRectangle(r,7.f,1.f);
}

BassforgeEditor::BassforgeEditor(BassforgeProcessor& p):AudioProcessorEditor(&p),processor(p){
 setLookAndFeel(&theme);
 setSize(1360,880);
 setResizable(true,true);setResizeLimits(1200,880,1900,1200);
 synthTab.onClick=[this]{activePage=0;updatePage();};fxTab.onClick=[this]{activePage=1;updatePage();};lfoTab.onClick=[this]{activePage=2;updatePage();};engineTab.onClick=[this]{activePage=3;updatePage();};oscTab.onClick=[this]{activePage=4;updatePage();};addAndMakeVisible(synthTab);addAndMakeVisible(fxTab);addAndMakeVisible(lfoTab);addAndMakeVisible(engineTab);addAndMakeVisible(oscTab);
 title.setText("BASSFORGE",juce::dontSendNotification);title.setFont(juce::Font(juce::FontOptions(29.f,juce::Font::bold)));title.setColour(juce::Label::textColourId,juce::Colours::white);addAndMakeVisible(title);
 subtitle.setText("WAVETABLE / FM SYNTHESIS   •   VERSION 2.1",juce::dontSendNotification);subtitle.setColour(juce::Label::textColourId,muted);addAndMakeVisible(subtitle);
 addAndMakeVisible(presets);
 currentPatchLabel.setColour(juce::Label::textColourId, accent);
 currentPatchLabel.setFont(juce::Font(juce::FontOptions(12.f, juce::Font::bold)));
 addAndMakeVisible(currentPatchLabel);
 rebuildBrowser(p.getCurrentProgram()+1);
 presets.onChange=[this]{if(!updatingBrowser)selectBrowserPatch(presets.getSelectedId());};
 if(processor.bankArchivePath.isNotEmpty())openBank(juce::File(processor.bankArchivePath),false);
 startTimerHz(4);
 addAndMakeVisible(savePreset);addAndMakeVisible(loadPreset);addAndMakeVisible(loadPack);
 savePreset.onClick=[this]{
  chooser=std::make_unique<juce::FileChooser>("Save BASSFORGE preset",juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("My Bass.bfpreset"),"*.bfpreset");
  chooser->launchAsync(juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::warnAboutOverwriting,[this](const juce::FileChooser& c){auto f=c.getResult();if(f==juce::File{})return;f=f.withFileExtension("bfpreset");auto json=juce::JSON::toString(processor.exportPreset(f.getFileNameWithoutExtension()),true);if(!f.replaceWithText(json))juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,"Save failed",f.getFullPathName());});
 };
 loadPreset.onClick=[this]{
  chooser=std::make_unique<juce::FileChooser>("Load BASSFORGE preset",juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),"*.bfpreset");
  chooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,[this](const juce::FileChooser& c){auto f=c.getResult();if(f!=juce::File{})loadPresetFile(f);});
 };
 loadPack.onClick=[this]{
  chooser=std::make_unique<juce::FileChooser>("Load BASSFORGE bank ZIP",juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),"*.zip");
  chooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,[this](const juce::FileChooser& c){
   auto f=c.getResult();if(f==juce::File{})return;
   openBank(f,true);
  });
 };
 const char* ids[]={"drive","fm","detune","cutoff","movement","release","tone","width","sub","output","punch","rasp"};
 for(int i=0;i<12;++i){auto& s=knobs[(size_t)i];s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);s.setTextBoxStyle(juce::Slider::TextBoxBelow,false,65,21);s.setColour(juce::Slider::rotarySliderFillColourId,accent);s.setColour(juce::Slider::thumbColourId,juce::Colours::white);addAndMakeVisible(s);auto& l=labels[(size_t)i];l.setText(juce::String(ids[i]).toUpperCase(),juce::dontSendNotification);l.setJustificationType(juce::Justification::centred);l.setColour(juce::Label::textColourId,juce::Colours::white);addAndMakeVisible(l);attachments[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.params,ids[i],s);}
 shape.addItemList({"Saw","Horn","Metal"},1);addAndMakeVisible(shape);shapeAttachment=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.params,"shape",shape);
 keyboard.setAvailableRange(24,84);keyboard.setKeyWidth(25.f);keyboard.setScrollButtonsVisible(false);keyboard.setVelocity(0.85f,false);addAndMakeVisible(keyboard);
 precisionAttach=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.params,"precision",precision);addAndMakeVisible(precision);
 bypassAttach=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.params,"fxbypass",bypass);addAndMakeVisible(bypass);
 for(int i=0;i<7;++i){
  auto id="fx"+juce::String(i);
  addAndMakeVisible(fxLabels[(size_t)i]);addAndMakeVisible(fxEnabled[(size_t)i]);
  addAndMakeVisible(fxMix[(size_t)i]);addAndMakeVisible(fxAmount[(size_t)i]);
  auto setup=[](juce::Slider& sl){sl.setSliderStyle(juce::Slider::LinearHorizontal);sl.setTextBoxStyle(juce::Slider::TextBoxRight,false,55,20);};
  setup(fxMix[(size_t)i]);setup(fxAmount[(size_t)i]);
  fxEnabledAttach[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.params,id+"on",fxEnabled[(size_t)i]);
  fxMixAttach[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.params,id+"mix",fxMix[(size_t)i]);
  fxAmountAttach[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.params,id+"amt",fxAmount[(size_t)i]);
  fxUp[(size_t)i].setButtonText("UP");fxDown[(size_t)i].setButtonText("DOWN");
  fxUp[(size_t)i].onClick=[this,i]{for(int j=0;j<7;++j)if(processor.getFxAt(j)==i){processor.moveFx(j,-1);break;}refreshFxLabels();};
  fxDown[(size_t)i].onClick=[this,i]{for(int j=0;j<7;++j)if(processor.getFxAt(j)==i){processor.moveFx(j,1);break;}refreshFxLabels();};
  fxMix[(size_t)i].setColour(juce::Slider::trackColourId,accent);fxAmount[(size_t)i].setColour(juce::Slider::trackColourId,accent);
  addAndMakeVisible(fxUp[(size_t)i]);addAndMakeVisible(fxDown[(size_t)i]);
 }
 for(int i=0;i<3;++i){
  auto id="lfo"+juce::String(i+1);
  auto init=[this](juce::ComboBox& cb,const juce::StringArray& items){cb.addItemList(items,1);addAndMakeVisible(cb);};
  init(lfoShape[(size_t)i],{"SINE","TRIANGLE","SAW","SQUARE","RAMP","CUSTOM"});
  init(lfoRate[(size_t)i],{"1/16","1/8","1/4","1/2","1 BAR","2 BARS"});
  init(lfoMode[(size_t)i],{"RETRIGGER","FREE RUN","ONE SHOT"});
  init(lfoTarget[(size_t)i],{"FM","CUTOFF","DRIVE","RASP","MOVEMENT","PITCH"});
  lfoTitles[(size_t)i].setText("LFO 0"+juce::String(i+1),juce::dontSendNotification);
  lfoTitles[(size_t)i].setColour(juce::Label::textColourId,accent);addAndMakeVisible(lfoTitles[(size_t)i]);
  auto& sl=lfoDepth[(size_t)i];sl.setSliderStyle(juce::Slider::LinearHorizontal);sl.setTextBoxStyle(juce::Slider::TextBoxRight,false,55,20);sl.setColour(juce::Slider::trackColourId,accent);addAndMakeVisible(sl);
  lfoShapeAttach[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.params,id+"shape",lfoShape[(size_t)i]);
  lfoRateAttach[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.params,id+"rate",lfoRate[(size_t)i]);
  lfoModeAttach[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.params,id+"mode",lfoMode[(size_t)i]);
  lfoTargetAttach[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.params,id+"target",lfoTarget[(size_t)i]);
  lfoDepthAttach[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.params,id+"depth",sl);
  auto& kt=lfoKeytrack[(size_t)i];kt.setSliderStyle(juce::Slider::LinearHorizontal);kt.setTextBoxStyle(juce::Slider::TextBoxRight,false,55,20);kt.setColour(juce::Slider::trackColourId,accent);addAndMakeVisible(kt);
  lfoKeytrackAttach[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.params,id+"keytrack",kt);
  lfoShape[(size_t)i].onChange=[this]{repaint();};lfoDepth[(size_t)i].onValueChange=[this]{repaint();};
 }
 const char* oscIds[]={"oscratio","oscwarp","oscmix"};
 const char* oscNames[]={"MODULATOR RATIO","PHASE WARP","SAW / METAL BLEND"};
 for(int i=0;i<3;++i){auto& sl=oscSliders[(size_t)i];sl.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);sl.setTextBoxStyle(juce::Slider::TextBoxBelow,false,70,22);addAndMakeVisible(sl);
 auto& label=oscLabels[(size_t)i];label.setText(oscNames[i],juce::dontSendNotification);label.setColour(juce::Label::textColourId,white);label.setJustificationType(juce::Justification::centred);addAndMakeVisible(label);
 oscAttachments[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.params,oscIds[i],sl);
 }
 oscWave[0].addItemList({"SINE","SAW","METAL"},1);addAndMakeVisible(oscWave[0]);oscWaveAttachment=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.params,"oscwave",oscWave[0]);
 for(int i=0;i<8;++i){auto id="partial"+juce::String(i+1);auto& sl=partialSliders[(size_t)i];sl.setSliderStyle(juce::Slider::LinearVertical);sl.setTextBoxStyle(juce::Slider::TextBoxBelow,false,54,19);addAndMakeVisible(sl);auto& label=partialLabels[(size_t)i];label.setText("H"+juce::String(i+1),juce::dontSendNotification);label.setJustificationType(juce::Justification::centred);label.setColour(juce::Label::textColourId,white);addAndMakeVisible(label);partialAttachments[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.params,id,sl);}
 const char* advIds[]={"additivemix","osc2level","osc2tune"};const char* advNames[]={"EDITED WAVE MIX","OSC 2 LEVEL","OSC 2 TUNE"};
 for(int i=0;i<3;++i){auto& sl=advancedOscSliders[(size_t)i];sl.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);sl.setTextBoxStyle(juce::Slider::TextBoxBelow,false,65,20);addAndMakeVisible(sl);auto& label=advancedOscLabels[(size_t)i];label.setText(advNames[i],juce::dontSendNotification);label.setColour(juce::Label::textColourId,white);label.setJustificationType(juce::Justification::centred);addAndMakeVisible(label);advancedOscAttachments[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.params,advIds[i],sl);}
 osc2Wave.addItemList({"SINE","TRIANGLE","SAW","SQUARE"},1);addAndMakeVisible(osc2Wave);osc2WaveAttachment=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.params,"osc2wave",osc2Wave);
 const char* engineIds[]={"bodylevel","characterlevel","split","harmonics","bodyattack"};
 const char* engineNames[]={"BODY LEVEL","CHARACTER LEVEL","CROSSOVER","HARMONIC DENSITY","TRANSIENT"};
 for(int i=0;i<5;++i){auto& sl=engineSliders[(size_t)i];sl.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);sl.setTextBoxStyle(juce::Slider::TextBoxBelow,false,65,21);addAndMakeVisible(sl);
  auto& label=engineLabels[(size_t)i];label.setText(engineNames[i],juce::dontSendNotification);label.setColour(juce::Label::textColourId,juce::Colours::white);label.setJustificationType(juce::Justification::centred);addAndMakeVisible(label);
  engineAttachments[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.params,engineIds[i],sl);
 }
 engineEnableAttach=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.params,"engine10",engineEnable);addAndMakeVisible(engineEnable);
 refreshFxLabels();updatePage();
}
void BassforgeEditor::loadPresetFile(const juce::File& file){
 if(file.getSize()>1000000){juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,"Invalid preset","File too large.");return;}
 auto data=juce::JSON::parse(file.loadFileAsString());
 importAndDisplay(data,file.getFileNameWithoutExtension());
}
void BassforgeEditor::importAndDisplay(const juce::var& data,const juce::String& fallbackName){
 juce::String error;
 if(!processor.importPreset(data,error)){juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,"Preset error",error);return;}
 auto name=data.getProperty("name",juce::var()).toString().trim();
 if(name.isEmpty())name=fallbackName;
 processor.setPatchDisplayName(name);
 rebuildBrowser(0);
 refreshControls();
}
bool BassforgeEditor::openBank(const juce::File& f,bool showErrors){
 juce::ZipFile archive(f);
 if(archive.getNumEntries()>512){if(showErrors)juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,"Bank error","Too many ZIP entries (limit 512).");return false;}
   std::vector<BrowserPatch> loaded;
   juce::StringArray errors;
   for(int i=0;i<archive.getNumEntries();++i){
    auto* entry=archive.getEntry(i);
    if(!entry || !entry->filename.endsWithIgnoreCase(".bfpreset"))continue;
    if(entry->filename.contains("..") || entry->uncompressedSize<=0 || entry->uncompressedSize>256*1024){errors.add("Invalid entry: "+entry->filename);continue;}
    auto stream=std::unique_ptr<juce::InputStream>(archive.createStreamForEntry(i));
    if(!stream){errors.add("Unreadable: "+entry->filename);continue;}
    auto text=stream->readEntireStreamAsString();
    if(text.getNumBytesAsUTF8()>256*1024){errors.add("Oversized: "+entry->filename);continue;}
    auto data=juce::JSON::parse(text);
    juce::String error;
    if(!processor.validatePreset(data,error)){errors.add(entry->filename+": "+error);continue;}
    auto name=data.getProperty("name",juce::var()).toString().trim();
    if(name.isEmpty())name=juce::File(entry->filename).getFileNameWithoutExtension();
    loaded.push_back({name,data});
   }
   if(loaded.empty()){if(showErrors)juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,"Bank error",errors.isEmpty()?"No .bfpreset entries found.":errors.joinIntoString("\n").substring(0,900));return false;}
   bankPatches=std::move(loaded);
   processor.bankArchivePath=f.getFullPathName();
   bankName=f.getFileNameWithoutExtension();
   int selection=1001;
   const auto target=processor.getPatchDisplayName();
   for(size_t i=0;i<bankPatches.size();++i)if(bankPatches[i].name==target){selection=1001+(int)i;break;}
   rebuildBrowser(selection);
   if(showErrors)selectBrowserPatch(selection);
   if(showErrors && !errors.isEmpty())juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,"Bank loaded with warnings",juce::String(bankPatches.size())+" presets loaded; "+juce::String(errors.size())+" skipped.");
 return true;
}
void BassforgeEditor::refreshControls(){
 refreshFxLabels();
 currentPatchLabel.setText(processor.getPatchDisplayName(),juce::dontSendNotification);
}
void BassforgeEditor::rebuildBrowser(int selectedId){
 updatingBrowser=true;
 presets.clear(juce::dontSendNotification);
 for(int i=0;i<30;++i)presets.addItem(factoryPatches[(size_t)i].name,i+1);
 if(!bankPatches.empty()){
  presets.addSeparator();
  for(size_t i=0;i<bankPatches.size();++i)presets.addItem(bankName+" / "+bankPatches[i].name,1001+(int)i);
 }
 presets.setSelectedId(selectedId,juce::dontSendNotification);
 updatingBrowser=false;
 refreshControls();
}
void BassforgeEditor::selectBrowserPatch(int id){
 if(id>=1 && id<=30){processor.setCurrentProgram(id-1);refreshControls();return;}
 const int idx=id-1001;
 if(idx>=0 && idx<(int)bankPatches.size()){
  juce::String error;
  if(!processor.importPreset(bankPatches[(size_t)idx].data,error)){
   juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,"Preset error",error);return;
  }
  processor.setPatchDisplayName(bankPatches[(size_t)idx].name);
  refreshControls();
 }
}
void BassforgeEditor::timerCallback(){
 const auto name=processor.getPatchDisplayName();
 if(currentPatchLabel.getText()!=name)currentPatchLabel.setText(name,juce::dontSendNotification);
}
void BassforgeEditor::refreshFxLabels(){
 for(int i=0;i<7;++i){int fx=i;fxLabels[(size_t)i].setText(juce::String([this,i]{for(int j=0;j<7;++j)if(processor.getFxAt(j)==i)return j+1;return i+1;}())+"  "+BassforgeProcessor::fxNames[fx],juce::dontSendNotification);}
}
void BassforgeEditor::updatePage(){
 showFx=activePage==1;const bool synth=activePage==0,lfo=activePage==2;
 for(auto& x:knobs)x.setVisible(synth);for(auto& x:labels)x.setVisible(synth);shape.setVisible(synth);
 for(int i=0;i<7;++i){fxLabels[(size_t)i].setVisible(showFx);fxEnabled[(size_t)i].setVisible(showFx);fxMix[(size_t)i].setVisible(showFx);fxAmount[(size_t)i].setVisible(showFx);fxUp[(size_t)i].setVisible(showFx);fxDown[(size_t)i].setVisible(showFx);}
 for(int i=0;i<3;++i){lfoShape[(size_t)i].setVisible(lfo);lfoRate[(size_t)i].setVisible(lfo);lfoMode[(size_t)i].setVisible(lfo);lfoTarget[(size_t)i].setVisible(lfo);lfoDepth[(size_t)i].setVisible(lfo);lfoKeytrack[(size_t)i].setVisible(lfo);lfoTitles[(size_t)i].setVisible(lfo);}
 for(auto& x:engineSliders)x.setVisible(activePage==3);for(auto& x:engineLabels)x.setVisible(activePage==3);engineEnable.setVisible(activePage==3);for(auto& x:oscSliders)x.setVisible(activePage==4);for(auto& x:oscLabels)x.setVisible(activePage==4);for(auto& x:oscWave)x.setVisible(activePage==4);for(auto& x:partialSliders)x.setVisible(activePage==4);for(auto& x:partialLabels)x.setVisible(activePage==4);for(auto& x:advancedOscSliders)x.setVisible(activePage==4);for(auto& x:advancedOscLabels)x.setVisible(activePage==4);osc2Wave.setVisible(activePage==4);oscTab.setToggleState(activePage==4,juce::dontSendNotification);precision.setVisible(synth);bypass.setVisible(showFx);engineTab.setToggleState(activePage==3,juce::dontSendNotification);synthTab.setToggleState(synth,juce::dontSendNotification);fxTab.setToggleState(showFx,juce::dontSendNotification);lfoTab.setToggleState(lfo,juce::dontSendNotification);repaint();
}
void BassforgeEditor::paint(juce::Graphics& g){
 const float w=(float)getWidth(), h=(float)getHeight();
 const auto teal=juce::Colour(0xff42c6c1), ink=juce::Colour(0xff0b0f13), tile=juce::Colour(0xff181e23), edge=juce::Colour(0xff333c42), grey=juce::Colour(0xff91a2a9);
 g.fillAll(ink);
 // Compact rack-style header and readable section divisions.
 g.setColour(juce::Colour(0xff171d21));g.fillRect(0,0,getWidth(),96);
 g.setColour(edge);g.drawHorizontalLine(95,0.f,w);
 g.setColour(teal);g.fillRect(0,0,5,96);
 g.setFont(juce::Font(juce::FontOptions(11.f,juce::Font::bold)));g.setColour(grey);
 g.drawText("PATCH / BANK",(int)w-530,5,150,17,juce::Justification::centredLeft);
 // Navigation rail stays visible across all synth pages.
 g.setColour(tile);g.fillRoundedRectangle(12.f,108.f,155.f,h-225.f,5.f);
 g.setColour(edge);g.drawRoundedRectangle(12.f,108.f,155.f,h-225.f,5.f,1.f);
 g.setColour(grey);g.drawText("MODULES",24,117,125,22,juce::Justification::centredLeft);
 g.setColour(teal);g.fillRect(20.f,144.f,3.f,24.f);
 g.setColour(grey);g.setFont(juce::Font(juce::FontOptions(10.f)));
 g.drawText("OSCILLATOR SHAPE",23,513,137,18,juce::Justification::centredLeft);
 g.drawText("PRECISION DSP",23,558,137,18,juce::Justification::centredLeft);
 g.drawText("BASSFORGE  /  2.1",23,(int)h-163,138,22,juce::Justification::centredLeft);
 const float x=178.f, mw=w-x-12.f;
 // Three oscillator visualisation panes. Displays follow current oscillator settings.
 const float gap=7.f, cw=(mw-2.f*gap)/3.f, top=108.f, cardH=236.f;
 const float fm=processor.params.getRawParameterValue("fm")->load();
 const float ratio=processor.params.getRawParameterValue("oscratio")->load();
 const float warp=processor.params.getRawParameterValue("oscwarp")->load();
 const float mix=processor.params.getRawParameterValue("oscmix")->load();
 const float osc2=processor.params.getRawParameterValue("osc2level")->load();
 const int osc2type=(int)processor.params.getRawParameterValue("osc2wave")->load();
 const char* names[]={"OSC A  /  FM CARRIER","OSC B  /  SECONDARY","HARMONIC EDITOR"};
 const char* subs[]={"PHASE MODULATION","MIX / TUNE / WAVE","EIGHT PARTIAL ADDITIVE"};
 for(int k=0;k<3;++k){
  const float left=x+k*(cw+gap);
  g.setColour(tile);g.fillRoundedRectangle(left,top,cw,cardH,4.f);
  g.setColour(edge);g.drawRoundedRectangle(left,top,cw,cardH,4.f,1.f);
  g.setColour(teal);g.fillRect(left,top,3.f,28.f);
  g.setFont(juce::Font(juce::FontOptions(12.f,juce::Font::bold)));g.drawText(names[k],(int)left+13,114,(int)cw-20,22,juce::Justification::centredLeft);
  g.setColour(grey);g.setFont(juce::Font(juce::FontOptions(9.f)));g.drawText(subs[k],(int)left+14,137,(int)cw-22,17,juce::Justification::centredLeft);
  const float px=left+13.f, py=166.f, pw=cw-26.f, ph=125.f;
  g.setColour(juce::Colour(0xff10171a));g.fillRect(px,py,pw,ph);
  g.setColour(edge.withAlpha(.65f));
  for(int j=1;j<6;++j)g.drawVerticalLine((int)(px+j*pw/6.f),py,py+ph);
  for(int j=1;j<4;++j)g.drawHorizontalLine((int)(py+j*ph/4.f),px,px+pw);
  juce::Path path;
  for(int i=0;i<=260;++i){
   const float t=i/260.f, a=t*juce::MathConstants<float>::twoPi*3.f;
   float v=0.f;
   if(k==0){v=std::sin(a+fm*.035f*std::sin(a*(1.5f+ratio*.08f))+warp*.025f*std::sin(a*2.f));}
   else if(k==1){const float phase=t*3.f;const float cyc=phase-std::floor(phase);v=osc2type==0?std::sin(a):osc2type==1?1.f-4.f*std::abs(cyc-.5f):osc2type==2?2.f*cyc-1.f:(cyc<.5f?1.f:-1.f);v*=.25f+.75f*osc2;}
   else {float sum=0.f,scale=0.f;for(int j=1;j<=8;++j){const auto id="partial"+juce::String(j);const float amp=processor.params.getRawParameterValue(id)->load();sum+=amp*std::sin(a*j);scale+=amp;}v=sum/juce::jmax(.8f,scale*.55f);}
   const float xx=px+t*pw,yy=py+ph*.5f-v*ph*.39f;
   if(i==0)path.startNewSubPath(xx,yy);else path.lineTo(xx,yy);
  }
  g.setColour(teal.withAlpha(.12f));g.strokePath(path,juce::PathStrokeType(7.f));
  g.setColour(k==1?juce::Colour(0xffa5d57b):teal);g.strokePath(path,juce::PathStrokeType(1.8f));
  g.setColour(grey);g.setFont(juce::Font(juce::FontOptions(10.f)));
  g.drawText(k==0?"FM / WARP":k==1?"SECOND OSCILLATOR":"ADDITIVE SPECTRUM",(int)left+14,300,(int)cw-28,18,juce::Justification::centredLeft);
  g.setColour(edge);g.drawLine(left+12.f,325.f,left+cw-12.f,325.f);
 }
 // Lower modulation / control deck.
 const float lowerTop=352.f, lowerBottom=h-120.f;
 g.setColour(tile);g.fillRoundedRectangle(x,lowerTop,mw,lowerBottom-lowerTop,4.f);
 g.setColour(edge);g.drawRoundedRectangle(x,lowerTop,mw,lowerBottom-lowerTop,4.f,1.f);
 g.setColour(teal);g.setFont(juce::Font(juce::FontOptions(12.f,juce::Font::bold)));
 g.drawText(activePage==4?"OSCILLATOR CONTROLS  /  DIRECT EDIT":activePage==3?"ENGINE  /  BODY + CHARACTER":activePage==2?"LFO  /  MODULATION":showFx?"FX RACK  /  SIGNAL CHAIN":"SYNTH  /  PERFORMANCE",(int)x+14,361,(int)mw-25,22,juce::Justification::centredLeft);
 if(activePage==4){
  g.setColour(edge);g.drawHorizontalLine(496,x+12.f,x+mw-12.f);
  g.drawHorizontalLine(623,x+12.f,x+mw-12.f);
  g.setColour(grey);g.setFont(juce::Font(juce::FontOptions(10.f,juce::Font::bold)));
  g.drawText("OSC A / FM / WARP",(int)x+18,388,230,16,juce::Justification::centredLeft);
  g.drawText("OSC B / BLEND / TUNING",(int)x+18,510,260,16,juce::Justification::centredLeft);
  g.drawText("HARMONIC PARTIALS  /  DRAG TO EDIT",(int)x+18,636,300,16,juce::Justification::centredLeft);
 }else if(activePage==2){
  for(int j=1;j<3;++j){const float xx=x+j*mw/3.f;g.setColour(edge);g.drawVerticalLine((int)xx,402.f,lowerBottom-10.f);}
 }else if(showFx){
  for(int j=0;j<7;++j){float yy=398.f+j*(lowerBottom-410.f)/7.f;g.setColour(j%2?juce::Colour(0xff20292e):juce::Colour(0xff151b20));g.fillRect(x+8.f,yy,mw-16.f,(lowerBottom-410.f)/7.f-2.f);}
 }
 // Bottom MIDI keyboard and performance strip.
 g.setColour(tile);g.fillRoundedRectangle(12.f,h-109.f,w-24.f,98.f,4.f);
 g.setColour(edge);g.drawRoundedRectangle(12.f,h-109.f,w-24.f,98.f,4.f,1.f);
 g.setColour(teal);g.setFont(juce::Font(juce::FontOptions(12.f,juce::Font::bold)));g.drawText("KEYS",22,(int)h-96,140,18,juce::Justification::centredLeft);
 g.setColour(grey);g.setFont(juce::Font(juce::FontOptions(10.f)));g.drawText("CLICK TO PLAY",22,(int)h-77,145,18,juce::Justification::centredLeft);
}
void BassforgeEditor::resized(){
 const int w=getWidth(),h=getHeight();
 title.setBounds(22,12,325,39);subtitle.setBounds(23,54,360,21);
 presets.setBounds(w-530,27,506,30);
 currentPatchLabel.setBounds(w-720,64,180,24);
 savePreset.setBounds(w-530,64,160,26);loadPreset.setBounds(w-365,64,160,26);loadPack.setBounds(w-200,64,176,26);
 synthTab.setBounds(23,150,132,46);fxTab.setBounds(23,201,132,46);lfoTab.setBounds(23,252,132,46);engineTab.setBounds(23,303,132,46);oscTab.setBounds(23,354,132,46);
 shape.setBounds(23,532,132,29);precision.setBounds(23,579,140,30);bypass.setBounds(23,532,132,29);engineEnable.setBounds(23,532,145,30);
 const float x=178.f,mw=(float)w-x-12.f, lower=(float)h-120.f;
 const float colW=mw/3.f;
 for(int i=0;i<12;++i){const int col=i%6,row=i/6;const float cw=mw/6.f,xx=x+col*cw,yy=407.f+row*(lower-417.f)/2.f;
  knobs[(size_t)i].setBounds((int)(xx+5),(int)yy,(int)cw-10,(int)((lower-417.f)/2.f)-22);
  labels[(size_t)i].setBounds((int)(xx+3),(int)(yy+(lower-417.f)/2.f-24),(int)cw-6,20);
 }
 const float fxRow=(lower-408.f)/7.f;
 for(int i=0;i<7;++i){const int yy=(int)(402.f+i*fxRow);
  fxLabels[(size_t)i].setBounds((int)x+15,yy,145,(int)fxRow-2);fxEnabled[(size_t)i].setBounds((int)x+163,yy+2,35,(int)fxRow-5);
  fxMix[(size_t)i].setBounds((int)x+202,yy+2,(int)(mw*.29f),(int)fxRow-5);fxAmount[(size_t)i].setBounds((int)(x+mw*.56f),yy+2,(int)(mw*.23f),(int)fxRow-5);
  fxUp[(size_t)i].setBounds(w-118,yy+3,41,(int)fxRow-6);fxDown[(size_t)i].setBounds(w-73,yy+3,50,(int)fxRow-6);
 }
 for(int i=0;i<3;++i){const int xx=(int)(x+i*colW+15),ww=(int)colW-30;
  lfoTitles[(size_t)i].setBounds(xx,395,ww,24);lfoShape[(size_t)i].setBounds(xx,438,ww,28);lfoRate[(size_t)i].setBounds(xx,485,ww,28);
  lfoMode[(size_t)i].setBounds(xx,532,ww,28);lfoTarget[(size_t)i].setBounds(xx,579,ww,28);lfoDepth[(size_t)i].setBounds(xx,626,ww,28);lfoKeytrack[(size_t)i].setBounds(xx,672,ww,28);
 }
 for(int i=0;i<5;++i){const int xx=(int)(x+(i%3)*colW+8),yy=425+(i/3)*140;
  engineSliders[(size_t)i].setBounds(xx,yy,(int)colW-16,100);engineLabels[(size_t)i].setBounds(xx,yy+100,(int)colW-16,22);
 }
 for(int i=0;i<3;++i){const int xx=(int)(x+i*colW+12);oscSliders[(size_t)i].setBounds(xx,406,(int)colW-24,62);oscLabels[(size_t)i].setBounds(xx,469,(int)colW-24,20);}
 for(int i=0;i<3;++i){const int xx=(int)(x+i*colW+12);advancedOscSliders[(size_t)i].setBounds(xx,529,(int)colW-24,65);advancedOscLabels[(size_t)i].setBounds(xx,595,(int)colW-24,21);}
 oscWave[0].setBounds((int)x+20,492,160,26);osc2Wave.setBounds((int)x+190,492,160,26);
 for(int i=0;i<8;++i){const float cw=mw/8.f;const int xx=(int)(x+i*cw+8);partialSliders[(size_t)i].setBounds(xx,659,(int)cw-16,(int)juce::jmax(48.f,lower-689.f));partialLabels[(size_t)i].setBounds(xx,(int)lower-23,(int)cw-16,18);}
 keyboard.setBounds(176,h-95,w-199,74);
}
BassforgeEditor::~BassforgeEditor(){stopTimer();setLookAndFeel(nullptr);}
juce::AudioProcessorEditor* BassforgeProcessor::createEditor(){return new BassforgeEditor(*this);}
