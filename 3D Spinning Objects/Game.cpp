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

// Acacia

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    Window win;
    Core core;
    GamesEngineeringBase::Timer tim;

    StaticMesh acaciaDraw;

    win.initialize(1024, 1024, "Acacia Model Viewer");
    core.initialize(win.hwnd, 1024, 1024);

    acaciaDraw.init(&core, "Models/acacia_003.gem");

    Matrix world;
    world.scaling(Vec3(0.01f, 0.01f, 0.01f));

    float t = 0;
    float radius = 20.0f; 

    while (true)
    {
        core.beginFrame();
        win.processMessages();
        if (win.keys[VK_ESCAPE] == 1) break;

        core.beginRenderPass();
        t += tim.dt();

        float aspect = (float)win.width / (float)win.height;
        float fieldOfView = 60.0f;
        float _near = 0.1f;  
        float _far = 10000.0f;

        Matrix p;
        p = p.perspectiveProjection(aspect, fieldOfView, _near, _far);

        Vec3 from = Vec3(radius * cos(t * 0.5f), 10.0f, radius * sinf(t * 0.5f));
        Matrix v;
        v = v.lookAtMatrix(from, Vec3(0, 0, 0), Vec3(0, 1, 0));

        Matrix vp = p.multiply(v);

        acaciaDraw.draw(&core, world, vp);

        core.finishFrame();
    }
    core.flushGraphicsQueue();
    return 0;
}

