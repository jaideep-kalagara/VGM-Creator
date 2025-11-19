#include "app.h"

int main() {
    App app = App();
    app.init();
    app.run();
    app.terminate();
    return 0;
}