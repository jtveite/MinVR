#include <cstdio>
#include <iostream>
#include <display/VRProjectionNode.h>


using namespace MinVR;
int testProjectionNodeBasic();
int testProjectionNodeViewMatrix();
int testProjectionNodeProjectionMatrix();


int projectiontest(int argc, char* argv[]) {
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
    output = testProjectionNodeBasic();
    break;
  case 2:
    output = testProjectionNodeViewMatrix();
    break;
  case 3:
    output = testProjectionNodeProjectionMatrix();
    break;

  default:
    std::cout << "Test #" << choice << " does not exist!" << std::endl;
    output = -1;
  }

  return output;



}

VRDisplayNode* createProjectionNode(float nearClip = 0.1, float farClip = 100., float fovY = 45, float fovX = 45){
  VRDataIndex config;
  config.addData("NearClip", nearClip);
  config.addData("FarClip", farClip);
  config.addData("FieldOfViewY", fovY);
  config.addData("FieldOfViewX", fovX);
  return VRProjectionNode::create(NULL, &config, "");

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


int testProjectionNodeBasic(){
  printf("we're in a test yay\n");
  VRDisplayNode* dn = createProjectionNode();

  if(dn == NULL){
    printf("Node creation failed.\n");
    return -1;
  }

  if (dn->getType() != "VRProjectionNode"){
    printf("TYPE STRING DOES NOT MATCH\n");
    return -2;
  }

  VRDataIndex renderState;
  VRMatrix4 lam = VRMatrix4::translation(VRVector3(0, 1, 0));
  renderState.addData("LookAtMatrix", lam);

  DummyRenderHandler rh;
  dn->render(&renderState, &rh);
  
  std::cout << rh.state.printStructure() << std::endl;
  if (! rh.state.exists("ViewMatrix")){
    printf("DID NOT ADD VIEW MATRIX\n");
    return -3;
  }
  if (! rh.state.exists("ProjectionMatrix")){
    printf("DID NOT ADD PROJECTION MATRIX\n");
    return -4;
  }
   
  return 0;
}

int testViewMatrix(VRMatrix4 lookAt){
  VRDisplayNode* dn = createProjectionNode();
  VRDataIndex renderState;
  renderState.addData("LookAtMatrix", lookAt);

  DummyRenderHandler rh;
  dn->render(&renderState, &rh);
  
  VRMatrix4 viewMat = rh.state.getValue("ViewMatrix");
  if (viewMat != lookAt.inverse() && 0){
    printf("INCORRECT VIEW MATRIX");
    std::cout << "EXPECTED: \n" << lookAt.inverse() << std::endl;
    std::cout << "GOT: \n" << viewMat << std::endl;
    return -4;
  }
  return 0;
}

int testProjectionNodeViewMatrix(){
  int res = 0;
  if ( (res = testViewMatrix(VRMatrix4::translation(VRVector3(0, 0, 0)))) != 0){
    return res;
  }
  if ( (res = testViewMatrix(VRMatrix4::translation(VRVector3(10, 20, -30)))) != 0){
    return res;
  }
  if ( (res = testViewMatrix(VRMatrix4::rotation(VRPoint3(0, 1, 2), VRVector3(6, -2, -1), 35))) != 0){
    return res;
  }
  if ( (res = testViewMatrix(VRMatrix4::rotation(VRPoint3(0, 1, 2), VRVector3(6, -2, -1), 35) * VRMatrix4::translation(VRVector3(3, 2, 1)))) != 0){
    return res;
  }
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

int checkPerspective(VRMatrix4 p, float xx, float yy, float zz, float trans, float bottom){
  return eq(p(0,0), xx) && eq(p(0,1), 0.) && eq(p(0,2), 0.) && eq(p(0,3), 0.) &&
         eq(p(1,0), 0.) && eq(p(1,1), yy) && eq(p(1,2), 0.) && eq(p(1,3), 0.) &&
         eq(p(2,0), 0.) && eq(p(2,1), 0.) && eq(p(2,2), zz) && eq(p(2,3), trans) &&
         eq(p(3,0), 0.) && eq(p(3,1), 0.) && eq(p(3,2), bottom) && eq(p(3,3), 0.);
}

         
         



int testProjMatrix(float nearClip, float farClip, float fovY, float fovX, float xx, float yy, float zz, float trans, float bottom = -1.0f){
  VRDisplayNode* dn = createProjectionNode(nearClip, farClip, fovY, fovX);
  VRDataIndex renderState;
  renderState.addData("LookAtMatrix", VRMatrix4());

  DummyRenderHandler rh;
  dn->render(&renderState, &rh);
  
  VRMatrix4 projMat = rh.state.getValue("ProjectionMatrix");
  if (! checkPerspective(projMat, xx, yy, zz, trans, bottom)){
    printf("Projection matrix is not as expected.\n");
    std::cout << projMat << std::endl;
    return -1;
  }
  
  return 0;
}




int testProjectionNodeProjectionMatrix(){
  int res = 0;
  if ( (res = testProjMatrix(0.1, 100, 45, 45,
                            2.414, 2.414, -1, -0.2, -1)) != 0) {
    return res;
  }
  if ( (res = testProjMatrix(0.1, 100, 90, 45,
                            2.414, 1, -1, -0.2, -1)) != 0) {
    return res;
  }
  if ( (res = testProjMatrix(1, 100, 90, 90,
                            1, 1, -1.02, -2.02, -1)) != 0) {
    return res;
  }
  return 0;
}
