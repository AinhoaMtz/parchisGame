#include "TableroScene.h"
#include "JuegoInicio.h"
#include "audio/include/AudioEngine.h"
#include <vector>

USING_NS_CC;

cocos2d::Scene* TableroScene::createScene(int numJugadores) {
    auto scene = TableroScene::create(numJugadores);

    return scene;
}

TableroScene* TableroScene::create(int numJugadores) {
    TableroScene* ret = new (nothrow) TableroScene();
    if (ret && ret->initWithPlayers(numJugadores)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool TableroScene::init() {
    if (!Scene::init()) return false;
    return true;
}

void TableroScene::inicializarTablero() {
    auto tablero = Sprite::create("tablero.png");
    tablero->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 + Director::getInstance()->getVisibleOrigin().x,
        Director::getInstance()->getVisibleSize().height / 2 + Director::getInstance()->getVisibleOrigin().y));
    this->addChild(tablero, 0);

    auto botonPausa = MenuItemImage::create("botonPausa.png", "botonPausa.png",
        CC_CALLBACK_1(TableroScene::mostrarMenuPausa, this));
    botonPausa->setPosition(Vec2(Director::getInstance()->getVisibleOrigin().x + 80,
        Director::getInstance()->getVisibleSize().height - 80));

    auto menu = Menu::create(botonPausa, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
}

void TableroScene::inicializarFichas() {
    vector<string> coloresFichas = {
        "fichaRoja.png", "fichaAzul.png",
        "fichaAmarilla.png", "fichaVerde.png"
    };

    vector<vector<Vec2>> posicionesCasas = { ROJOS, AZULES, AMARILLOS, VERDES };

    for (int i = 0; i < numJugadores; i++) {
        for (int j = 0; j < 4; j++) {
            auto ficha = Sprite::create(coloresFichas[i]);
            ficha->setPosition(posicionesCasas[i][j]);
            // Las fichas empiezan con tamaño normal en la base
            ficha->setScale(1.0f);
            this->addChild(ficha, 1);
            fichasJugadores.push_back(ficha);
        }
    }
}

bool TableroScene::initWithPlayers(int numJugadores) {
    juegoTerminado = false;
    ventanaGanador = nullptr;
    capaPausa = nullptr;

    permitirNuevoTiro = false;
    fichaEnMovimiento = false;
    fichasBase.resize(numJugadores * 4, true);
    posiFichas.resize(numJugadores, vector<int>(4, -1));
    turnoAct = 0;
    this->numJugadores = numJugadores;
    this->PAUSA = false;
    fichasJugadores.clear();

    JUGADOR_HUMANO.resize(4, false);
    JUGADOR_HUMANO[0] = true;

    inicializarTablero();
    inicializarFichas();
    inicializarDados();


    indicadorTurno = Sprite::create("turnoRojo.png");
    indicadorTurno->setPosition(Vec2(700, 230));
    this->addChild(indicadorTurno, 2);

    fichasEnMeta.resize(numJugadores, std::vector<bool>(4, false)); 
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(TableroScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    return true;
}

void TableroScene::actualizarIndicadorTurno() {
    string texturaTurno;
    switch (turnoAct) {
    case 0: texturaTurno = "turnoRojo.png"; break;
    case 1: texturaTurno = "turnoAzul.png"; break;
    case 2: texturaTurno = "turnoAmarillo.png"; break;
    case 3: texturaTurno = "turnoVerde.png"; break;
    }
    indicadorTurno->setTexture(texturaTurno);
}

bool TableroScene::onTouchBegan(Touch* touch, Event* event) {
    if (juegoTerminado) return false;
    if (!JUGADOR_HUMANO[turnoAct]) return false;

    if (dadosGirando || PAUSA || fichaEnMovimiento)
        return false;

    if (!hayMovimientosDisponibles()) {
        if (valordado1 == 0 && valordado2 == 0) {
            cambiarTurno();
        }
        return false;
    }

    Vec2 touchLoc = touch->getLocation();

    for (int i = 0; i < 4; i++) {
        if (!fichasBase[turnoAct * 4 + i] && !fichasEnMeta[turnoAct][i]) {
            Sprite* ficha = fichasJugadores[turnoAct * 4 + i];
            if (ficha->getBoundingBox().containsPoint(touchLoc)) {
                if ((valordado1 > 0 && esMovimientoValido(i, valordado1)) ||
                    (valordado2 > 0 && esMovimientoValido(i, valordado2))) {
                    mostrarOpcionesDados(i);
                    return true;
                }
            }
        }
    }
    if (verificarSalidaFicha(valordado1, valordado2) && !hayFichaEnSalida(turnoAct)) {
        for (int i = 0; i < 4; i++) {
            if (fichasBase[turnoAct * 4 + i]) {
                Sprite* ficha = fichasJugadores[turnoAct * 4 + i];
                if (ficha->getBoundingBox().containsPoint(touchLoc)) {
                    moverFichaASalida();
                    return true;
                }
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        if (!fichasBase[turnoAct * 4 + i] && !fichasEnMeta[turnoAct][i]) {
            Sprite* ficha = fichasJugadores[turnoAct * 4 + i];
            if (ficha->getBoundingBox().containsPoint(touchLoc)) {
                if (esMovimientoValido(i, valordado1) || esMovimientoValido(i, valordado2)) {
                    mostrarOpcionesDados(i);
                    return true;
                }
            }
        }
    }

    return false;
}

void TableroScene::resetearDados() {
    valordado1 = 0;
    valordado2 = 0;
    permitirNuevoTiro = false;
    cambiarTurno();
}

void TableroScene::mostrarOpcionesDados(int indiceficha) {
    this->removeChildByName("opciones_dados");
    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);

    auto layer = LayerColor::create(Color4B(0, 0, 0, 128));
    this->addChild(layer, 10, "opciones_dados");

    int opcionesDisponibles = 0;

    if (valordado1 > 0) {
        if (esMovimientoValido(indiceficha, valordado1)) {
            auto opcion1 = MenuItemLabel::create(
                Label::createWithTTF("Mover " + std::to_string(valordado1) + " casillas",
                    "fonts/arial.ttf", 30),
                [=](Ref* sender) {
                    moverFicha(indiceficha, valordado1);
                    this->removeChildByName("opciones_dados");
                });
            opcion1->setPosition(Director::getInstance()->getVisibleSize().width / 2,
                Director::getInstance()->getVisibleSize().height / 2 + 50);
            menu->addChild(opcion1);
            opcionesDisponibles++;
        }
    }

    if (valordado2 > 0) {
        if (esMovimientoValido(indiceficha, valordado2)) {
            auto opcion2 = MenuItemLabel::create(
                Label::createWithTTF("Mover " + std::to_string(valordado2) + " casillas",
                    "fonts/arial.ttf", 30),
                [=](Ref* sender) {
                    moverFicha(indiceficha, valordado2);
                    this->removeChildByName("opciones_dados");
                });
            opcion2->setPosition(Director::getInstance()->getVisibleSize().width / 2,
                Director::getInstance()->getVisibleSize().height / 2 - (opcionesDisponibles == 0 ? 0 : 50));
            menu->addChild(opcion2);
            opcionesDisponibles++;
        }
    }

    auto botonCancelar = MenuItemLabel::create(
        Label::createWithTTF("Cancelar", "fonts/arial.ttf", 30),
        [=](Ref* sender) {
            this->removeChildByName("opciones_dados");
        });
    botonCancelar->setPosition(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2 - (opcionesDisponibles * 50 + 25));
    menu->addChild(botonCancelar);

    layer->addChild(menu);
}

bool TableroScene::sonPosicionesCercanas(const Vec2& pos1, const Vec2& pos2, float tolerancia)const {
    return std::abs(pos1.x - pos2.x) <= tolerancia && std::abs(pos1.y - pos2.y) <= tolerancia;
}

void TableroScene::moverFicha(int indiceficha, int cantidadCasillas) {
    if (juegoTerminado || fichaEnMovimiento) {
        return;
    }

    /*if (fichasEnMeta[turnoAct][indiceficha] ||
        indiceficha < 0 || indiceficha >= 4 ||
        turnoAct < 0 || turnoAct >= numJugadores) {
        return;
    }*/
    bool usandoDado1 = (cantidadCasillas == valordado1);
    bool usandoDado2 = (cantidadCasillas == valordado2);

    if (usandoDado1) valordado1 = 0;
    if (usandoDado2) valordado2 = 0;

    /*
    int otroValorDado = 0;
    if (usandoDado1) {
        otroValorDado = valordado2;
        valordado2 = 0;
    }
    else if (usandoDado2) {
        otroValorDado = valordado1;
        valordado1 = 0;
    }*/

    //if (cantidadCasillas <= 0) return;
    fichaEnMovimiento = true;
    int posicionActual = posiFichas[turnoAct][indiceficha];
    vector<Vec2> rutaMovimiento;

    const vector<Vec2>* caminoActual;
    switch (turnoAct) {
    case 0: caminoActual = &CAMINO_ROJO; break;
    case 1: caminoActual = &CAMINO_AZUL; break;
    case 2: caminoActual = &CAMINO_AMARILLO; break;
    case 3: caminoActual = &CAMINO_VERDE; break;
    default: return;
    }

    if (caminoActual->empty()) return;

    int nuevaPosicion = posicionActual + cantidadCasillas;
    bool llegaraAMeta = nuevaPosicion >= caminoActual->size();

    if (llegaraAMeta) {
        Vec2 posicionMeta = obtenerPosicionRandomEnMeta();
        rutaMovimiento.push_back(posicionMeta);

        auto secuencia = Sequence::create(
            MoveTo::create(0.3f, posicionMeta),
            CallFunc::create([this, indiceficha]() {
                fichasEnMeta[turnoAct][indiceficha] = true;
                fichasJugadores[turnoAct * 4 + indiceficha]->setScale(0.7f);
                int fichasEnMetaCount = 0;
                for (int i = 0; i < 4; i++) {
                    if (fichasEnMeta[turnoAct][i]) {
                        fichasEnMetaCount++;
                    }
                }
                if (fichasEnMetaCount == 4) {
                    verificarVictoria();
                }
                fichaEnMovimiento = false;
                if (valordado1 == 0 && valordado2 == 0) {
                    this->scheduleOnce([this](float) {
                        cambiarTurno();
                        }, 0.2f, "cambiar_turno_meta");
                }
                }),
            nullptr
        );
        if (fichasJugadores[turnoAct * 4 + indiceficha]) {
            fichasJugadores[turnoAct * 4 + indiceficha]->runAction(secuencia);
        }
        if (usandoDado1) valordado1 = 0;
        if (usandoDado2) valordado2 = 0;
        return;
    }

    if (posicionActual == -1) {
        for (int i = 0; i < cantidadCasillas && i < caminoActual->size(); i++) {
            rutaMovimiento.push_back((*caminoActual)[i]);
        }
        posicionActual = cantidadCasillas - 1;
    }
    else {
        if (nuevaPosicion >= caminoActual->size()) {
            Vec2 posicionMeta = obtenerPosicionRandomEnMeta();
            rutaMovimiento.push_back(posicionMeta);
            fichasEnMeta[turnoAct][indiceficha] = true;
            fichasJugadores[turnoAct * 4 + indiceficha]->setScale(0.7f);

            metaEnProceso = true;
            this->scheduleOnce([this](float) {
                aplicarBonusMeta();
                }, 0.3f, "aplicar_bonus_meta");

            return;
        }
        else {
            for (int i = 1; i <= cantidadCasillas; i++) {
                int posCheck = posicionActual + i;
                if (posCheck >= caminoActual->size()) break;

                Vec2 posicionCheck = (*caminoActual)[posCheck];
                if (hayBloqueoEnPosicion(posicionCheck, turnoAct)) {
                    return;
                }
            }

            for (int i = 1; i <= cantidadCasillas && (posicionActual + i) < caminoActual->size(); i++) {
                rutaMovimiento.push_back((*caminoActual)[posicionActual + i]);
            }

            if (!rutaMovimiento.empty() && !esCasillaSegura(rutaMovimiento.back())) {
                if (hayCapturaPosible(rutaMovimiento.back(), turnoAct)) {
                    capturaEnProceso = true;
                    this->scheduleOnce([this, rutaMovimiento](float) {
                        capturarFicha(rutaMovimiento.back(), turnoAct);
                        }, 0.3f, "realizar_captura");
                }
            }
            posicionActual += cantidadCasillas;
        }
    }

    if (!rutaMovimiento.empty()) {
        Vector<FiniteTimeAction*> acciones;

        for (size_t i = 0; i < rutaMovimiento.size(); i++) {
            auto movimiento = MoveTo::create(0.3f, rutaMovimiento[i]);
            auto salto = JumpBy::create(0.3f, Vec2::ZERO, 20, 1);
            auto movimientoCompleto = Spawn::create(movimiento, salto, nullptr);
            acciones.pushBack(movimientoCompleto);

            if (i < rutaMovimiento.size() - 1) {
                acciones.pushBack(DelayTime::create(0.1f));
            }
        }

        auto finMovimiento = CallFunc::create([=]() {
            fichaEnMovimiento = false;
            fichasJugadores[turnoAct * 4 + indiceficha]->setScale(0.9f);

            if (!capturaEnProceso && !metaEnProceso) {
                if (valordado1 == 0 && valordado2 == 0) {
                    this->scheduleOnce([this](float) {
                        cambiarTurno();
                        }, 0.5f, "cambiar_turno");
                }
                else {
                    if (!JUGADOR_HUMANO[turnoAct]) {
                        this->scheduleOnce([this](float) {
                            seleccionarMovimientoIA();
                            }, 0.5f, "siguiente_movimiento_ia");
                    }
                    this->removeChildByName("opciones_dados");
                }
            }
            this->removeChildByName("opciones_dados");
            });

        acciones.pushBack(finMovimiento);
        auto secuencia = Sequence::create(acciones);
        fichasJugadores[turnoAct * 4 + indiceficha]->runAction(secuencia);
        posiFichas[turnoAct][indiceficha] = posicionActual;

        if (!esCasillaSegura(rutaMovimiento.back()) &&
            hayCapturaPosible(rutaMovimiento.back(), turnoAct)) {
            capturaEnProceso = true;
            this->scheduleOnce([this, rutaMovimiento](float) {
                capturarFicha(rutaMovimiento.back(), turnoAct);
                }, 0.3f, "realizar_captura");
        }
    }
}

void TableroScene::ajustarFichasEnSalida(const Vec2& posicion) {
    vector<Sprite*> fichasEnPosicion;
    vector<int> coloresFichas;

    for (int j = 0; j < numJugadores; j++) {
        for (int k = 0; k < 4; k++) {
            if (!fichasBase[j * 4 + k] && !fichasEnMeta[j][k]) {
                Vec2 posFicha = fichasJugadores[j * 4 + k]->getPosition();
                if (sonPosicionesCercanas(posFicha, posicion)) {
                    fichasEnPosicion.push_back(fichasJugadores[j * 4 + k]);
                    coloresFichas.push_back(j);
                }
            }
        }
    }

    for (auto* ficha : fichasEnPosicion) {
        ficha->setScale(0.9f);
        ficha->setPosition(posicion);
    }

    if (fichasEnPosicion.size() >= 2) {
        bool mismoColor = true;
        for (size_t i = 1; i < coloresFichas.size(); i++) {
            if (coloresFichas[i] != coloresFichas[0]) {
                mismoColor = false;
                break;
            }
        }

         bool enCasillaSegura = esCasillaSegura(posicion);

         if ((mismoColor && !enCasillaSegura) || enCasillaSegura) {
             float offsetX = 10;
             float offsetY = 10;

             for (size_t i = 0; i < fichasEnPosicion.size(); i++) {
                 fichasEnPosicion[i]->setScale(0.7f);

                 if (i == 0) {
                     fichasEnPosicion[i]->setPosition(posicion + Vec2(-offsetX, -offsetY));
                 }
                 else {
                     fichasEnPosicion[i]->setPosition(posicion + Vec2(offsetX, offsetY));
                 }
             }
         } else {
            for (auto* ficha : fichasEnPosicion) {
                ficha->setScale(0.8f);
            }
        }
    }
}

void TableroScene::mostrarVentanaVictoria(int colorGanador) {
    if (ventanaGanador != nullptr) {
        ventanaGanador->removeAllChildren();
        ventanaGanador->removeFromParent();
        ventanaGanador = nullptr;
    }

    ventanaGanador = Layer::create();
    if (!ventanaGanador) return;

    auto fondoOscuro = LayerColor::create(Color4B(0, 0, 0, 128));
    if (fondoOscuro) {
        ventanaGanador->addChild(fondoOscuro);
    }

    auto fondoVentana = Sprite::create("ventanaWinner.png");
    if (!fondoVentana) return;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    fondoVentana->setPosition(visibleSize / 2);
    ventanaGanador->addChild(fondoVentana);

    // Obtener la fecha actual
    time_t now = time(0);
    tm* ltm = localtime(&now);
    string fecha = StringUtils::format("%d-%02d-%02d %02d:%02d",
        1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday,
        ltm->tm_hour, ltm->tm_min);

    // Crear y posicionar el label de "¡GANADOR!"
    auto labelVictoria = Label::createWithTTF("¡GANADOR!", "fonts/arial.ttf", 50);
    if (labelVictoria) {
        labelVictoria->setPosition(Vec2(fondoVentana->getContentSize().width / 2,
            fondoVentana->getContentSize().height * 0.8f));
        labelVictoria->setColor(Color3B::BLACK);
        fondoVentana->addChild(labelVictoria);
    }

    // Crear y posicionar la ficha ganadora
    string nombreFicha;
    switch (colorGanador) {
    case 0: nombreFicha = "fichaRoja.png"; break;
    case 1: nombreFicha = "fichaAzul.png"; break;
    case 2: nombreFicha = "fichaAmarilla.png"; break;
    case 3: nombreFicha = "fichaVerde.png"; break;
    }

    auto fichaGanadora = Sprite::create(nombreFicha);
    if (fichaGanadora) {
        fichaGanadora->setPosition(Vec2(fondoVentana->getContentSize().width / 2,
            fondoVentana->getContentSize().height * 0.6f));
        fichaGanadora->setScale(1.5f); // Hacer la ficha un poco más grande
        fondoVentana->addChild(fichaGanadora);
    }

    // Crear y posicionar el label de fecha
    auto labelFecha = Label::createWithTTF(fecha, "fonts/arial.ttf", 24);
    if (labelFecha) {
        labelFecha->setPosition(Vec2(fondoVentana->getContentSize().width / 2,
            fondoVentana->getContentSize().height * 0.4f));
        labelFecha->setColor(Color3B::BLACK);
        fondoVentana->addChild(labelFecha);
    }

    // Crear y posicionar el botón de cerrar
    auto botonMenu = MenuItemImage::create(
        "cruzCerrar.png", "cruzCerrar.png",
        CC_CALLBACK_1(TableroScene::volverAlMenu, this));

    if (botonMenu) {
        botonMenu->setPosition(Vec2(fondoVentana->getContentSize().width / 2,
            fondoVentana->getContentSize().height * 0.2f));

        auto menu = Menu::create(botonMenu, nullptr);
        menu->setPosition(Vec2::ZERO);
        fondoVentana->addChild(menu);
    }

    if (this->getChildByName("ventana_ganador")) {
        this->removeChildByName("ventana_ganador");
    }
    ventanaGanador->setName("ventana_ganador");
    this->addChild(ventanaGanador, 1000);

    guardarHistorialPartida(colorGanador, fecha);
}

Vec2 TableroScene::obtenerPosicionRandomEnMeta() {
    float minX = 570;
    float maxX = 835;
    float minY = 840;
    float maxY = 1100;

    int intentos = 0;

    while (intentos < 20) {
        float randomX = minX + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxX - minX);
        float randomY = minY + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxY - minY);

        Vec2 posicionPropuesta(randomX, randomY);

        if (posicionValidaEnMeta(posicionPropuesta)) {
            return posicionPropuesta;
        }

        intentos++;
    }
    return Vec2((minX + maxX) / 2, (minY + maxY) / 2);
}

bool TableroScene::posicionValidaEnMeta(const Vec2& pos) {
    for (int j = 0; j < numJugadores; j++) {
        for (int k = 0; k < 4; k++) {
            if (fichasEnMeta[j][k]) {
                Vec2 posFichaExistente = fichasJugadores[j * 4 + k]->getPosition();
                float distancia = pos.distance(posFichaExistente);
                if (distancia < DISTANCIA_MINIMA_META) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool TableroScene::hayBloqueoEnPosicion(const Vec2& posicion, int colorJugador) {
    int fichasEnPosicion = 0;
    std::vector<int> coloresEnPosicion;

    for (int j = 0; j < numJugadores; j++) {
        for (int k = 0; k < 4; k++) {
            if (!fichasBase[j * 4 + k] && !fichasEnMeta[j][k]) {
                Vec2 posFicha = fichasJugadores[j * 4 + k]->getPosition();
                if (sonPosicionesCercanas(posFicha, posicion)) {
                    fichasEnPosicion++;
                    coloresEnPosicion.push_back(j);
                }
            }
        }
    }

    if (fichasEnPosicion == 2) {
        if (esCasillaSegura(posicion)) {
            return coloresEnPosicion[0] == coloresEnPosicion[1] ||
                coloresEnPosicion[0] != coloresEnPosicion[1];
        }
        else {
            return coloresEnPosicion[0] == coloresEnPosicion[1] &&
                coloresEnPosicion[0] != colorJugador;
        }
    }

    return false;
}

bool TableroScene::hayCapturaPosible(const Vec2& posicion, int colorJugador) {
    if (esCasillaSegura(posicion)) {
        return false;
    }

    for (int j = 0; j < numJugadores; j++) {
        if (j != colorJugador) {
            for (int k = 0; k < 4; k++) {
                if (!fichasBase[j * 4 + k]) {
                    Vec2 posFicha = fichasJugadores[j * 4 + k]->getPosition();
                    if (sonPosicionesCercanas(posFicha, posicion)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool TableroScene::esCasillaSegura(const Vec2& posicion) {
    if (esCasillaSalidaSegura(posicion)) {
        return true;
    }
    for (const auto& casillaSegura : CASILLAS_SEGURAS) {
        if (sonPosicionesCercanas(posicion, casillaSegura)) {
            return true;
        }
    }
    return false;
}

bool TableroScene::hayFichaEnSalida(int colorJugador) {
    Vec2 posicionSalida;
    switch (colorJugador) {
    case 0: posicionSalida = CAMINO_ROJO[0]; break;
    case 1: posicionSalida = CAMINO_AZUL[0]; break;
    case 2: posicionSalida = CAMINO_AMARILLO[0]; break;
    case 3: posicionSalida = CAMINO_VERDE[0]; break;
    default: return false;
    }

    for (int i = 0; i < 4; i++) {
        if (!fichasBase[colorJugador * 4 + i] && !fichasEnMeta[colorJugador][i]) {
            if (sonPosicionesCercanas(fichasJugadores[colorJugador * 4 + i]->getPosition(), posicionSalida)) {
                return true;
            }
        }
    }
    return false;
}

bool TableroScene::esCasillaSalidaSegura(const Vec2& posicion) const {
    for (const auto& posSalida : POSICIONES_SALIDA) {
        if (sonPosicionesCercanas(posicion, posSalida)) {
            return true;
        }
    }
    return false;
}

bool TableroScene::esCasillaSalida(const Vec2& posicion) {
    for (const auto& posSalida : POSICIONES_SALIDA) {
        if (sonPosicionesCercanas(posicion, posSalida)) {
            return true;
        }
    }
    return false;
}

void TableroScene::ajustarVisualizacionFichas(const Vec2& posicion) {
    vector<Sprite*> fichasEnPosicion;
    vector<int> coloresFichas;

    for (int j = 0; j < numJugadores; j++) {
        for (int k = 0; k < 4; k++) {
            if (!fichasBase[j * 4 + k]) {
                Vec2 posFicha = fichasJugadores[j * 4 + k]->getPosition();
                if (sonPosicionesCercanas(posFicha, posicion)) {
                    fichasEnPosicion.push_back(fichasJugadores[j * 4 + k]);
                    coloresFichas.push_back(j);
                }
            }
        }
    }

    if (fichasEnPosicion.size() == 1) {
        fichasEnPosicion[0]->setScale(0.9f);
        return;
    }

    if (fichasEnPosicion.size() >=2) {
        bool mismoColor = true;
        for (size_t i = 1; i < coloresFichas.size(); i++) {
            if (coloresFichas[i] != coloresFichas[0]) {
                mismoColor = false;
                break;
            }
        }

        bool enCasillaSegura = esCasillaSegura(posicion);

        if ((mismoColor && !enCasillaSegura) || enCasillaSegura) {
            float offsetX = 10;
            float offsetY = 10;

            for (size_t i = 0; i < fichasEnPosicion.size(); i++) {
                fichasEnPosicion[i]->setScale(0.7f);
                if (i == 0) {
                    fichasEnPosicion[i]->setPosition(posicion + Vec2(-offsetX, -offsetY));
                }
                else {
                    fichasEnPosicion[i]->setPosition(posicion + Vec2(offsetX, offsetY));
                }
            }
        }
        else {
            for (auto* ficha : fichasEnPosicion) {
                ficha->setScale(0.9f);
            }
        }
    }
    this->scheduleOnce([this, fichasEnPosicion](float) {
        for (auto* ficha : fichasEnPosicion) {
            if (!fichaEnBase(ficha) && !fichaEnMeta(ficha)) {
                ficha->setScale(0.9f);
            }
        }
        }, 0.5f, "restaurar_tamaño");
}

void TableroScene::capturarFicha(const Vec2& posicion, int colorJugador) {
    capturaEnProceso = true;
    bool capturaRealizada = false;
    int indiceFichaCaptora = -1;

    for (int i = 0; i < 4; i++) {
        if (!fichasBase[colorJugador * 4 + i] && !fichasEnMeta[colorJugador][i]) {
            Vec2 posFichaCaptora = fichasJugadores[colorJugador * 4 + i]->getPosition();
            if (sonPosicionesCercanas(posFichaCaptora, posicion)) {
                indiceFichaCaptora = i;
                break;
            }
        }
    }

    for (int j = 0; j < numJugadores; j++) {
        if (j != colorJugador) {
            for (int k = 0; k < 4; k++) {
                if (!fichasBase[j * 4 + k] && !fichasEnMeta[j][k]) {
                    Vec2 posFicha = fichasJugadores[j * 4 + k]->getPosition();
                    if (sonPosicionesCercanas(posFicha, posicion)) {
                        moverFichaABase(j, k);
                        capturaRealizada = true;
                    }
                }
            }
        }
    }
    if (capturaRealizada && indiceFichaCaptora != -1) {
        this->scheduleOnce([this, colorJugador, indiceFichaCaptora](float) {
            valordado1 = 0; 
            valordado2 = 0;
            moverFicha(indiceFichaCaptora, 0);
            }, 0.5f, "bono_captura");
    }

    capturaEnProceso = false;
}

void TableroScene::aplicarBonusMeta() {
    metaEnProceso = false;
    if (valordado1 == 0 && valordado2 == 0) {
        cambiarTurno();
    }
}

void TableroScene::moverFichaABase(int jugador, int indiceficha) {
    vector<vector<Vec2>> posicionesCasas = { ROJOS, AZULES, AMARILLOS, VERDES };
    Vec2 posicionBase = posicionesCasas[jugador][indiceficha];

    auto movimiento = MoveTo::create(0.5f, posicionBase);
    fichasJugadores[jugador * 4 + indiceficha]->runAction(movimiento);

    fichasBase[jugador * 4 + indiceficha] = true;
    posiFichas[jugador][indiceficha] = -1;
    fichasJugadores[jugador * 4 + indiceficha]->setScale(1.0f);
}

void TableroScene::avanzarCasillasBonus(int indiceficha) {
    moverFicha(indiceficha, 20);
}

void TableroScene::ejecutarMovimientoPorCasillas(cocos2d::Sprite* ficha, const std::vector<cocos2d::Vec2>& posiciones) {
    if (posiciones.empty()) {
        fichaEnMovimiento = false;
        return;
    }

    Vector<FiniteTimeAction*> acciones;

    for (size_t i = 0; i < posiciones.size(); i++) {
        auto movimiento = MoveTo::create(0.3f, posiciones[i]);
        auto salto = JumpBy::create(0.3f, Vec2::ZERO, 20, 1);
        auto movimientoCompleto = Spawn::create(movimiento, salto, nullptr);
        acciones.pushBack(movimientoCompleto);

        if (i < posiciones.size() - 1) {
            acciones.pushBack(DelayTime::create(0.1f));
        }
    }

    auto finMovimiento = CallFunc::create([this, ficha]() {
        fichaEnMovimiento = false;
        ficha->setScale(0.9f);

        bool hayFichasFuera = false;
        for (int j = 0; j < numJugadores; j++) {
            for (int k = 0; k < 4; k++) {
                if (!fichasBase[j * 4 + k] && !fichasEnMeta[j][k]) {
                    fichasJugadores[j * 4 + k]->setScale(0.9f);
                }
            }
        }
        if (!capturaEnProceso && !metaEnProceso && valordado1 == 0 && valordado2 == 0) {
            this->scheduleOnce([this](float) {
                cambiarTurno();
                }, 0.5f, "cambiar_turno");
        }

        if (!hayFichasFuera || (valordado1 == 0 && valordado2 == 0)) {
            this->scheduleOnce([this](float) {
                cambiarTurno();
                }, 0.5f, "cambiar_turno");
        }
        else if (!JUGADOR_HUMANO[turnoAct]) {
            this->scheduleOnce([this](float) {
                seleccionarMovimientoIA();
                }, 0.5f, "continuar_movimiento_ia");
        }
        });
    acciones.pushBack(finMovimiento);
    auto secuencia = Sequence::create(acciones);
    ficha->runAction(secuencia);
}

void TableroScene::cambiarTurno() {
    if (fichaEnMovimiento) return;

    for (int i = 0; i < numJugadores; i++) {
        for (int j = 0; j < 4; j++) {
            if (!fichasBase[i * 4 + j] && !fichasEnMeta[i][j]) {
                fichasJugadores[i * 4 + j]->setScale(0.9f);
            }
        }
    }
    do {
        turnoAct = (turnoAct + 1) % numJugadores;
    } while (turnoAct >= numJugadores);

    actualizarIndicadorTurno();
    valordado1 = 0;
    valordado2 = 0;

    if (turnoAct < numJugadores && !JUGADOR_HUMANO[turnoAct]) {
        this->scheduleOnce([this](float) {
            ejecutarTurnoIA();
            }, 2.0f, "inicio_turno_ia");
    }
}

void TableroScene::ejecutarTurnoIA() {
    if (!dadosGirando && !fichaEnMovimiento && !PAUSA) {
        lanzarDados(nullptr);
        this->scheduleOnce([this](float) {
            seleccionarMovimientoIA();
            }, 1.0f, "procesar_turno_ia");
    }
}

void TableroScene::procesarTurnoIA() {
    if (verificarSalidaFicha(valordado1, valordado2)) {
        int fichasEnSalida = contarFichasEnSalida();
        if (fichasEnSalida < 2 && tieneFichasEnBase()) {
            moverFichaASalida();
            return;
        }
    }
    seleccionarMovimientoIA();
}

int TableroScene::contarFichasEnSalida() const {
    int count = 0;
    Vec2 posicionSalida = POSICIONES_SALIDA[turnoAct];

    for (int i = 0; i < 4; i++) {
        if (!fichasBase[turnoAct * 4 + i] && !fichasEnMeta[turnoAct][i]) {
            Vec2 posFicha = fichasJugadores[turnoAct * 4 + i]->getPosition();
            if (sonPosicionesCercanas(posFicha, posicionSalida)) {
                count++;
            }
        }
    }
    return count;
}

bool TableroScene::tieneFichasEnBase() const {
    for (int i = 0; i < 4; i++) {
        if (fichasBase[turnoAct * 4 + i]) {
            return true;
        }
    }
    return false;
}

void TableroScene::seleccionarMovimientoIA() {
    if (dadosGirando || fichaEnMovimiento || PAUSA || JUGADOR_HUMANO[turnoAct]) {
        return;
    }

    if(valordado1 == 0 && valordado2 == 0) {
        cambiarTurno();
        return;
    }

    if(!hayMovimientosDisponibles()) {
        if (valordado1 == 0 && valordado2 == 0) {
            cambiarTurno();
        }
        return;
    }

    if ((valordado1 == 6 || valordado2 == 6) && !hayFichaEnSalida(turnoAct) && tieneFichasEnBase()) {
        moverFichaASalida();
        return;
    }

    bool hayFichasFuera = false;
    for (int i = 0; i < 4; i++) {
        if (!fichasBase[turnoAct * 4 + i] && !fichasEnMeta[turnoAct][i]) {
            hayFichasFuera = true;
            break;
        }
    }

    if (hayFichasFuera) {
        vector<pair<int, int>> movimientosDado1;
        vector<pair<int, int>> movimientosDado2;

        if (valordado1 > 0) {
            for (int i = 0; i < 4; i++) {
                if (!fichasBase[turnoAct * 4 + i] && !fichasEnMeta[turnoAct][i]) {
                    const vector<Vec2>* caminoActual;
                    switch (turnoAct) {
                    case 0: caminoActual = &CAMINO_ROJO; break;
                    case 1: caminoActual = &CAMINO_AZUL; break;
                    case 2: caminoActual = &CAMINO_AMARILLO; break;
                    case 3: caminoActual = &CAMINO_VERDE; break;
                    default: continue;
                    }

                    int nuevaPos = posiFichas[turnoAct][i] + valordado1;
                    if (nuevaPos >= caminoActual->size() || esMovimientoValido(i, valordado1)) {
                        movimientosDado1.push_back({ i, valordado1 });
                    }
                }
            }
        }

        if (valordado2 > 0) {
            for (int i = 0; i < 4; i++) {
                if (!fichasBase[turnoAct * 4 + i] && !fichasEnMeta[turnoAct][i]) {
                    const vector<Vec2>* caminoActual;
                    switch (turnoAct) {
                    case 0: caminoActual = &CAMINO_ROJO; break;
                    case 1: caminoActual = &CAMINO_AZUL; break;
                    case 2: caminoActual = &CAMINO_AMARILLO; break;
                    case 3: caminoActual = &CAMINO_VERDE; break;
                    default: continue;
                    }

                    int nuevaPos = posiFichas[turnoAct][i] + valordado2;
                    if (nuevaPos >= caminoActual->size() || esMovimientoValido(i, valordado2)) {
                        movimientosDado2.push_back({ i, valordado2 });
                    }
                }
            }
        }

        if (movimientosDado1.empty() && movimientosDado2.empty()) {
            valordado1 = 0;
            valordado2 = 0;
            cambiarTurno();
            return;
        }

        if (!movimientosDado1.empty()) {
            auto mov = movimientosDado1[rand() % movimientosDado1.size()];
            moverFicha(mov.first, mov.second);

            this->scheduleOnce([this, movimientosDado2](float) {
                if (!fichaEnMovimiento && !capturaEnProceso && !metaEnProceso &&
                    !movimientosDado2.empty() && valordado2 > 0) {
                    auto mov = movimientosDado2[rand() % movimientosDado2.size()];
                    moverFicha(mov.first, mov.second);
                }
                }, 0.5f, "segundo_movimiento_ia");
        }
        else if (!movimientosDado2.empty()) {
            auto mov = movimientosDado2[rand() % movimientosDado2.size()];
            moverFicha(mov.first, mov.second);
        }
    }
    else {
        valordado1 = 0;
        valordado2 = 0;
        cambiarTurno();
    }
}

bool TableroScene::hayMovimientosDisponibles() {
    if (verificarSalidaFicha(valordado1, valordado2) && !hayFichaEnSalida(turnoAct)) {
        for (int i = 0; i < 4; i++) {
            if (fichasBase[turnoAct * 4 + i]) {
                return true;
            }
        }
    }
    if (valordado1 > 0) {
        for (int i = 0; i < 4; i++) {
            if (!fichasBase[turnoAct * 4 + i] && !fichasEnMeta[turnoAct][i]) {
                if (esMovimientoValido(i, valordado1)) {
                    return true;
                }
            }
        }
    }

    if (valordado2 > 0) {
        for (int i = 0; i < 4; i++) {
            if (!fichasBase[turnoAct * 4 + i] && !fichasEnMeta[turnoAct][i]) {
                if (esMovimientoValido(i, valordado2)) {
                    return true;
                }
            }
        }
    }

    return false;
}

void TableroScene::actualizarTexturasDados() {
    string texturaDado1 = "dado" + to_string(valordado1) + ".png";
    string texturaDado2 = "dado" + to_string(valordado2) + ".png";

    dadop->setTexture(Director::getInstance()->getTextureCache()->addImage(texturaDado1));
    dados->setTexture(Director::getInstance()->getTextureCache()->addImage(texturaDado2));
}

void TableroScene::mostrarMenuPausa(Ref* sender) {
    PAUSA = true;
    capaPausa = Layer::create();
    auto fondoOscuro = LayerColor::create(Color4B(0, 0, 0, 128));
    capaPausa->addChild(fondoOscuro);

    auto ventanaPausa = Sprite::create("ventanaPausa.png");
    ventanaPausa->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2));
    capaPausa->addChild(ventanaPausa);

    auto botonReanudar = MenuItemImage::create("botonReanudar.png", "botonReanudar.png",
        CC_CALLBACK_1(TableroScene::reanudarJuego, this));
    botonReanudar->setPosition(Vec2(0, 250));

    auto botonSalir = MenuItemImage::create("botonSalir.png", "botonSalir.png",
        CC_CALLBACK_1(TableroScene::volverAlMenu, this));
    botonSalir->setPosition(Vec2(0, -250));

    auto menuPausa = Menu::create(botonReanudar, botonSalir, nullptr);
    menuPausa->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2));
    capaPausa->addChild(menuPausa);
    this->addChild(capaPausa, 10);
}

void TableroScene::inicializarDados() {
    dadosGirando = false;
    veces = 0;

    dadop = Sprite::create("dado12.png");
    dados = Sprite::create("dado12.png");

    dadop->setPosition(Vec2(560 + Director::getInstance()->getVisibleOrigin().x,//
        1938 + Director::getInstance()->getVisibleOrigin().y));
    dados->setPosition(Vec2(840 + Director::getInstance()->getVisibleOrigin().x,
        1938 + Director::getInstance()->getVisibleOrigin().y));
    this->addChild(dadop, 1);
    this->addChild(dados, 1);

    //lanzar
    auto botonLanzar = MenuItemImage::create(
        "botonLanzar.png", "botonLanzar.png", CC_CALLBACK_1(TableroScene::lanzarDados, this));
    botonLanzar->setPosition(Vec2(700 + Director::getInstance()->getVisibleOrigin().x,
        1734 + Director::getInstance()->getVisibleOrigin().y));

    auto menuDados = Menu::create(botonLanzar, nullptr);
    menuDados->setPosition(Vec2::ZERO);
    this->addChild(menuDados, 1);
}

void TableroScene::lanzarDados(Ref* sender) {
    if (juegoTerminado || dadosGirando || fichaEnMovimiento || PAUSA) return;
    if (!dadosGirando && !fichaEnMovimiento && !PAUSA) {

        AudioEngine::play2d("dadosSonido.mp3", false);
        dadosGirando = true;
        veces = 0;
        valordado1 = 6;
        valordado2 = 6;
        this->schedule(CC_SCHEDULE_SELECTOR(TableroScene::animarDados), 0.1f);

        auto rotarDado1 = RotateBy::create(0.6f, 360);
        auto saltarDado1 = JumpBy::create(0.6f, Vec2(0, 0), 36, 1);
        auto rotarDado2 = RotateBy::create(0.6f, 360);
        auto saltarDado2 = JumpBy::create(0.6f, Vec2(0, 0), 36, 1);

        auto animacionDado1 = Spawn::create(rotarDado1, saltarDado1, nullptr);
        auto animacionDado2 = Spawn::create(rotarDado2, saltarDado2, nullptr);
        dadop->runAction(animacionDado1);
        dados->runAction(animacionDado2);
        this->scheduleOnce(CC_SCHEDULE_SELECTOR(TableroScene::finGirarDados), 0.6f);
    }
}

bool TableroScene::esMovimientoValido(int indiceficha, int cantidadCasillas) {
    if (fichasBase[turnoAct * 4 + indiceficha] ||
        fichasEnMeta[turnoAct][indiceficha]) {
        return false;
    }

    int posicionActual = posiFichas[turnoAct][indiceficha];
    const std::vector<Vec2>* caminoActual;

    switch (turnoAct) {
    case 0: caminoActual = &CAMINO_ROJO; break;
    case 1: caminoActual = &CAMINO_AZUL; break;
    case 2: caminoActual = &CAMINO_AMARILLO; break;
    case 3: caminoActual = &CAMINO_VERDE; break;
    default: return false;
    }

    // bloqueo
    for (int i = 1; i <= cantidadCasillas; i++) {
        int posCheck = posicionActual + i;
        if (posCheck >= caminoActual->size()) return true; // Pllegar meta

        Vec2 posicionCheck = (*caminoActual)[posCheck];
        if (!esCasillaSegura(posicionCheck)) {
            if (hayBloqueoEnPosicion(posicionCheck, turnoAct)) {
                return false;
            }
        }
    }

    return true;
}

void TableroScene::detenerJuego() {
    this->unscheduleAllCallbacks();

    for (auto& ficha : fichasJugadores) {
        if (ficha) {
            ficha->stopAllActions();
        }
    }

    fichaEnMovimiento = false;
    dadosGirando = false;
    metaEnProceso = false;
    capturaEnProceso = false;
}

void TableroScene::animarDados(float) {
    if (veces < 22) {
        int dado1, dado2;
        do {
            dado1 = random(6, 12);
            dado2 = random(6, 12);
        } while (dado1 == dado2);

        string texturaDado1 = "dado" + to_string(valordado1) + ".png";
        string texturaDado2 = "dado" + to_string(valordado2) + ".png";

        dadop->setTexture(Director::getInstance()->getTextureCache()->addImage(texturaDado1));
        dados->setTexture(Director::getInstance()->getTextureCache()->addImage(texturaDado2));
        veces++;
    }
}

void TableroScene::finGirarDados(float) {
    this->unschedule(CC_SCHEDULE_SELECTOR(TableroScene::animarDados));
    dadosGirando = false;

    do {
        valordado1 = random(6, 12);
        valordado2 = random(6, 12);
    } while (valordado1 == valordado2 || (valordado1 == 6 && valordado2 == 6));

    actualizarTexturasDados();

    if (!hayMovimientosDisponibles()) {
        this->scheduleOnce([this](float) {
            valordado1 = 0;
            valordado2 = 0;
            cambiarTurno();
            }, 1.0f, "cambiar_turno_sin_movimientos");
    }
    else if (!JUGADOR_HUMANO[turnoAct]) {
        this->scheduleOnce([this](float) {
            seleccionarMovimientoIA();
        }, 0.5f, "ejecutar_turno_ia");
    }
}

void TableroScene::moverFichaASalida() {
    if (hayFichaEnSalida(turnoAct)) {
        return;
    }

    bool fichaMovida = false;
    for (int i = 0; i < 4; i++) {
        if (fichasBase[turnoAct * 4 + i]) {
            Vec2 posicionDestino;
            switch (turnoAct) {
            case 0: posicionDestino = CAMINO_ROJO[0]; break;
            case 1: posicionDestino = CAMINO_AZUL[0]; break;
            case 2: posicionDestino = CAMINO_AMARILLO[0]; break;
            case 3: posicionDestino = CAMINO_VERDE[0]; break;
            }

            fichaEnMovimiento = true;
            fichasJugadores[turnoAct * 4 + i]->setScale(0.9f);

            auto secuencia = Sequence::create(
                MoveTo::create(0.4f, posicionDestino),
                CallFunc::create([this, posicionDestino]() {
                    fichaEnMovimiento = false;
                    ajustarVisualizacionFichas(posicionDestino);
                    valordado1 = 0;
                    valordado2 = 0;
                    this->scheduleOnce([this](float) {
                        cambiarTurno();
                        }, 0.2f, "cambiar_turno_salida");
                    }),
                nullptr
            );

            fichasJugadores[turnoAct * 4 + i]->runAction(secuencia);
            fichasBase[turnoAct * 4 + i] = false;
            posiFichas[turnoAct][i] = 0;
            break;
        }
    }
}

bool TableroScene::verificarSalidaFicha(int valordado1, int valordado2) {
    if (valordado1 <= 0 || valordado2 <= 0) {
        return false;
    }
    return (valordado1 == 6 || valordado2 == 6);
}

void TableroScene::verificarVictoria() {
    if (juegoTerminado) return;

    for (int j = 0; j < numJugadores; j++) {
        int fichasEnMetaCount = 0;
        for (int k = 0; k < 4; k++) {
            if (fichasEnMeta[j][k]) {
                fichasEnMetaCount++;
            }
        }

        if (fichasEnMetaCount == 4) {
            juegoTerminado = true;
            detenerJuego();

            Director::getInstance()->getScheduler()->schedule(
                [this, j](float) {
                    mostrarVentanaVictoria(j);
                },
                this,
                0,
                0,
                0,
                false,
                "mostrar_victoria"
            );
            return;
        }
    }
}

void TableroScene::manejarEntradaMeta(int jugador, int indiceficha) {
    fichasEnMeta[jugador][indiceficha] = true;
    fichasJugadores[jugador * 4 + indiceficha]->setScale(0.7f);
    verificarVictoria();

    if (!todasFichasEnMeta(jugador)) {
        aplicarBonusMeta();
    }
}

bool TableroScene::todasFichasEnMeta(int jugador) {
    int count = 0;
    for (int i = 0; i < 4; i++) {
        if (fichasEnMeta[jugador][i]) {
            count++;
        }
    }
    return count == 4;
}

bool TableroScene::fichaEnBase(Sprite* ficha) {
    for (size_t i = 0; i < fichasJugadores.size(); i++) {
        if (fichasJugadores[i] == ficha) {
            return fichasBase[i];
        }
    }
    return false;
}

bool TableroScene::fichaEnMeta(Sprite* ficha) {
    for (size_t i = 0; i < fichasJugadores.size(); i++) {
        if (fichasJugadores[i] == ficha) {
            int jugador = i / 4;
            int indice = i % 4;
            return fichasEnMeta[jugador][indice];
        }
    }
    return false;
}

void TableroScene::guardarHistorialPartida(int colorGanador, const string& fecha) {
    string colorTexto;
    switch (colorGanador) {
    case 0: colorTexto = "ROJO"; break;
    case 1: colorTexto = "AZUL"; break;
    case 2: colorTexto = "AMARILLO"; break;
    case 3: colorTexto = "VERDE"; break;
    }

    // Crear el registro con formato claro
    string registro = fecha + " - " +
        std::to_string(numJugadores) + " jugadores - " +
        "Ganador: " + colorTexto + "\n";

    string filePath = FileUtils::getInstance()->getWritablePath() + "historial_partidas.txt";

    FILE* file = fopen(FileUtils::getInstance()->getSuitableFOpen(filePath).c_str(), "a");
    if (file) {
        fputs(registro.c_str(), file);
        fclose(file);
        CCLOG("Registro guardado exitosamente en: %s", filePath.c_str());
        CCLOG("Contenido del registro: %s", registro.c_str());
    }
    else {
        CCLOG("Error al abrir el archivo para escribir en: %s", filePath.c_str());
    }
}
void TableroScene::reanudarJuego(Ref* sender) {
    if (capaPausa) {
        capaPausa->removeFromParent();
        capaPausa = nullptr;
        PAUSA = false;
    }
}

void TableroScene::volverAlMenu(Ref* sender) {
    this->unscheduleAllCallbacks();
    this->stopAllActions();

    for (auto& ficha : fichasJugadores) {
        if (ficha) {
            ficha->stopAllActions();
        }
    }

    if (ventanaGanador) {
        ventanaGanador->removeFromParent();
        ventanaGanador = nullptr;
    }

    if (capaPausa) {
        capaPausa->removeFromParent();
        capaPausa = nullptr;
    }

    auto scene = JuegoInicio::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}