#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include "glm/glm.hpp"
#include "glm/gtx/projection.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "kiwi/core/all.hpp"

namespace renderer{
  class Shader;
  class FrameBuffer;
  
class Renderer 
{
public:

  Renderer(unsigned int x, unsigned int y){
    window.x = x;
    window.y = y;
    bufID[0] = 0;
    _frameBuffer = 0;
  }
  ~Renderer();

  void init();
  void drawScene();

  void setWindowDimensions (unsigned int x, unsigned int y) {
    window.x = x;
    window.y = y;
    freeBuffers();
    createBuffers();
  }

  void createBuffers();

private:
  FrameBuffer* _frameBuffer;
  
  struct {
    unsigned int x;
    unsigned int y;
  } window;

  glm::mat4 viewMatrix;

  kiwi::core::Node * projMatNode; 
  kiwi::core::Node * viewMatNode; 
  kiwi::core::Node * modelMatNode; 
  kiwi::core::Node * winSizeNode;
  kiwi::core::Node * TimeNode;

  kiwi::core::Pipeline * renderPipeline;

  Shader *raymarchingShader;
  Shader *postEffectShader;
  unsigned int vaoID[1];
  unsigned int tcoID[1];
  unsigned int vboID[1];
  unsigned int bufID[1];

  float fuffaTime;

  void freeBuffers();
  void createPlane();
};


} //  namespace

#endif // RENDERER_HPP_INCLUDED
