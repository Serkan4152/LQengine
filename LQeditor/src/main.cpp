#include "Core/App.h"
#include "Editor.h"

int main(int argc, char** argv)
{
    std::stringstream name;
    name << APP_NAME << " v" << APP_VERSION;

    #if defined(_DEBUG)
        name << " - [DEBUG]";
    #endif

    auto app = LQ::App(argc, argv);
    app.set_name(APP_NAME)
        .set_window(LQ::WindowInfo{
            .title = name.str(),
            .size = {
                .width = 1280,
                .height = 720
            },
            .flags = LQ::WindowFlags::Resizable
        })
        .with<>(LQ::DefaultModules{})
        //.with<LQ::Renderer>()
        .with<LQ::Editor>()
        .run();

    return 0;
}