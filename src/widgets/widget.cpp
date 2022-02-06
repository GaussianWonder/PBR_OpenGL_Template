#include "widget.h"
#include "global_settings.h"

namespace glt {

template<typename T>
Widget<T>::Widget(T model)
  :model(model)
{}

// make sure the compiler knows about this

template class Widget<GlobalSettings*>;

} // namespace glt
