# WebServ

Config class defines server config;
HttpServer class defines server creation and inlcudes the whloe logic of server;

```
	if (webServer.openServer() == FAIL)
		return (1);

	if (webServer.runServer() == FAIL) //runServer() includes Connection class methods
		return (1);
```
webServer.openServer() : socket() + bind() + listen();
webServer.runServer() : accept() + read() + write();
