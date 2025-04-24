#include <main.h>

void handleRoot() {
    server.send(200, "text/html", generateHTML());
}