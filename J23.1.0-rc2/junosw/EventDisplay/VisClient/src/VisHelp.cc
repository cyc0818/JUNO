#include "VisClient/VisHelp.h"

const char* VisHelp::m_authorText = "\
The JUNO Event Display Software \n\
SERENA (\"Software of Event display with Root Eve for Neutrino Analysis\") \n\
Developed by \n\
    Yumei Zhang (Sun-Yat-Sen University) and Zhengyun You \n\
";

// copied from TGLSAViewer.cxx

const char* VisHelp::m_helpText1 = "\
DIRECT SCENE INTERACTIONS\n\n\
   Press:\n\
   \tw          --- wireframe mode\n\
   \te          --- switch between dark / light color-set\n\
   \tr          --- filled polygons mode\n\
   \tt          --- outline mode\n\
   \tj          --- Move camera forward\n\
   \tk          --- Move camera backward\n\
   \ta          --- switch on/off arc-ball camera rotation control\n\
   \tArrow Keys --- PAN (TRUCK) across scene\n\
   \tHome       --- reset current camera\n\
   \tCtrl-Home  --- switch external/automatic camera center\n\
\n\
   By pressing Ctrl and Shift keys the mouse precision can be changed:\n\
     Shift      -- 10 times less precise\n\
     Ctrl       -- 10 times more precise\n\
     Ctrl Shift -- 100 times more precise\n\
\n\
   LEFT mouse button -- ROTATE (ORBIT) the scene by holding the mouse button and moving\n\
   the mouse (perspective camera, needs to be enabled in menu for orthograpic cameras).\n\
   By default, the scene will be rotated about its center. To select arbitrary center\n\
   bring up the viewer-editor (e.g., shift-click into empty background) and use\n\
   'Camera center' controls in the 'Guides' tab.\n\
\n\
   MIDDLE mouse button or arrow keys --  PAN (TRUCK) the camera.\n\
\n\
   RIGHT mouse button action depends on camera type:\n\
     orthographic -- zoom,\n\
     perspective  -- move camera forwards / backwards\n\
\n\
   Mouse wheel action depends on camera type:\n\
     orthographic -- zoom,\n\
     perspective  -- change field-of-view (focal length)\n\
\n\
   To invert direction of mouse and key actions from scene-centric\n\
   to viewer-centric, set in your .rootrc file:\n\
      OpenGL.EventHandler.ViewerCentricControls: 1\n\
\n\
   Double clik will show GUI editor of the viewer (if assigned).\n\
\n\
   RESET the camera via the button in viewer-editor or Home key.\n\
\n\
   SELECT a shape with Shift+Left mouse button click.\n\
\n\
   SELECT the viewer with Shift+Left mouse button click on a free space.\n\
\n\
   MOVE a selected shape using Shift+Mid mouse drag.\n\
\n\
   Invoke the CONTEXT menu with Shift+Right mouse click.\n\n"
   "Secondary selection and direct render object interaction is initiated\n\
   by Alt+Left mouse click (Mod1, actually). Only few classes support this option.\n\
   When 'Alt' is taken by window manager, try Alt-Ctrl-Left.\n\
\n\
CAMERA\n\
\n\
   The \"Camera\" menu is used to select the different projections from \n\
   the 3D world onto the 2D viewport. There are three perspective cameras:\n\
\n\
   \tPerspective (Floor XOZ)\n\
   \tPerspective (Floor YOZ)\n\
   \tPerspective (Floor XOY)\n\
\n\
   In each case the floor plane (defined by two axes) is kept level.\n\
\n\
   There are also three orthographic cameras:\n\
\n\
   \tOrthographic (XOY)\n\
   \tOrthographic (XOZ)\n\
   \tOrthographic (ZOY)\n\
\n\
   In each case the first axis is placed horizontal, the second vertical e.g.\n\
   XOY means X horizontal, Y vertical.\n\n";

const char* VisHelp::m_helpText2 = "\
SHAPES COLOR AND MATERIAL\n\
\n\
   The selected shape's color can be modified in the Shapes-Color tabs.\n\
   Shape's color is specified by the percentage of red, green, blue light\n\
   it reflects. A surface can reflect DIFFUSE, AMBIENT and SPECULAR light.\n\
   A surface can also emit light. The EMISSIVE parameter allows to define it.\n\
   The surface SHININESS can also be modified.\n\
\n\
SHAPES GEOMETRY\n\
\n\
   The selected shape's location and geometry can be modified in the Shapes-Geom\n\
   tabs by entering desired values in respective number entry controls.\n\
\n\
SCENE CLIPPING\n\
\n\
   In the Scene-Clipping tabs select a 'Clip Type': None, Plane, Box\n\
\n\
   For 'Plane' and 'Box' the lower pane shows the relevant parameters:\n\
\n\
\tPlane: Equation coefficients of form aX + bY + cZ + d = 0\n\
\tBox: Center X/Y/Z and Length X/Y/Z\n\n"
   "For Box checking the 'Show / Edit' checkbox shows the clip box (in light blue)\n\
   in viewer. It also attaches the current manipulator to the box - enabling\n\
   direct editing in viewer.\n\
\n\
MANIPULATORS\n\
\n\
   A widget attached to the selected object - allowing direct manipulation\n\
   of the object with respect to its local axes.\n\
\n\
   There are three modes, toggled with keys while manipulator is active, that is,\n\
   mouse pointer is above it (switches color to yellow):\n\
   \tMode\t\tWidget Component Style\t\tKey\n\
   \t----\t\t----------------------\t\t---\n\
   \tTranslation\tLocal axes with arrows\t\tv\n\
   \tScale\t\tLocal axes with boxes\t\tx\n\
   \tRotate\t\tLocal axes rings\t\tc\n\
\n\
   Each widget has three axis components - red (X), green (Y) and blue (Z).\n\
   The component turns yellow, indicating an active state, when the mouse is moved\n\
   over it. Left click and drag on the active component to adjust the objects\n\
   translation, scale or rotation.\n\
   Some objects do not support all manipulations (e.g. clipping planes cannot be \n\
   scaled). If a manipulation is not permitted the component it drawn in grey and \n\
   cannot be selected/dragged.\n";

