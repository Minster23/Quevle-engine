#pragma once

#include <fstream>      // if you're reading script files
#include <sstream>      // if you're parsing text content
#include <iostream>     // optional, for debug/logging

#include <future>       // std::async, std::future
#include <chrono>       // std::chrono::seconds
#include <string>       // std::string
#include <functional>   // std::function
#include <atomic>       // std::atomic for thread-safe flags (optional but recommended)


struct script {
    std::string content;
};

// Your class
class AsyncCompiler {
public:
    void loadScript(const std::string& path);
    void update(std::function<void(const script&)> onDone);

private:
    std::atomic<bool> loading = false;
    std::string currentPath;
    std::future<script> scriptFuture;
};