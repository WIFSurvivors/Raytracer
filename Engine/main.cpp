#include "includes/Engine.hpp"
#include "includes/utility/SimpleLogger.hpp"

int main() {
  SimpleLogger::print("=== APP STARTED ===");
  Engine engine{};
  SimpleLogger::print("=== START LOOP ===");
  engine.startLoop();
  SimpleLogger::print("=== APP ENDED ===");
  return 0;
}
