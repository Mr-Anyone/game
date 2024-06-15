#include <stdio.h>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "physics.h"
#include "ds.h"
#include "ecs.h"
#include "quadtree.h"
#include "shader.h"
#include "sphere.h"
#include "cube.h"

constexpr int width = 1920;
constexpr int height = 1080;

glm::mat4 projection = glm::perspective(
    glm::radians(45.0f), (float)width / (float)height, 0.1f, 10000.0f);

glm::vec3 camPos = glm::vec3(0, 0, 3);
glm::vec3 up = glm::vec3(0.0, 1.0f, 0.0);
glm::vec3 front = glm::vec3(0.0, 0.0, -1.0);
glm::mat4 view = glm::lookAt(camPos, camPos + front, up);

glm::vec3 lightPos(0, 0, 0);
glm::vec3 lightColor(0.5, 0.5, 0.5);

float yaw = -90.0f;
float pitch = 0;

class Cube {
public:
  Cube()
      : m_shader{"./../shader/sphere_vertex.glsl",
                 "./../shader/sphere_fragment_shader.glsl"} {

    const float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f,
        0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f,
        0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,
        0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f};

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0],
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3,
                          (void *)0);
    glEnableVertexAttribArray(0);
  }

  void renderCube(const RigidBody &body) {
    glm::mat4 model = make4xRotationMatrx(body.orientation);
    model = glm::scale(glm::mat4(1.0f), glm::vec3(5.0, 5.0, 5.0)) * model;
    model = glm::translate(glm::mat4(1.0), body.pos) * model;
    // printMat(model, 4);
    //     printMat(body.orientation, 3);

    m_shader.use();
    m_shader.setMat4("model", model);
    m_shader.setMat4("view", view);
    m_shader.setMat4("projection", projection);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

private:
  glm::mat4 make4xRotationMatrx(const glm::mat4 &rotation_matrix) {
    return glm::mat4{
        {rotation_matrix[0][0], rotation_matrix[0][1], rotation_matrix[0][2],
         0},
        {rotation_matrix[1][0], rotation_matrix[1][1], rotation_matrix[1][2],
         0},
        {rotation_matrix[2][0], rotation_matrix[2][1], rotation_matrix[2][2],
         0},
        {0, 0, 0, 1},
    };
  }
  unsigned int m_vao, m_vbo;
  Shader m_shader;
};

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  const float cameraSpeed = 5.0f; // adjust accordingly
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camPos += cameraSpeed * front;

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camPos -= cameraSpeed * front;

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camPos -= glm::normalize(glm::cross(front, up)) * cameraSpeed;

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camPos += glm::normalize(glm::cross(front, up)) * cameraSpeed;

  static int mode = GLFW_CURSOR_DISABLED;
  static float last_update_time = glfwGetTime();
  if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS &&
      glfwGetTime() - last_update_time > 0.1f) {
    if (mode == GLFW_CURSOR_NORMAL)
      mode = GLFW_CURSOR_DISABLED;

    else if (mode == GLFW_CURSOR_DISABLED)
      mode = GLFW_CURSOR_NORMAL;

    glfwSetInputMode(window, GLFW_CURSOR, mode);
    last_update_time = glfwGetTime();
  }

  static int last_cursor_mode = GL_FILL;
  if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS &&
      glfwGetTime() - last_update_time > 1) {
    if (last_cursor_mode == GL_LINE) {
      last_cursor_mode = GL_FILL;
    } else {
      last_cursor_mode = GL_LINE;
    }

    last_update_time = glfwGetTime();
  }

  glPolygonMode(GL_FRONT_AND_BACK, last_cursor_mode);

  view = glm::lookAt(camPos, camPos + front, up);
}

float lastX;
float lastY;
bool firstMouse = true;
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) {
    return;
  }

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(direction);
}

void lightingImGUI(Shader &shader) {
  ImGui::Text("Lighting");

  ImGui::Text("Object Color");
  static float objc[3] = {0.5, 0.5, 0.5};
  static float lightc[3] = {0.52, 0.52, 0.52};
  static float pos[3] = {0, 1000.0f, 0};

  ImGui::DragFloat3("Object Color", objc, 0.01f, 0.0, 1.0);
  ImGui::DragFloat3("Light Color", lightc, 0.01f, 0.0, 1.0);
  ImGui::DragFloat3("Light Pos", pos, 10, -2000, 2000);

  lightPos = glm::vec3(pos[0], pos[1], pos[2]);
  lightColor = glm::vec3(lightc[0], lightc[1], lightc[2]);

  shader.use();
  shader.setVec3("objectColor", objc[0], objc[1], objc[2]);
  shader.setVec3("lightColor", lightc[0], lightc[1], lightc[2]);
  shader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
}

void imgui_write_vec(const char *vector, const glm::vec3 &vec) {
  ImGui::Text("%s %5.3f %5.3f %5.3f", vector, vec.x, vec.y, vec.z);
}

void render_imgui(DiamondSquareMesh &mesh, Shader &shader, RigidBody &body) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Terrain Window");
  ImGui::Text("Terrain Generation");

  static float height_one, height_two, height_three, height_four;
  constexpr float minscale = 1000.0f;
  ImGui::SliderFloat("height one", &height_one, -minscale, minscale);
  ImGui::SliderFloat("height two", &height_two, -minscale, minscale);
  ImGui::SliderFloat("height three", &height_three, -minscale, minscale);
  ImGui::SliderFloat("height foure", &height_four, -minscale, minscale);

  static int size = 7;
  ImGui::SliderInt("Size", &size, 7, 11);

  static float range = 1.0f;
  ImGui::SliderFloat("range", &range, 0.0f, 3000.0f);
  mesh.setRange(range);

  if (ImGui::Button("Generate"))
    mesh.generateMesh(size, height_one, height_two, height_three, height_four);

  lightingImGUI(shader);

  ImGui::Text("Looking At %5.1f, %5.1f, %5.1f", front.x, front.y, front.z);
  ImGui::Text("Camera Pos x: %5.1f, %5.1f, %5.1f", camPos.x, camPos.y,
              camPos.z);
  ImGui::SetWindowFontScale(1.5);

  // displaying rigid body stuff
  ImGui::Text("Rigid Body Stuff");
  imgui_write_vec("force", body.net_force);
  imgui_write_vec("torque", body.net_torque);
  imgui_write_vec("angular velocity",
                  body.inverse_inertia_tensor * body.angular_momentum);
  imgui_write_vec("position", body.pos);

  ImGui::End();
}

void apply_user_force(GLFWwindow *window, ComponentArray<RigidBody> &bodies) {
  float force_magnitude = 1.0f; // 10 N
  glm::vec3 push_location =
      glm::vec3(1.0f, 1.0f,
                1.0f); // a little bit above the box to create a bit of torque!

  glm::vec3 applied_force = force_magnitude * glm::normalize(push_location);
  bodies.getComponent(0).net_force += applied_force;
  bodies.getComponent(0).net_torque +=
      glm::cross(push_location, glm::vec3(0.0, 0.0, force_magnitude));
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Basic OpenGL initialization

  GLFWwindow *window =
      glfwCreateWindow(width, height, "Some OpenGL Window", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, width, height);

  Shader shader("./../shader/vertex_shader.glsl",
                "./../shader/fragment_shader.glsl");
  shader.use();
  shader.setMat4("projection", projection);

  LightingSphere sphere(10, 100);
  DiamondSquareMesh mesh(7);

  glfwSetCursorPosCallback(window, mouse_callback);

  glEnable(GL_DEPTH_TEST);

  // setting up imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(
      window, true); // Second param install_callback=true will install GLFW
                     // callbacks and chain to existing ones.
  ImGui_ImplOpenGL3_Init();

  shader.use();

  // setup ecs and other stuff
  ComponentArray<RigidBody> rigid_bodies;
  rigid_bodies.appendComponent(0, makeBoxRigidBody(1, 0.5));
  Cube some_cube;

  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_FRONT);

  std::vector<TriangleMesh> forQuadTree = mesh.makeTriangleMesh();
  QuadTree<TriangleMesh> tree{Square(glm::vec2(-10, -10), 3000)};
  for (auto &mesh : forQuadTree) {
    tree.insert(glm::vec2(mesh.center.x, mesh.center.z), mesh);
  }
  std::cout << "I've made a tree?" << std::endl;

  std::vector<TriangleMesh> test;
  tree.query(glm::vec2(0, 0), test);
   for (int i = 0; i < test.size(); ++i) {
     std::cout << test[i].center.x << ",";
     std::cout << test[i].center.y << ",";
     std::cout << test[i].center.z << std::endl;
   }

   std::cout << forQuadTree.size() << std::endl;
   std::cout << test.size() << std::endl;

   exit(-1);

  float last_time = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // apply force
    apply_user_force(window, rigid_bodies);

    // rendering imgui stuff
    render_imgui(mesh, shader, rigid_bodies.getComponent(0));

    integrate(rigid_bodies, glfwGetTime() - last_time);

    // rendering my stuff
    shader.use();
    shader.setMat4("view", view);
    mesh.renderMesh();
    rigid_bodies.getComponent(0).pos = lightPos;
    // sphere.render(projection, view, lightPos, lightColor);
    some_cube.renderCube(rigid_bodies.getComponent(0));

    processInput(window);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
    last_time = glfwGetTime();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
  return 0;
}
