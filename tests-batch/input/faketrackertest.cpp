#include <cstdio>
#include <iostream>
#include <input/VRFakeTrackerDeviceRelative.h>
#include <main/VRMain.h>
#include <main/VREventInternal.h>


using namespace MinVR;
int testFakeTrackerNodeBasic();
int testFakeTrackerNodeCorrectValues();


int faketrackertest(int argc, char* argv[]) {
  int defaultchoice = 1;
  int choice = defaultchoice;

  if (argc > 1) {
    if (sscanf(argv[1], "%d", &choice) != 1){
      printf("Couldn't parse that input as a number.\n");
      return -1;
    }
  }

  int output;

  switch(choice) {
  case 1:
    output = testFakeTrackerNodeBasic();
    break;
  case 2:
    output = testFakeTrackerNodeCorrectValues();
    break;

  default:
    std::cout << "Test #" << choice << " does not exist!" << std::endl;
    output = -1;
  }

  return output;



}


class DummyMain : public VRMainInterface{
public:
  virtual void addInputDevice(VRInputDevice *dev){  }
  virtual void addEventHandler(VREventHandler *eHandler) {}
  virtual void addRenderHandler(VRRenderHandler*) {}
  virtual void addPluginSearchPath(const std::string& path) {}
  virtual VRDataIndex* getConfig() {return new VRDataIndex();}
  virtual VRGraphicsToolkit* getGraphicsToolkit(const std::string &name) {return NULL;}
  virtual VRWindowToolkit* getWindowToolkit(const std::string &name) {return NULL;}
  virtual VRFactory* getFactory() {return NULL;}

  
};


VRFakeTrackerDeviceRelative* createFakeTrackerDevice(VRDataIndex base){
  VRDataIndex config = base;
  config.addData("Tmp/TrackerName", "testTracker");
  config.addData("Tmp/ToggleOnOffEvent", "Start");
  config.addData("Tmp/RotateEvent", "Rot");
  config.addData("Tmp/RollEvent", "Roll");
  config.addData("Tmp/TranslateEvent", "Trans");
  config.addData("Tmp/TranslateZEvent", "TransZ");
  std::cout << config.printStructure() << std::endl;
  
  DummyMain dm;
  return (VRFakeTrackerDeviceRelative*) VRFakeTrackerDeviceRelative::create(&dm, &config, "Tmp");

}

class DummyRenderHandler : public VRRenderHandler{
public:
  virtual void onVRRenderScene(VRDataIndex *renderState, VRDisplayNode *callingNode){
    state = *renderState;
    node = callingNode;
  }
  virtual void onVRRenderContext(VRDataIndex *renderState, VRDisplayNode *callingNode){
  
    state = *renderState;
    node = callingNode;
  }

  VRDataIndex state;
  VRDisplayNode* node;
};


void SendMouseEvent(VRFakeTrackerDeviceRelative* dev, float x, float y){
  VRDataIndex di;
  VRFloatArray da;
  da.push_back(x);
  da.push_back(y);
  di.addData("Mouse_Move/NormalizedPosition", da);
  VREventInternal mouse ("Mouse_Move", &di);
  dev->onVREvent(*mouse.getAPIEvent());

}

void SendEvent(VRFakeTrackerDeviceRelative* dev, std::string name){
  printf("Sending event %s\n", name.c_str());
  VRDataIndex di;
  di.addData("/" + name + "/Val", 1);
  VREventInternal e (name, &di);
  dev->onVREvent(*e.getAPIEvent());
}



int testFakeTrackerNodeBasic(){
  printf("we're in a test yay\n");
  VRDataIndex di;
  VRFakeTrackerDeviceRelative* dn = createFakeTrackerDevice(di);

  if(dn == NULL){
    printf("Node creation failed.\n");
    return -1;
  }
  else{
    printf("Node creation succes.\n");
  }
  SendEvent(dn, "Start");
  VRDataQueue queue;
  dn->appendNewInputEventsSinceLastCall(&queue);
  if (! queue.notEmpty()){
    printf("FAILED TO SUBMIT STARTING EVENT\n");
    return -2;
  }
  queue.pop();
  if (queue.notEmpty()){
    printf("SUBMITTED MULTIPLE STARTING EVENTS\n");
    return -3;
  }
  SendEvent(dn, "Trans_Down");
  SendMouseEvent(dn, 0., 0.);
  dn->appendNewInputEventsSinceLastCall(&queue);
  if (! queue.notEmpty()){
    printf("FAILED TO GENERATE EVENT ON TRANSLATION\n");
    return -4;
  }
  queue.pop();
  if (queue.notEmpty()){
    printf("GENERATED TOO MANY EVENTS!!!!\n");
    return -5;
  }
  SendEvent(dn, "Start"); //Actually toggle
  SendMouseEvent(dn, 0.5, 0.5);
  dn->appendNewInputEventsSinceLastCall(&queue);
  if (queue.notEmpty()){
    printf("Generated events when it should be off.\n");
    return -6;
  }

  return 0;
}

//TODO
int testFakeTrackerNodeCorrectValues(){
  int res = 0;
  return res;
}

bool eq(float a, float b, float delta = 0.01){
  if ( abs(a - b) < delta ){
    return true;
  }
  else{
    printf("ERROR: FALSE: a: %f, b: %f, a-b: %f, delta: %f\n", a, b, abs(a-b), delta);
    return false;
  }
}


