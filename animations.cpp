#include <emscripten.h>
#include <string>

class AnimationManager {
private:
    std::string currentAnimation = "fade";
    
public:
    void setAnimation(const std::string& animationType) {
        currentAnimation = animationType;
    }
    
    std::string getAnimationCSS() const {
        if (currentAnimation == "fade") {
            return "transition: opacity 0.5s ease;";
        } else if (currentAnimation == "slide") {
            return "transition: transform 0.3s ease;";
        } else if (currentAnimation == "bounce") {
            return "animation: bounce 0.5s ease;";
        }
        return "";
    }
};

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void setAnimationType(const char* type) {
        // Функция для установки типа анимации
    }
}