#ifndef _GLOBAL_CONTEXT_WIDGET_H
#define _GLOBAL_CONTEXT_WIDGET_H

#include "global_settings.h"
#include "widget.h"

namespace glt {

class GlobalContextWidget : public Widget<GlobalSettings*> {
public:
  GlobalContextWidget();
  void draw() override;
};

} // namespace glt

#endif // _GLOBAL_CONTEXT_WIDGET_H