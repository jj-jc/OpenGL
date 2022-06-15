#ifndef _LAYER_H_
#define _LAYER_H_

#include <iostream>

class Layer
{
public:
    Layer(const std::string& debugName = "Layer");
    virtual ~Layer() = default;

    virtual void attach() {}
    virtual void detach() {}
    virtual void update(double ts) {}
    virtual void renderImGui() {}
    // virtual void manageEvent() {}

    inline const std::string& getName() const {return m_DebugName;}
protected:
    std::string m_DebugName;    
};

#endif