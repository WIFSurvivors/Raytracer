#include "includes/RenderSystem.h"
#include "includes/RenderComponent.h"

int main()
{
	core::RenderSystem rs;
	core::RenderComponent comp;
	core::RenderComponent comp02;
	rs._component = std::make_unique<core::RenderComponent>(comp);
	rs.init();

	rs.render(); // shouldn't exist 
	
	rs.destroy();
}



