#include <glew.h>
#include "Camera.h"
Camera::Camera() :skybox(nullptr)
{
    skybox = new Skybox("./textures/Underwater/Underwater Box_Posx.png",
        "./textures/Underwater/Underwater Box_Posy.png",
        "./textures/Underwater/Underwater Box_Posz.png",
        "./textures/Underwater/Underwater Box_Negx.png",
        "./textures/Underwater/Underwater Box_Negy.png",
        "./textures/Underwater/Underwater Box_Negz.png");
    skybox->OnCreate();

    shader = new Shader("shaders/skyboxVert.glsl", "shaders/skyboxFrag.glsl");
    if (shader->OnCreate() == false) {
        std::cout << "Shader failed ... we have a problem\n";
    }
    trackball = new Trackball();
}
Camera::~Camera()
{

}
void Camera::Perspective(float fovy, float aspectRatio, float near, float far)
{
    glGetIntegerv(GL_VIEWPORT, viewport); // (0,0,w,h)                      
    aspectRatio = static_cast<float>(viewport[2]) / static_cast<float>(viewport[3]);
    projection = MMath::perspective(fovy, aspectRatio, near, far);
}

void Camera::LookAt(const Vec3& eye, const Vec3& at, const Vec3& up)
{
    view = MMath::lookAt(eye, at, up);
}

void Camera::Render()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader->GetProgram());

    glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projection);
  
    glUniformMatrix4fv(shader->GetUniformID("cameraRotationMatrix"), 1, GL_FALSE, trackball->getMatrix4());
    skybox->Render();
    glUseProgram(0);
}

void Camera::HandleEvents(const SDL_Event& sdlEvent) {
    trackball->HandleEvents(sdlEvent);
}
