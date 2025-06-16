#include <application/application.hpp>

using namespace QuavleEngine;
application app;

int main(){
    app.init();
    app.tick();
    app.shutdown();
    return 0;
}