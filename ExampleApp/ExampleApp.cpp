#include <iostream>

#include <GLFW/glfw3.h>

#include "ECS.hpp"

#include "Components.h"
#include "Systems.h"

int width = 1280;
int height = 720;

// Helper methods.
void FillQuad(float x, float y, float width, float height)
{
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
}

void SetColor(float r, float g, float b)
{
    glColor3f(r, g, b);
}

// Sorry!
ECS ecs;
int entityToAddGravityTo = 0;

constexpr int numEntities = 20;
std::array<Entity, numEntities> entities;

int main()
{
    // Initial Rendering setup ////////////////////////////////////////////////////////////////////////////////////
    GLFWwindow* window;

    // Initialize the glfw library.
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context.
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(width, height, "ECS Example, Press space to add gravity to them one by one", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make windoe's context current.
    glfwMakeContextCurrent(window);

    // Basic OpenGL Setup
    glViewport(0, 0, width, height);
    glOrtho(0, width, height, 0, -1, 1);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Initialize ECS.
    ecs.Init();

    // Register components.
    ecs.RegisterComponent<RigidBody>();
    ecs.RegisterComponent<Size>();
    ecs.RegisterComponent<Gravity>();

    // Register systems.
    auto rigidBodySystem = ecs.RegisterSystem<RigidBodySystem>();
    auto gravitySystem = ecs.RegisterSystem<GravitySystem>();
    auto renderSystem = ecs.RegisterSystem<RenderSystem>();

    // Set signature for the systems.
    Signature signature;
    signature.set(ecs.GetComponentType<RigidBody>(), true);
    signature.set(ecs.GetComponentType<Size>(), true);

    ecs.SetSystemSignature<RigidBodySystem>(signature);
    ecs.SetSystemSignature<RenderSystem>(signature);

    signature.set(ecs.GetComponentType<Size>(), false);
    signature.set(ecs.GetComponentType<Gravity>(), true);

    ecs.SetSystemSignature<GravitySystem>(signature);

    for (int i = 0; i < numEntities; i++)
    {
        entities[i] = ecs.CreateEntity();
        ecs.AddComponent(entities[i], RigidBody(i * 60 + 20, 50, 0, 0));
        ecs.AddComponent(entities[i], Size(50, 50));
    }

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (key == GLFW_KEY_SPACE)
            {
                switch (action)
                {
                case GLFW_PRESS:
                {
                    if (entityToAddGravityTo == numEntities)
                        return;
                    ecs.AddComponent(entities[entityToAddGravityTo++], Gravity(1));
                    break;
                }
                default:
                    break;
                }
            }
        });

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(92.0f / 255, 154.0f / 255, 255.0f / 255, 1);
        glClear(GL_COLOR_BUFFER_BIT);
            
        // Main Rendering and logic here. /////////////////////////////////////////

        rigidBodySystem->Update(ecs);
        gravitySystem->Update(ecs);
        renderSystem->Update(ecs);

        ///////////////////////////////////////////////////////////////////////////

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
