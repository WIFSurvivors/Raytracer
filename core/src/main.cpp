#include "includes/RenderSystem.h"
#include "includes/RenderComponent.h"
int main()
{
	core::RenderSystem rs;
	core::RenderComponent comp;
  	core::RenderComponent comp02;
	//TcpServer server(8080);
	//server.start();
	rs._component = std::make_unique<core::RenderComponent>(comp02);
	rs.init();

	rs.render(); // shouldn't exist 
	
	rs.destroy();
}
