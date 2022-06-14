#include "LayerStack.h"

LayerStack::LayerStack()
{

}

LayerStack::~LayerStack()
{
    for(Layer* layer : m_Layers)
        delete layer;
}

void LayerStack::pushLayer(Layer* layer)
{
    m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
    m_LayerInsertIndex++;
    layer->attach();
}

void LayerStack::pushOverLayer(Layer* overlayer)
{
    m_Layers.emplace_back(overlayer);
    overlayer->attach();
}

void LayerStack::popLayer(Layer* layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
    if(it != m_Layers.begin() + m_LayerInsertIndex)
    {
        layer->detach();
        m_Layers.erase(it);
        m_LayerInsertIndex--;
    }
}

void LayerStack::popOverLayer(Layer* overlayer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, overlayer);
    if(it != m_Layers.end())
    {
        overlayer->detach();
        m_Layers.erase(it);
    }
}