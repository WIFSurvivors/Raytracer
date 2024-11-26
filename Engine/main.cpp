#include "includes/TcpServer.hpp"
#include "includes/Entity.hpp"
#include "includes/system/RenderSystem.hpp"
#include "includes/component/RenderComponent.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/SimpleLogger.hpp"

int main() {
  SimpleLogger::print("=== APP STARTED ===");
  Engine engine{};
  SimpleLogger::print("=== START LOOP ===");
  engine.startLoop();
  SimpleLogger::print("=== APP ENDED ===");
    try {
        auto server = std::make_shared<TcpServer>(51234);
        Entity entity;
        entity.print();
        server->start();
        std::this_thread::sleep_for(std::chrono::seconds(1000));
        server->stop();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
