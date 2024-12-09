#include "HelloWorldScene.h"
#include "JuegoInicio.h"
#include "musicaFondo.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;

Scene* HelloWorld::createScene() {
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init() {
    auto fondoMain = Sprite::create("fondoMain.png");
    fondoMain->setPosition(Director::getInstance()->getVisibleSize() / 2);
    this->addChild(fondoMain, -1);

    //botonJuego
    auto botonJuegoInicio = MenuItemImage::create(
        "botonPlay.png", "botonPlay.png", CC_CALLBACK_1(HelloWorld::openJuegoInicio, this));
    botonJuegoInicio->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        botonJuegoInicio->getContentSize().height / 2 + 200));

    auto listenerJuego = EventListenerTouchOneByOne::create();
    listenerJuego->onTouchBegan = [=](Touch* touch, Event* event) {
        if (botonJuegoInicio->getBoundingBox().containsPoint(touch->getLocation())) {
            botonJuegoInicio->runAction(ScaleTo::create(0.1f, 1.2f));
            return true;
        }
        return false;
        };
    listenerJuego->onTouchEnded = [=](Touch* touch, Event* event) {
        botonJuegoInicio->runAction(ScaleTo::create(0.1f, 1.0f));};

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerJuego, botonJuegoInicio);

    // boton Historial
    auto botonHistorial = MenuItemImage::create(
        "historialBoton.png", "historialBoton.png", CC_CALLBACK_1(HelloWorld::openHistorial, this));
    botonHistorial->setPosition(Vec2(botonJuegoInicio->getPositionX() + 300 + botonHistorial->getContentSize().width / 2,
        botonJuegoInicio->getPositionY() - 160));
    auto listenerHistorial = EventListenerTouchOneByOne::create();
    listenerHistorial->onTouchBegan = [=](Touch* touch, Event* event) {
        if (botonHistorial->getBoundingBox().containsPoint(touch->getLocation())) {
            botonHistorial->runAction(ScaleTo::create(0.1f, 1.2f));
            return true;
        }
        return false;
        };
    listenerHistorial->onTouchEnded = [=](Touch* touch, Event* event) {
        botonHistorial->runAction(ScaleTo::create(0.1f, 1.0f));};

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerHistorial, botonHistorial);

    //boton Perfil
    auto botonPerfil = MenuItemImage::create(
        "perfilBoton.png", "perfilBoton.png", CC_CALLBACK_1(HelloWorld::openPerfil, this));
    botonPerfil->setPosition(Vec2(130, Director::getInstance()->getVisibleSize().height - 130));
    auto listenerPerfil = EventListenerTouchOneByOne::create();
    listenerPerfil->onTouchBegan = [=](Touch* touch, Event* event) {
        if (botonPerfil->getBoundingBox().containsPoint(touch->getLocation())) {
            botonPerfil->runAction(ScaleTo::create(0.1f, 1.2f));
            return true;
        }
        return false;
        };
    listenerPerfil->onTouchEnded = [=](Touch* touch, Event* event) {
        botonPerfil->runAction(ScaleTo::create(0.1f, 1.0f));};

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerPerfil, botonPerfil);

    //boton COnfig
    auto botonConfiguracion = MenuItemImage::create(
        "configBoton.png", "configBoton.png", CC_CALLBACK_1(HelloWorld::openConfiguracion, this));
    botonConfiguracion->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 80,
        Director::getInstance()->getVisibleSize().height - 130));

    auto listenerConfig = EventListenerTouchOneByOne::create();
    listenerConfig->onTouchBegan = [=](Touch* touch, Event* event) {
        if (botonConfiguracion->getBoundingBox().containsPoint(touch->getLocation())) {
            auto rotacion = RotateBy::create(0.1f, 120);
            auto regreso = RotateBy::create(0.1f, -120);
            botonConfiguracion->runAction(Sequence::create(rotacion, regreso, nullptr));
            return true;
        }
        return false;
        };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerConfig, botonConfiguracion);

    auto botonTutorial = MenuItemImage::create(
        "tutorialBoton.png", "tutorialBoton.png",
        CC_CALLBACK_1(HelloWorld::openTutorial, this));
    botonTutorial->setPosition(Vec2(botonJuegoInicio->getPositionX() - 500 + botonTutorial->getContentSize().width / 2,
        botonJuegoInicio->getPositionY() - 150));

    auto listenerTutorial = EventListenerTouchOneByOne::create();
    listenerTutorial->onTouchBegan = [=](Touch* touch, Event* event) {
        if (botonTutorial->getBoundingBox().containsPoint(touch->getLocation())) {
            botonTutorial->runAction(ScaleTo::create(0.1f, 1.2f));
            return true;
        }
        return false;
        };
    listenerTutorial->onTouchEnded = [=](Touch* touch, Event* event) {
        botonTutorial->runAction(ScaleTo::create(0.1f, 1.0f));
        };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerTutorial, botonTutorial);

    auto menu = Menu::create(botonJuegoInicio, botonHistorial, botonPerfil, botonConfiguracion, botonTutorial, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu); 

    return true;
}

void HelloWorld::openTutorial(Ref* sender) {
    capaTutorial = Layer::create();
    paginaActualTutorial = 1;

    auto fondoOscuro = LayerColor::create(Color4B(0, 0, 0, 160));
    capaTutorial->addChild(fondoOscuro);

    auto ventanaTutorial = Sprite::create("ventanaHistorial.png");
    ventanaTutorial->setPosition(Director::getInstance()->getVisibleSize() / 2);
    capaTutorial->addChild(ventanaTutorial);

    contenedorImagenTutorial = Node::create();
    contenedorImagenTutorial->setPosition(Vec2(ventanaTutorial->getContentSize().width / 2,
        ventanaTutorial->getContentSize().height / 2));
    ventanaTutorial->addChild(contenedorImagenTutorial);

    realizarTransicionImagen("tutorial1.png");

    auto botonAnterior = MenuItemImage::create(
        "flechaI.png", "flechaI.png",
        CC_CALLBACK_1(HelloWorld::anteriorPaginaTutorial, this));
    botonAnterior->setPosition(Vec2(-ventanaTutorial->getContentSize().width / 2 + 50, 0));

    auto botonSiguiente = MenuItemImage::create(
        "flechaD.png", "flechaD.png",
        CC_CALLBACK_1(HelloWorld::siguientePaginaTutorial, this));
    botonSiguiente->setPosition(Vec2(ventanaTutorial->getContentSize().width / 2 - 50, 0));

    auto botonCerrar = MenuItemImage::create(
        "cruzCerrar.png", "cruzCerrar.png",
        CC_CALLBACK_1(HelloWorld::cerrarTutorial, this));
    botonCerrar->setPosition(Vec2(
        ventanaTutorial->getContentSize().width / 2 - 60,
        ventanaTutorial->getContentSize().height / 2 - 65));

    auto menu = Menu::create(botonAnterior, botonSiguiente, botonCerrar, nullptr);
    menu->setPosition(Vec2(ventanaTutorial->getContentSize().width / 2,
        ventanaTutorial->getContentSize().height / 2));
    ventanaTutorial->addChild(menu);

    this->addChild(capaTutorial, 100);
}

void HelloWorld::siguientePaginaTutorial(Ref* sender) {
    if (paginaActualTutorial < TOTAL_PAGINAS_TUTORIAL && contenedorImagenTutorial) {
        paginaActualTutorial++;
        actualizarPaginaTutorial();
    }
}

void HelloWorld::anteriorPaginaTutorial(Ref* sender) {
    if (paginaActualTutorial > 1 && contenedorImagenTutorial) {
        paginaActualTutorial--;
        actualizarPaginaTutorial();
    }
}

void HelloWorld::actualizarPaginaTutorial() {
    if (!contenedorImagenTutorial) return;

    string nombreImagen = "tutorial" + to_string(paginaActualTutorial) + ".png";
    realizarTransicionImagen(nombreImagen);
}

void HelloWorld::cerrarTutorial(Ref* sender) {
    if (capaTutorial) {
        if (imagenTutorialActual) {
            imagenTutorialActual->stopAllActions();
        }
        if (contenedorImagenTutorial) {
            contenedorImagenTutorial->stopAllActions();
        }
        imagenTutorialActual = nullptr;
        contenedorImagenTutorial = nullptr;

        capaTutorial->removeFromParent();
        capaTutorial = nullptr;
    }
}

void HelloWorld::openJuegoInicio(Ref* sender) {
    Director::getInstance()->replaceScene(JuegoInicio::createScene());
}

void HelloWorld::openHistorial(Ref* sender) {
    capaHistorial = Layer::create();
    auto fondoOscuro = LayerColor::create(Color4B(0, 0, 0, 160));
    capaHistorial->addChild(fondoOscuro);

    auto ventanaHistorial = Sprite::create("ventanaHistorial.png");
    ventanaHistorial->setPosition(Director::getInstance()->getVisibleSize() / 2);
    ventanaHistorial->setName("ventanaHistorial");
    capaHistorial->addChild(ventanaHistorial);

    auto scrollView = ui::ScrollView::create();
    scrollView->setName("scrollView");
    scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    scrollView->setContentSize(Size(ventanaHistorial->getContentSize().width - 100,
        ventanaHistorial->getContentSize().height - 200));
    scrollView->setPosition(Vec2(50, 100));
    scrollView->setBackGroundColorType(ui::Layout::BackGroundColorType::NONE);
    ventanaHistorial->addChild(scrollView);

    auto botonCerrar = MenuItemImage::create(
        "cruzCerrar.png", "cruzCerrar.png",
        CC_CALLBACK_1(HelloWorld::cerrarHistorial, this));
    botonCerrar->setPosition(Vec2(
        Director::getInstance()->getVisibleSize().width / 2 + ventanaHistorial->getContentSize().width / 2 - 60,
        Director::getInstance()->getVisibleSize().height / 2 + ventanaHistorial->getContentSize().height / 2 - 65));

    auto menu = Menu::create(botonCerrar, nullptr);
    menu->setPosition(Vec2::ZERO);
    capaHistorial->addChild(menu);

    cargarContenidoHistorial(ventanaHistorial);

    this->addChild(capaHistorial, 100);
}

void HelloWorld::cerrarHistorial(Ref* sender) {
    if (capaHistorial) {
        capaHistorial->removeFromParent();
        capaHistorial = nullptr;
    }
}

void HelloWorld::realizarTransicionImagen(const std::string& nombreNuevaImagen) {
    auto nuevaImagen = Sprite::create(nombreNuevaImagen);
    if (!nuevaImagen || !contenedorImagenTutorial) return;

    nuevaImagen->setPosition(Vec2::ZERO);
    nuevaImagen->setOpacity(0);
    contenedorImagenTutorial->addChild(nuevaImagen);

    if (imagenTutorialActual && imagenTutorialActual->getParent()) {
        auto imagenAnterior = imagenTutorialActual;

        auto fadeOut = FadeOut::create(0.2f);
        auto remover = CallFunc::create([imagenAnterior]() {
            if (imagenAnterior && imagenAnterior->getParent()) {
                imagenAnterior->removeFromParent();
            }
            });

        imagenAnterior->runAction(Sequence::create(fadeOut, remover, nullptr));
    }

    auto fadeIn = FadeIn::create(0.2f);
    nuevaImagen->runAction(fadeIn);

    imagenTutorialActual = nuevaImagen;
}

void HelloWorld::cargarContenidoHistorial(Node* ventanaHistorial) {
    std::string filePath = FileUtils::getInstance()->getWritablePath() + "historial_partidas.txt";
    std::string contenido = FileUtils::getInstance()->getStringFromFile(filePath);

    auto scrollView = static_cast<ui::ScrollView*>(ventanaHistorial->getChildByName("scrollView"));
    if (!scrollView) {
        return;
    }

    float yPos = ventanaHistorial->getContentSize().height - 150;
    auto container = Node::create();
    float contentHeight = 0;

    if (contenido.empty()) {
        auto label = Label::createWithTTF("No hay partidas registradas", "fonts/arial.ttf", 30);
        label->setPosition(Vec2(ventanaHistorial->getContentSize().width / 2,
            ventanaHistorial->getContentSize().height / 2));
        label->setColor(Color3B::BLACK);
        ventanaHistorial->addChild(label);
        return;
    }

    istringstream stream(contenido);
    string linea;

    while (std::getline(stream, linea)) {
        auto labelPartida = Label::createWithTTF(linea, "fonts/arial.ttf", 30);
        labelPartida->setPosition(Vec2(ventanaHistorial->getContentSize().width / 2, yPos));
        labelPartida->setColor(Color3B::BLACK);
        container->addChild(labelPartida);
        yPos -= 40;
        contentHeight += 40;
    }

    container->setPosition(Vec2::ZERO);
    scrollView->addChild(container);
    scrollView->setInnerContainerSize(Size(scrollView->getContentSize().width,
        max(contentHeight, scrollView->getContentSize().height)));
}

void HelloWorld::openConfiguracion(Ref* sender) {
    capaPopup = Layer::create();
    auto fondoOscuro = LayerColor::create(Color4B(0, 0, 0, 160));
    capaPopup->addChild(fondoOscuro);

    //ventana
    auto panelConfig = Sprite::create("ventanaConfig.png");
    panelConfig->setPosition(Director::getInstance()->getVisibleSize() / 2);
    capaPopup->addChild(panelConfig);

    auto label = Label::createWithTTF("Ajusta el volumen:", "fonts/arial.ttf", 60);
    label->setPosition(Vec2(panelConfig->getContentSize().width / 2,
        panelConfig->getContentSize().height - 450));
    label->setColor(Color3B::BLACK);
    panelConfig->addChild(label);

    // Control de volumen
    volumeSlider = Slider::create();
    volumeSlider->loadBarTexture("volLinea.png");
    volumeSlider->loadSlidBallTextures("volFlecha.png", "volFlecha.png", "");
    volumeSlider->loadProgressBarTexture("volProgreso.png");
    volumeSlider->setPosition(Vec2(panelConfig->getPositionX() + 50,
        panelConfig->getPositionY()));
    volumeSlider->setPercent(audioControl::getInstance()->getVolume() * 100);
    volumeSlider->addEventListener(CC_CALLBACK_2(HelloWorld::onVolumeChanged, this));
    capaPopup->addChild(volumeSlider);

    //cerrar
    auto botonCerrar = MenuItemImage::create(
        "cruzCerrar.png", "cruzCerrar.png", CC_CALLBACK_1(HelloWorld::cerrarVentana, this)
    );
    botonCerrar->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 + panelConfig->getContentSize().width / 2 - 60,
        Director::getInstance()->getVisibleSize().height / 2 + panelConfig->getContentSize().height / 2 - 65));

    auto menuCerrar = Menu::create(botonCerrar, nullptr);
    menuCerrar->setPosition(Vec2::ZERO);
    capaPopup->addChild(menuCerrar);

    this->addChild(capaPopup, 100);
}

void HelloWorld::onVolumeChanged(Ref* sender, Slider::EventType type) {
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
        float volume = volumeSlider->getPercent() / 100.0f;
        audioControl::getInstance()->setBackgroundMusicVolume(volume);
    }
}

void HelloWorld::openPerfil(Ref* sender) {
    capaPopup = Layer::create();
    auto fondoOscuro = LayerColor::create(Color4B(0, 0, 0, 160));
    capaPopup->addChild(fondoOscuro);
    //ventana
    auto panelPerfil = Sprite::create("ventanaPerfil.png");
    panelPerfil->setPosition(Director::getInstance()->getVisibleSize() / 2);
    capaPopup->addChild(panelPerfil);

    auto avatarFrame = Sprite::create("marcoAvatar.png");
    avatarFrame->setPosition(Vec2(panelPerfil->getContentSize().width / 2,
        panelPerfil->getContentSize().height - 350));
    panelPerfil->addChild(avatarFrame);

    auto label = Label::createWithTTF("ROJO", "fonts/arial.ttf", 70);
    label->setPosition(Vec2(panelPerfil->getContentSize().width / 2,
        panelPerfil->getContentSize().height- 600));
    label->setColor(Color3B::BLACK);
    panelPerfil->addChild(label);

    auto label2 = Label::createWithTTF("Mueve solo fichas rojas", "fonts/arial.ttf", 60);
    label2->setPosition(Vec2(panelPerfil->getContentSize().width / 2,
        panelPerfil->getContentSize().height - 750));
    label2->setColor(Color3B::BLACK);
    panelPerfil->addChild(label2);

    auto botonCerrar = MenuItemImage::create(
        "cruzCerrar.png", "cruzCerrar.png",
        CC_CALLBACK_1(HelloWorld::cerrarVentana, this));
    botonCerrar->setPosition(Vec2(
        Director::getInstance()->getVisibleSize().width / 2 + panelPerfil->getContentSize().width / 2 - 60,
        Director::getInstance()->getVisibleSize().height / 2 + panelPerfil->getContentSize().height / 2 - 65));

    auto menuCerrar = Menu::create(botonCerrar, nullptr);
    menuCerrar->setPosition(Vec2::ZERO);
    capaPopup->addChild(menuCerrar);

    this->addChild(capaPopup, 100);
}

void HelloWorld::cerrarVentana(Ref* sender) {
    if (capaPopup) {
        capaPopup->removeFromParent();
        capaPopup = nullptr;
    }
}