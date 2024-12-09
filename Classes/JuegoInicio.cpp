#include "JuegoInicio.h"
#include "HelloWorldScene.h"
#include "TableroScene.h"

USING_NS_CC;

Scene* JuegoInicio::createScene()
{
    return JuegoInicio::create();
}

bool JuegoInicio::init() {
    if (!Scene::init())
    {
        return false;
    }
    auto fondo = Sprite::create("fondoJuegoInicio.png");
    fondo->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 + Director::getInstance()->getVisibleOrigin().x,
        Director::getInstance()->getVisibleSize().height / 2 + Director::getInstance()->getVisibleOrigin().y));
    this->addChild(fondo, -1);

    auto botonRegresar = MenuItemImage::create(
        "cruzCerrar.png",
        "cruzCerrar.png",
        CC_CALLBACK_1(JuegoInicio::regresarCb, this));
    botonRegresar->setPosition(Vec2(Director::getInstance()->getVisibleOrigin().x + 1300,
        Director::getInstance()->getVisibleSize().height - 100));

    auto flechaIzquierda = MenuItemImage::create(
        "flechaI2.png",
        "flechaI2.png",
        CC_CALLBACK_1(JuegoInicio::quitarJUgadores, this) );
    flechaIzquierda->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 - 250,
        Director::getInstance()->getVisibleSize().height / 2));

    auto flechaDerecha = MenuItemImage::create(
        "flechaD2.png",
        "flechaD2.png",
        CC_CALLBACK_1(JuegoInicio::agregarJugadores, this)
    );
    flechaDerecha->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 + 250,
        Director::getInstance()->getVisibleSize().height / 2));

    auto botonJugar = MenuItemImage::create(
        "botonJugar.png",
        "botonJugar.png",
        CC_CALLBACK_1(JuegoInicio::iniciarJuegoCb, this));
    botonJugar->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2 - 200));

    auto menu = Menu::create(botonRegresar, flechaIzquierda, flechaDerecha, botonJugar, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    actualCantidad = 2;
    escojerCantidad = Sprite::create("2jugadores.png");
    escojerCantidad->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height - 650));
    this->addChild(escojerCantidad);
    return true;
}

void JuegoInicio::agregarJugadores(Ref* sender){
    actualCantidad++;
    if (actualCantidad > 4) actualCantidad = 2;
    actualizarJugadores();
}

void JuegoInicio::quitarJUgadores(Ref* sender){
    actualCantidad--;
    if (actualCantidad < 2) actualCantidad = 4;
    actualizarJugadores();
}

void JuegoInicio::actualizarJugadores(){
    string nuevo = to_string(actualCantidad) + "jugadores.png";
    auto nuevaCanti = Director::getInstance()->getTextureCache()->addImage(nuevo);
    if (nuevaCanti) {
        escojerCantidad->setTexture(nuevaCanti);
    }
}

void JuegoInicio::regresarCb(Ref* pSender){
    Director::getInstance()->replaceScene(HelloWorld::createScene());
}

void JuegoInicio::iniciarJuegoCb(Ref* pSender){
    auto tab = TableroScene::createScene(actualCantidad);
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, tab));
}