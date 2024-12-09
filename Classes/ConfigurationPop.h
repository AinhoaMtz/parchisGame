#ifndef __CONFIGURATION_POP_H__
#define __CONFIGURATION_POP_H__

#include "cocos2d.h"

class ConfigurationPop : public cocos2d::Scene
{
public:
    static cocos2d::Scene* create();
    virtual bool init();

private:
    void cerrarVentana(cocos2d::Ref* sender);
    cocos2d::Layer* capaConfig;
};

#endif // __CONFIGURATION_POP_H__