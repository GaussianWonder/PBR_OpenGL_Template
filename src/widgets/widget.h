#ifndef _WIDGET_H_
#define _WIDGET_H_

/**
 * @brief A widget should be the means of user input through the interface, or the interface itself.
 *  Thus, by default it is assumend that the data within the widget is not reactive to other components.
 *  Reactivity, on the other hand, is totally doable.
 * 
 * One can pass a pointer to a widget, enabling unsafe reactivity (smart pointer or not).
 * An alternative is to use a special pub-sub ReactiveWidget.
 * TODO implement ReactiveWidget and provide a concept of reactive data
 */
namespace glt {

template<typename T>
class Widget
{
public:
  T model; //! unsafe, just like every other public model out in this codebase
  virtual void draw() = 0;
  Widget(T model);
};

} // namespace glt

#endif // _WIDGET_H_