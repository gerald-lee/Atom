/*
* @file		Application.cpp
* @author	Team Atom
* @brief	main bootstrapper
* @date		2021-01-12
*/

#include <Pch.hpp>
#include <cstdlib>
#include <clocale>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "src/core/Renderer.hpp"
//#define GLEW_STATIC

#include "core/Types.hpp"
#include "utils/Log.hpp"
#include "components/RenderBoxComponent.hpp"

//Event
#include "core/Event.hpp"
#include "core/EventManager.hpp"

//Input
#include "core/InputManager.hpp"
#include "components/Controller.hpp"

//Global Variable
EventManager gpEventManager;
InputCallBack gpInputManager;

#ifdef _WIN64
#include "Windows.h"
FILE _iob[] = {
    *stdin,
    *stdout,
    *stderr
};
extern "C" FILE * __cdecl __iob_func(void) {
    return _iob;
}
#pragma comment(lib, "legacy_stdio_definitions.lib")

//allocating console
void console() {
#ifdef _WIN64
    if (AllocConsole())
    {
        FILE* file;

        freopen_s(&file, "CONOUT$", "wt", stdout);
        freopen_s(&file, "CONOUT$", "wt", stderr);
        freopen_s(&file, "CONOUT$", "wt", stdin);

        SetConsoleTitle(L"[Lime]");
    }
#endif
}

// max 80 to prevent using new and having memory leaks 
void setConsoleTitle(const char* title) {
    std::setlocale(LC_ALL, "en_US.utf8");
    std::wcout.imbue(std::locale("en_US.utf8"));
    wchar_t wstr[80];
    // +1 to account for \0 termination
    size_t wsize;
    mbstowcs_s(&wsize, wstr, strlen(title) + 1, title, strlen(title));
    SetConsoleTitle(wstr);
}
#endif

float random() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);//uniform distribution between 0 and 1
    return (float)dis(gen);
}

//Controller Portion

void onEvent01(Event& e) {
    if (e.getType() == EventID::E_WINDOW_KEY_TRIGGERED) {
        ATOM_INFO("KEYCODE : {}", e.getParam<int>(EventID::P_WINDOW_KEY_TRIGGERED_KEYCODE));
    }
}

void onEvent02(Event& e) {
    if (e.getType() == EventID::E_WINDOW_KEY_PRESSED) {
        ATOM_TRACE("KEYCODE : {}", e.getParam<int>(EventID::P_WINDOW_KEY_PRESSED_KEYCODE));
    }
}

void onEvent03(Event& e) {
    if (e.getType() == EventID::E_WINDOW_KEY_RELEASED) {
        ATOM_WARN("KEYCODE : {}", e.getParam<int>(EventID::P_WINDOW_KEY_RELEASED_KEYCODE));
    }
}

void controllerReact()
{
    //React to listner with Key and Event ID
    //Triggered
    gpEventManager.addListener(EventID::E_WINDOW_KEY_TRIGGERED, onEvent01);
        /*[](Event &e) {
        std::cout << "The following key is Triggered : {}", e.getParam<int>(EventID::P_WINDOW_KEY_TRIGGERED_KEYCODE);
        });*/

    //Triggered
    gpEventManager.addListener(EventID::E_WINDOW_KEY_PRESSED, onEvent02);
        /*[](Event &e) {
        std::cout << "The following key is Triggered : {}", e.getParam<int>(EventID::P_WINDOW_KEY_PRESSED_KEYCODE);
        });*/

    //Triggered
    gpEventManager.addListener(EventID::E_WINDOW_KEY_RELEASED, onEvent03);
        /*[](Event &e) {
        std::cout << "The following key is Triggered : {}", e.getParam<int>(EventID::P_WINDOW_KEY_RELEASED_KEYCODE);
        });*/
}

void serdeDemo() {

    // simple serde demo
    ATOM_INFO("Simple Serde Demo...");
    // saving to file
    RenderBoxComponent rbc01{ glm::vec3{0.0f,1.0f,1.0f} };
    ordered_json j01;
    to_json(j01, rbc01);
    save("out01.json", j01);

    // loading from file
    RenderBoxComponent rbc02;
    ordered_json j02;
    load("out01.json", j02);
    from_json(j02, rbc02);

    // log both values
    ATOM_TRACE("RenderBoxComponent 1...");
    std::cout << std::setw(4) << j01 << std::endl;
    ATOM_TRACE("RenderBoxComponent 2...");
    std::cout << std::setw(4) << j02 << std::endl;

    // advanced serde demo
    ATOM_INFO("Advanced Serde Demo...");
    // saving
    ordered_json j03 = json::array();
    for (int i = 0; i < 10;++i) {
        ordered_json j04;
        RenderBoxComponent rbc03{ glm::vec3{random(),random(),random()} };
        to_json(j04, rbc03);
        j03.push_back(j04);
    }
    save("out02.json", j03);

    //loading
    unsigned int count = 0;
    ordered_json j05 = json::array();
    std::vector<RenderBoxComponent> rbc04;
    load("out02.json", j05);
    for (auto& j06 : j05) {
        RenderBoxComponent rbc05;
        from_json(j06, rbc05);
        ATOM_TRACE("RenderBoxComponent {}", count++);
        std::cout << std::setw(4) << j06 << std::endl;
        rbc04.push_back(rbc05);
    }
    ATOM_TRACE("Vector size : {}", rbc04.size());
    ATOM_INFO("Done...")
}

int main(void)
{
    // allocating and setting up console
    // initializing logging 
    console();
    setConsoleTitle(APPNAME);
    Log::init();

    controllerReact();
    serdeDemo();

    GLFWwindow* window;


    /* Initialize the library */
    if (!glfwInit()) return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, APPNAME, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    gpInputManager.init(window);


    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW init error" << std::endl;
        return false;
    }
    
    Renderer renderer;

    //glm::vec2 pos(0.0, 0.0);
    glm::vec2 pos(0.0, 0.0);
    glm::vec2 scale(1.0, 1.0);
    glm::vec3 color(0.2, 0.4, 0.8);

    //renderer.EnableFrameWireMode();
    // Test glm has been setup properly
    glm::vec3(1.0f);

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        renderer.DrawRec(pos, scale, color);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}