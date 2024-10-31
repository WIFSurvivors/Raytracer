#include "Engine.h"
#include "RenderComponent.h"

int main()
{
	core::RenderSystem rs;
	core::RenderComponent comp;
	rs._component = std::make_unique<core::RenderComponent>(comp);

	rs.init();
	rs.render();
	rs.destroy();
}



