#pragma once



struct component
{
	// Life cycle of a component:

	/// @brief Use the constructor to internally initialize the object
	virtual component();

	/// @brief Appendum to consturctor for referencing external objects
	virtual void init();

	/// @brief Use to update component's logic each tick
	/// @param deltaTime Time step
	virtual void update(float deltaTime);

	/// @brief Use to delete references to external objects
	virtual void destroy();

	/// @brief Use the constructor to internally deconstruct the object
	virtual ~component();

private:

};

