#include <X11/Xlib.h>
#include <assert.h>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <functional>
#include <vulkan/vulkan.h>

#define NIL (0)

#define print(printable) std::cout << printable << std::endl;

class Renderer {
    private:
    VkInstance instance;
    public:

    Renderer() {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "X11Vulkan";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledLayerCount = 0;

        if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
    }

    void vulkanRender() {

    }
};

void callback(XEvent e, Display *display, Window window, GC gc) {
    print("callback");
}

void xEventThread(Display *display, std::function<void(XEvent, Display*, Window, GC)> callback, Window window, GC gc) {
    
    XSelectInput(display, window, ExposureMask);
    while(1) {
        XEvent e;
        XNextEvent(display, &e);
        callback(e, display, window, gc);
    }
}

int main() {
    std::clock_t time = std::clock();
    double cps = CLOCKS_PER_SEC;

    Display *display = XOpenDisplay(NIL);
    assert(display);
    int blackColor = BlackPixel(display, DefaultScreen(display));
    int whiteColor = WhitePixel(display, DefaultScreen(display));
    Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 200, 100, 0, blackColor, blackColor);  
    Renderer renderer = Renderer();

    XMapWindow(display, window);
    GC gc = XCreateGC(display, window, 0, NIL);
    XSetForeground(display, gc, whiteColor);

    std::thread eventThread (xEventThread, display, callback, window, gc); 

    while(1) {
        if((std::clock()-time) >= cps/60) {
            time = std::clock();
            renderer.vulkanRender();
        }
    }
}
