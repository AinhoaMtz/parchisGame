#ifndef __JUEGO_INICIO_H__
#define __JUEGO_INICIO_H__

#include "cocos2d.h"
using namespace std;

class JuegoInicio : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(JuegoInicio);

private:
    int actualCantidad;
    cocos2d::Sprite* escojerCantidad;

    void agregarJugadores(cocos2d::Ref* sender);
    void quitarJUgadores(cocos2d::Ref* sender);
    void actualizarJugadores();
    void regresarCb(cocos2d::Ref* pSender);
    void iniciarJuegoCb(cocos2d::Ref* pSender);
};

#endif //__JUEGO_INICIO_H__