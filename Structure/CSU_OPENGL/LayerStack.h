#ifndef _LAYER_STACK_H_
#define _LAYER_STACK_H_

#include <vector>
#include <algorithm>
#include "Layer.h"

class LayerStack
{
public:
    LayerStack();
    ~LayerStack();

    void pushLayer(Layer* layer);
    void pushOverLayer(Layer* overlayer);
    void popLayer(Layer* layer);
    void popOverLayer(Layer* overlayer);

    std::vector<Layer*>::iterator begin() {return m_Layers.begin();}
    std::vector<Layer*>::iterator end() {return m_Layers.end();}
private:
    std::vector<Layer*> m_Layers;
    uint32_t m_LayerInsertIndex = 0; 
};

#endif