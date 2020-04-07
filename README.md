# aw-plugin-http

HTTP plugin for Angels' Ware rendering engine.

### Getting Started

Add the **plugins** section to the **config.yaml** file of your project.

```
...

plugins:
-	repo: https://github.com/angelsware/aw-plugin-http.git
```

```
Http::CClient* client = Http::CClientFactory::create();
client->addListener(this);

Http::CRequest request("GET", Type::CUri("https://angelsware.com"));
client->send(request);

Http::CClientFactory::destroy(client);

```
