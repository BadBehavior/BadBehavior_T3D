#ifndef _GUI_BTVIEWCTRL_H_
#define _GUI_BTVIEWCTRL_H_

#ifndef _GUI_TREEVIEWCTRL_H
#include "gui/controls/guiTreeViewCtrl.h"
#endif

namespace BadBehavior
{
   // subclassing GuiTreeViewCtrl so that we can tweak it for the behavior tree editor
   class GuiBehaviorTreeViewCtrl : public GuiTreeViewCtrl
   {
      typedef GuiTreeViewCtrl Parent;

   public:
      void onRenderCell(Point2I offset, Point2I cell, bool, bool);

      DECLARE_CONOBJECT(GuiBehaviorTreeViewCtrl);
   };

} // namespace BadBehavior

#endif
