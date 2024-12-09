#include "LoadingScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene* LoadingScene::createScene()
{
    return LoadingScene::create();
}

bool LoadingScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto fondo = Sprite::create("fondoCarga.png");
    Size tamaDise = Director::getInstance()->getVisibleSize();
    fondo->setContentSize(tamaDise);
    fondo->setPosition(tamaDise / 2);
    this->addChild(fondo);

    auto cargaSprite = Sprite::create("loadingNaranja.png");
    cargaSprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
    this->addChild(cargaSprite);

    auto rotarAccion = RotateBy::create(.9f, -350);
    auto repetir = RepeatForever::create(rotarAccion);
    cargaSprite->runAction(repetir);

    auto tiempoCarga = DelayTime::create(5.0f);
    auto funcionMain = CallFunc::create([this]() {
        this->transitionMain(0.0f);
        });

    auto cambio = Sequence::create(tiempoCarga, funcionMain, NULL);
    this->runAction(cambio);

    return true;
}

void LoadingScene::transitionMain(float tiempo) {
    auto escenaMain = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, escenaMain));
}