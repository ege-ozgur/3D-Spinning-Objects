#include "Window.h"
#include "Core.h"
#include "PSOManager.h"
#include "ShaderManager.h"
#include "maths.h"
#include "Vertex.h"
#include "Camera.h"
#include "Plane.h"
#include "Cube.h"
#include "Mesh.h"
#include "Sphere.h"
#include "GEMLoader.h"
#include "StaticMesh.h"
#include "AnimatedMesh.h"
#include "Animation.h"
#include <chrono>
#include <vector>
#include <cmath>

using namespace std;

extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

const string SHADER_PATH_VS = "vertexShader.hlsl";
const string SHADER_PATH_PS = "pixelShader.hlsl";

class Timer {
public:
    std::chrono::steady_clock::time_point last;
    Timer() { last = std::chrono::steady_clock::now(); }
    float dt() {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> diff = now - last;
        last = now;
        return diff.count();
    }
};

// 3D Spinning Cube
//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
//{
//
//	Window win;
//
//	Core core;
//
//	GamesEngineeringBase::Timer tim;
//
//	Cube cubeDraw;
//
//	win.initialize(1024, 1024, "3D Cube");
//
//	core.initialize(win.hwnd, 1024, 1024);
//
//	cubeDraw.init(&core);
//
//	Matrix matrixWorld;
//
//	float t = 0;
//
//	while (true)
//	{
//		core.beginFrame();        
//
//		win.processMessages();
//
//		if (win.keys[VK_ESCAPE] == 1)   
//		{
//			break;
//		}
//
//		core.beginRenderPass();
//
//		t += tim.dt();
//
//		float aspect = (float)win.width / (float)win.height;
//		float fieldOfView = 60.0f;
//		float _near = 0.01f;
//		float _far = 10000.0f;
//
//		Matrix p = p.perspectiveProjection(aspect, fieldOfView, _near, _far);
//
//		Vec3 from = Vec3(11 * cos(t), 5, 11 * sinf(t));
//
//		Matrix v = v.lookAtMatrix(from, Vec3(0, 0, 0), Vec3(0, 1, 0));
//
//		Matrix vp = p.multiply(v);
//
//		cubeDraw.draw(&core, matrixWorld, vp);
//
//		core.finishFrame();
//
//	}
//	core.flushGraphicsQueue();
//
//	return 0;
//}
//

// 3D Spinning Sphere

//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
//{
//    Window win;
//    Core core;
//    GamesEngineeringBase::Timer tim;
//
//    Sphere sphereDraw;
//
//    win.initialize(1024, 1024, "3D Sphere");
//    core.initialize(win.hwnd, 1024, 1024);
//
//    sphereDraw.init(&core, 20, 20, 1.5f);
//
//    Matrix sphereWorld;
//
//    float t = 0;
//    float radius = 8.0f; 
//
//    while (true)
//    {
//        core.beginFrame();
//        win.processMessages();
//        if (win.keys[VK_ESCAPE] == 1) break;
//
//        core.beginRenderPass();
//
//        t += tim.dt();
//
//        float aspect = (float)win.width / (float)win.height;
//        float fieldOfView = 60.0f;
//        float _near = 0.01f;
//        float _far = 10000.0f;
//
//        Matrix p;
//        p = p.perspectiveProjection(aspect, fieldOfView, _near, _far);
//
//        Vec3 from = Vec3(radius * cos(t), 4.0f, radius * sinf(t));
//        Matrix v;
//        v = v.lookAtMatrix(from, Vec3(0, 0, 0), Vec3(0, 1, 0));
//
//        Matrix vp = p.multiply(v);
//
//        sphereDraw.draw(&core, sphereWorld, vp);
//
//        core.finishFrame();
//    }
//
//    core.flushGraphicsQueue();
//    return 0;
//}

// Static Cubes - Orbiting Camera
//
//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
//{
//    Window win;
//    Core core;
//    GamesEngineeringBase::Timer tim;
//
//    Cube cubeDraw;
//
//    win.initialize(1024, 1024, "Static Cubes - Orbiting Camera");
//    core.initialize(win.hwnd, 1024, 1024);
//    cubeDraw.init(&core);
//
//    float t = 0;
//    float cameraRadius = 11.0f;
//
//    while (true)
//    {
//        core.beginFrame();
//        win.processMessages();
//        if (win.keys[VK_ESCAPE] == 1) break;
//
//        core.beginRenderPass();
//        t += tim.dt();
//
//        float aspect = (float)win.width / (float)win.height;
//        Matrix p;
//        p = p.perspectiveProjection(aspect, 60.0f, 0.1f, 1000.0f);
//
//        Vec3 target = Vec3(0, 0, 0);
//
//        Vec3 from = Vec3(cameraRadius * cos(-t * 0.5f), 4.0f, cameraRadius * sinf(-t * 0.5f));
//
//        Matrix v;
//        v = v.lookAtMatrix(from, target, Vec3(0, 1, 0));
//        Matrix vp = p.multiply(v);
//
//        Matrix world1;
//
//        cubeDraw.draw(&core, world1, vp);
//
//        Matrix world2;
//        world2.translation(Vec3(5.0f, 0.0f, 0.0f)); 
//
//        cubeDraw.draw(&core, world2, vp);
//        core.finishFrame();
//    }
//
//    core.flushGraphicsQueue();
//    return 0;
//}

// Acacia + Dinosaur Scene

struct CameraController {
    Vec3 position;
    Vec3 forward;
    Vec3 right;
    Vec3 up;

    float yaw;
    float pitch;

    float moveSpeed = 10.0f;
    float mouseSensitivity = 0.002f;

    void init(Vec3 startPos) {
        position = startPos;
        up = Vec3(0, 1, 0);
        yaw = 0.0f;
        pitch = 0.0f;
        updateVectors();
    }

    void updateVectors() {
        forward.x = sin(yaw) * cos(pitch);
        forward.y = sin(pitch);
        forward.z = cos(yaw) * cos(pitch);

        float len = sqrt(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z);
        if (len > 0) { forward.x /= len; forward.y /= len; forward.z /= len; }

        right = forward.Cross(up);

        len = sqrt(right.x * right.x + right.y * right.y + right.z * right.z);
        if (len > 0) { right.x /= len; right.y /= len; right.z /= len; }
    }

    void update(float dt, Window* win) {

        int centerX = win->width / 2;
        int centerY = win->height / 2;
        float deltaX = (float)(win->mousex - centerX);
        float deltaY = (float)(win->mousey - centerY);

        if (deltaX != 0 || deltaY != 0) {
            yaw += deltaX * mouseSensitivity;
            pitch -= deltaY * mouseSensitivity; 

            if (pitch > 1.5f) pitch = 1.5f;
            if (pitch < -1.5f) pitch = -1.5f;

            updateVectors();
            POINT pt;
            pt.x = centerX;
            pt.y = centerY;
            ClientToScreen(win->hwnd, &pt); 
            SetCursorPos(pt.x, pt.y);
            win->updateMouse(centerX, centerY);
        }

        float speed = moveSpeed * dt;

        if (win->keys['W']) position = position + (forward * speed);
        if (win->keys['S']) position = position - (forward * speed);
        if (win->keys['D']) position = position - (right * speed);
        if (win->keys['A']) position = position + (right * speed);

    }

    Matrix getViewMatrix() {
        Matrix v;
        return v.lookAtMatrix(position, position + forward, up);
    }
};

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    Window win;
    Core core;
    Timer tim;
    CameraController cam;

    ShaderManager shaderMgr;
    PSOManager psoMgr;

    StaticMesh acaciaDraw;
	StaticMesh ashTreeDraw;
    Plane planeDraw;
    AnimatedMesh dinoDraw;
    AnimationInstance dinoAnim;

    win.initialize(1024, 1024, "Game Scene");
    core.initialize(win.hwnd, 1024, 1024);

    planeDraw.init(&core);
    acaciaDraw.init(&core, "Models/acacia_003.gem");
	ashTreeDraw.init(&core, "Models/Ash_Tree_01e.gem");
    dinoDraw.load(&core, "Models/TRex.gem", &psoMgr, &shaderMgr);

    dinoAnim.init(&dinoDraw.animation, 0);
    dinoAnim.usingAnimation = "run";
    dinoAnim.t = 0;

    cam.init(Vec3(0, 5, -20));
    ShowCursor(FALSE);

    Matrix worldAcacia1;
    worldAcacia1.scaling(Vec3(0.01f, 0.01f, 0.01f));
    worldAcacia1.translation(Vec3(5.0f, 0.0f, 0.0f));

    Matrix worldAcacia2;
    worldAcacia2.scaling(Vec3(0.01f, 0.01f, 0.01f));
    worldAcacia2.translation(Vec3(10.0f, 0.0f, 0.0f));

    Matrix worldAshTree;
    worldAshTree.scaling(Vec3(1.0f, 1.0f, 1.0f));
    worldAshTree.translation(Vec3(15.0f, 0.0f, 0.0f));

    Matrix worldPlane;
    worldPlane.scaling(Vec3(1.0f, 1.0f, 1.0f));
    worldPlane.translation(Vec3(0.0f, -0.1f, 0.0f));

    Matrix worldDino;
    worldDino.scaling(Vec3(0.01f, 0.01f, 0.01f));
    worldDino.translation(Vec3(0.0f, 0.0f, 0.0f));

    while (true)
    {
        core.beginFrame();
        win.processMessages();

        if (win.keys[VK_ESCAPE]) {
            break;
        }

        core.beginRenderPass();

        float dt = tim.dt();

        cam.update(dt, &win);
        dinoAnim.update("run", dt);

        float aspect = (float)win.width / (float)win.height;
        Matrix p;
        p = p.perspectiveProjection(aspect, 60, 0.1f, 5000);

        Matrix v = cam.getViewMatrix();
        Matrix vp = v * p;

        acaciaDraw.draw(&core, worldAcacia1, vp);
        acaciaDraw.draw(&core, worldAcacia2, vp);
		ashTreeDraw.draw(&core, worldAshTree, vp);
        planeDraw.draw(&core, worldPlane, vp);


        dinoDraw.draw(&core, &psoMgr, &shaderMgr, &dinoAnim, vp, worldDino);

        core.finishFrame();
    }

    ShowCursor(TRUE);
    core.flushGraphicsQueue();
    return 0;
}