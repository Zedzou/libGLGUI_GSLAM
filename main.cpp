#include "libGUI_GSLAM/GUI_GSLAM.h"

int main()
{
    GLGUI::GUI_GSLAM gui_gslam("gslam", 800, 600);
    gui_gslam.run();
    return 0;
}