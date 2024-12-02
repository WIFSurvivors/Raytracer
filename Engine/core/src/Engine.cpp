#include "includes/Engine.hpp"
#include "glm/ext/vector_float3.hpp"
#include "includes/utility/SimpleLogger.hpp"
#include <vector>

class TcpServer;
Engine::Engine() { init(); }

void Engine::init() {
  SimpleLogger::print("Engine::init()");
  try {
    _tcp_server->start();
  } catch (const std::exception &e) {
    SimpleLogger::print("Error: " + std::string(e.what()));
  }
  _render_system.init();
}

void Engine::startLoop() {
    std::vector<glm::vec3> v1 = {
         glm::vec3{-1.0f, -1.0f, 0.0f}, glm::vec3{1.0f, -1.0f, 0.0f},
         glm::vec3{1.0f, 1.0f, 0.0f},   glm::vec3{-1.0f, -1.0f, 0.0f},
         glm::vec3{1.0f, 1.0f, 0.0f},   glm::vec3{-1.0f, 1.0f, 0.0f}
    };
    std::vector<glm::vec3> v2 = {
             glm::vec3{-1.0f, -1.0f, 0.0f},
             glm::vec3{1.0f, -1.0f, 0.0f},
             glm::vec3{1.0f, 1.0f, 0.0f}
        };
    std::vector<glm::vec3> v3 = {
        glm::vec3{-1.0f, -1.0f, 0.0f},
        glm::vec3{1.0f, 1.0f, 0.0f},
        glm::vec3{-1.0f, 1.0f, 0.0f}
    };
    std::vector<glm::vec2> u1 = {glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 0.0f},
                                  glm::vec2{1.0f, 1.0f}, glm::vec2{0.0f, 0.0f},
                                  glm::vec2{1.0f, 1.0f}, glm::vec2{0.0f, 1.0f}};
    std::vector<glm::vec2> u2 = {
        glm::vec2{0.0f, 0.0f},
        glm::vec2{1.0f, 0.0f},
        glm::vec2{1.0f, 1.0f}
    };
    std::vector<glm::vec2> u3 = {
        glm::vec2{0.0f, 0.0f},
        glm::vec2{1.0f, 1.0f},
        glm::vec2{0.0f, 1.0f}
    };
  // RenderComponent comp1{
  //     get_active_uuid_manager()->getNewUUID(),
  //     _scene.get_root().lock().get(),
  //     _render_system.program->programID,
  //     v1,
  //     u1
  // };
  // RenderComponent comp2{
  //     get_active_uuid_manager()->getNewUUID(),
  //     _scene.get_root().lock().get(),
  //     _render_system.program->programID,
  //     v1,
  //     u1
  // };

 _render_system.create_component(
     get_active_uuid_manager()->getNewUUID(),
     _scene.get_root().lock().get(),
     v2,
    u2
 );
 _render_system.create_component(
     get_active_uuid_manager()->getNewUUID(),
     _scene.get_root().lock().get(),
     v3,
    u3
 );
  // comp1.init(_render_system.program->programID);
  // _render_system._component = std::make_unique<RenderComponent>(comp1);
  // _render_system.render();
  while(_wm.shouldClose()) {
          // _wm->processInput(_wm->_window);
          _render_system.update(_wm.get_time());
          // std::cout << "IM IN WHILE LOOP\n";
          _wm.update();
        // glfwSwapBuffers(_wm._window);
      }
	// glfwPollEvents();
  // comp.destroy();
  _render_system.destroy();
}

UUIDManager *Engine::get_active_uuid_manager() {
  return _scene.get_uuid_manager();
}
