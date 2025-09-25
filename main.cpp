#include <emscripten.h>
#include <emscripten/html5.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>

class VirtualLinux {
private:
    struct Application {
        std::string name;
        std::string icon;
        std::function<void()> launch;
        bool isOpen = false;
    };
    
    struct Window {
        int id;
        std::string title;
        int x, y;
        int width, height;
        bool isMinimized = false;
        Application* app;
    };
    
    std::map<std::string, Application> applications;
    std::vector<Window> windows;
    int nextWindowId = 1;
    bool showDesktop = true;
    
public:
    VirtualLinux() {
        initializeApplications();
    }
    
    void initializeApplications() {
        // Терминал
        applications["terminal"] = {
            "Terminal", "📁", [this]() { launchTerminal(); }, false
        };
        
        // Файловый менеджер
        applications["filemanager"] = {
            "File Manager", "📄", [this]() { launchFileManager(); }, false
        };
        
        // Текстовый редактор
        applications["editor"] = {
            "Text Editor", "📝", [this]() { launchTextEditor(); }, false
        };
        
        // Настройки анимаций
        applications["animations"] = {
            "Animation Settings", "⚙️", [this]() { launchAnimationSettings(); }, false
        };
    }
    
    void launchTerminal() {
        Window window;
        window.id = nextWindowId++;
        window.title = "Terminal";
        window.x = 100;
        window.y = 100;
        window.width = 600;
        window.height = 400;
        window.app = &applications["terminal"];
        windows.push_back(window);
        
        applications["terminal"].isOpen = true;
        std::cout << "Terminal launched!" << std::endl;
    }
    
    void launchFileManager() {
        Window window;
        window.id = nextWindowId++;
        window.title = "File Manager";
        window.x = 150;
        window.y = 150;
        window.width = 700;
        window.height = 500;
        window.app = &applications["filemanager"];
        windows.push_back(window);
        
        applications["filemanager"].isOpen = true;
        std::cout << "File Manager launched!" << std::endl;
    }
    
    void launchTextEditor() {
        Window window;
        window.id = nextWindowId++;
        window.title = "Text Editor";
        window.x = 200;
        window.y = 200;
        window.width = 800;
        window.height = 600;
        window.app = &applications["editor"];
        windows.push_back(window);
        
        applications["editor"].isOpen = true;
        std::cout << "Text Editor launched!" << std::endl;
    }
    
    void launchAnimationSettings() {
        Window window;
        window.id = nextWindowId++;
        window.title = "Animation Settings";
        window.x = 250;
        window.y = 250;
        window.width = 500;
        window.height = 300;
        window.app = &applications["animations"];
        windows.push_back(window);
        
        applications["animations"].isOpen = true;
        std::cout << "Animation Settings opened!" << std::endl;
    }
    
    void closeWindow(int windowId) {
        for (auto it = windows.begin(); it != windows.end(); ++it) {
            if (it->id == windowId) {
                it->app->isOpen = false;
                windows.erase(it);
                break;
            }
        }
    }
    
    void minimizeWindow(int windowId) {
        for (auto& window : windows) {
            if (window.id == windowId) {
                window.isMinimized = !window.isMinimized;
                break;
            }
        }
    }
    
    std::string getDesktopHTML() {
        std::string html = R"(
        <div id="virtual-linux">
            <div id="desktop">
                <div id="taskbar">
                    <div id="start-menu">🚀 Start</div>
                    <div id="open-apps"></div>
                </div>
                <div id="windows-container"></div>
            </div>
            <div id="applications-panel">
        )";
        
        // Добавляем иконки приложений
        for (const auto& app : applications) {
            html += R"(
            <div class="app-icon" onclick="launchApp(')" + app.first + R"(')">
                <div class="app-icon-image">)" + app.second.icon + R"(</div>
                <div class="app-icon-name">)" + app.second.name + R"(</div>
            </div>
            )";
        }
        
        html += R"(
            </div>
        </div>
        <style>
        #virtual-linux {
            width: 100vw;
            height: 100vh;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            font-family: Arial, sans-serif;
        }
        
        #desktop {
            position: relative;
            width: 100%;
            height: calc(100% - 50px);
        }
        
        #taskbar {
            position: fixed;
            bottom: 0;
            width: 100%;
            height: 50px;
            background: rgba(0, 0, 0, 0.8);
            display: flex;
            align-items: center;
            padding: 0 10px;
        }
        
        #start-menu {
            background: #4CAF50;
            color: white;
            padding: 10px 20px;
            border-radius: 5px;
            cursor: pointer;
            margin-right: 10px;
        }
        
        #applications-panel {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(80px, 1fr));
            gap: 20px;
            padding: 20px;
        }
        
        .app-icon {
            text-align: center;
            cursor: pointer;
            transition: transform 0.3s ease;
            color: white;
        }
        
        .app-icon:hover {
            transform: scale(1.1);
        }
        
        .app-icon-image {
            font-size: 2em;
            margin-bottom: 5px;
        }
        
        .window {
            position: absolute;
            background: white;
            border-radius: 10px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.3);
            overflow: hidden;
            transition: all 0.3s ease;
        }
        
        .window-header {
            background: #333;
            color: white;
            padding: 10px;
            display: flex;
            justify-content: space-between;
            cursor: move;
        }
        
        .window-content {
            padding: 20px;
            height: calc(100% - 40px);
        }
        </style>
        )";
        
        return html;
    }
};

VirtualLinux virtualLinux;

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void launchApp(const char* appName) {
        virtualLinux.launchApplication(std::string(appName));
    }
    
    EMSCRIPTEN_KEEPALIVE
    void closeWindow(int windowId) {
        virtualLinux.closeWindow(windowId);
    }
}

int main() {
    std::cout << "Virtual Linux Desktop Initialized!" << std::endl;
    return 0;
}