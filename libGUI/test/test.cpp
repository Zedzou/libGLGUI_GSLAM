#include "../GUI.h"

int main(){
    GLGUI::GUI_base guiBase;
    guiBase.initWindow("Test_window", 1200, 800);
    guiBase.run();
    return 0;
}