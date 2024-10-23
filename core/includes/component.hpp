#pragma once

struct component {
  // Life cycle of a component:

  /// @brief Use the constructor to internally initialize the object
  component() = default;

  /// @brief Appendum to consturctor for referencing external objects
  virtual void init();

  /// @brief Use to update component's logic each tick
  /// @param dt Time step
  virtual void update(float dt);

  /// @brief Use to delete references to external objects
  virtual void destroy();

  /// @brief Use the constructor to internally deconstruct the object
  virtual ~component() = default;

private:
};
