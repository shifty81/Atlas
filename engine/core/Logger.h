#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <ostream>

namespace atlas {

class Logger {
public:
    static void Init();
    static void Shutdown();
    static void Info(const std::string& msg);
    static void Warn(const std::string& msg);
    static void Error(const std::string& msg);

private:
    static void Write(const std::string& line, std::ostream& console);

    static std::ofstream s_logFile;
    static std::mutex s_mutex;
};

}
