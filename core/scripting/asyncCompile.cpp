#include <core/scripting/asyncCompile.hpp>
#include <thread>

void AsyncCompiler::loadScript(const std::string& path) {
    if (loading) return;

    loading = true;
    currentPath = path;

    scriptFuture = std::async(std::launch::async, [path]() -> script {
        script s;

        // Simulate loading or compiling the script
        std::ifstream file(path);
        if (file) {
            std::ostringstream ss;
            ss << file.rdbuf();
            s.content = ss.str();  // pretend it's compiled
        } else {
            s.content = "// failed to load script";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // simulate compile time
        return s;
    });
}

void AsyncCompiler::update(std::function<void(const script&)> onDone) {
    if (!loading) return;

    if (scriptFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
        script result = scriptFuture.get();
        onDone(result);
        loading = false;
    }
}