#ifndef __PERFIL_POP_H__
#define __PERFIL_POP_H__

#include "cocos2d.h"

class PerfilPop : public cocos2d::Scene
{
public:
    static cocos2d::Scene* create();
    virtual bool init();

private:
    void cerrarVentana(cocos2d::Ref* sender);
    cocos2d::Layer* capaPerfil;
};

#endif // __PERFIL_POP_H__
