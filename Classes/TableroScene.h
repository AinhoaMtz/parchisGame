#ifndef __TABLERO_SCENE_H__
#define __TABLERO_SCENE_H__

#include "cocos2d.h"

using namespace std;
USING_NS_CC;

class TableroScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene(int numJugadores);
    static TableroScene* create(int numJugadores);
    virtual bool init();
    bool initWithPlayers(int numJugadores);
    
    
private:
    int contarFichasEnSalida()const;
    void inicializarTablero();
    void inicializarFichas();
    void mostrarMenuPausa(cocos2d::Ref* sender);
    void reanudarJuego(cocos2d::Ref* sender);
    void volverAlMenu(cocos2d::Ref* sender);
    void inicializarDados();
    void lanzarDados(cocos2d::Ref* sender);
    void animarDados(float);
    void finGirarDados(float);
    void cambiarTurno();
    void resetearDados();
    void verificarVictoria();
    void ajustarFichasEnSalida(const Vec2& posicion);
    void mostrarVentanaVictoria(int colorGanador);
    void actualizarTexturasDados();
    void moverFichaASalida();
    void ejecutarTurnoIA();
    void seleccionarMovimientoIA();
    void aplicarBonusMeta();
    void guardarHistorialPartida(int colorGanador, const string& fecha);
    void procesarTurnoIA();
    void capturarFicha(const Vec2& posicion, int colorJugador);
    void moverFichaABase(int jugador, int indiceficha);
    void avanzarCasillasBonus(int indiceficha);
    void actualizarIndicadorTurno();
    void ajustarVisualizacionFichas(const Vec2& posicion);
    void mostrarOpcionesDados(int indiceficha);
    void manejarEntradaMeta(int jugador, int indiceficha);
    void moverFicha(int indiceficha, int cantidadCasillas);
    void ejecutarMovimientoPorCasillas(cocos2d::Sprite* ficha, const std::vector<cocos2d::Vec2>& posiciones);
  
    void detenerJuego();
    bool esCasillaSalidaSegura(const Vec2& posicion) const;
    bool esCasillaSegura(const Vec2& posicion);
    bool esCasillaSalida(const Vec2& posicion);
    bool tieneFichasEnBase()const;
    bool todasFichasEnMeta(int jugador);
    bool hayMovimientosDisponibles();

    bool ambosDadosUsados() const {
        return valordado1 == 0 && valordado2 == 0;
    }
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    bool verificarSalidaFicha(int valordado1, int valordado2);
    bool hayBloqueoEnPosicion(const Vec2& posicion, int colorJugador);
    bool hayCapturaPosible(const Vec2& posicion, int colorJugador);
    bool sonPosicionesCercanas(const Vec2& pos1, const Vec2& pos2, float tolerancia = 5.0f) const;
    bool posicionValidaEnMeta(const Vec2& pos);
    bool esMovimientoValido(int indiceficha, int cantidadCasillas);
    bool fichaEnBase(Sprite* ficha);
    bool fichaEnMeta(Sprite* ficha);
    bool hayFichaEnSalida(int colorJugador);
    bool dadosGirando, permitirNuevoTiro;
    bool juegoTerminado;
    Layer* ventanaGanador;
    bool capturaEnProceso = false;
    bool metaEnProceso = false;
    int veces, valordado1, valordado2, turnoAct, numJugadores;
    bool PAUSA, fichaEnMovimiento = false;
    const float DISTANCIA_MINIMA_META = 40.0f;
    vector<bool> JUGADOR_HUMANO;

    cocos2d::Layer* capaPausa;
    cocos2d::Sprite* indicadorTurno;
    cocos2d::Sprite* dadop;
    cocos2d::Sprite* dados;

    vector<cocos2d::Sprite*> fichasJugadores;
    vector<bool> fichasBase;
    vector<vector<int>> posiFichas;

    const Rect ZONA_VICTORIA = Rect(600, 900, 840 - 580, 1100 - 845);
    vector<std::vector<bool>> fichasEnMeta;
    
    Vec2 obtenerPosicionRandomEnMeta();

    const vector<Vec2> CAMINO_ROJO = {
        Vec2(348.00, 848.99),//roja
Vec2(404.00, 848.99),
Vec2(466.00, 844.99),
Vec2(526.00, 860.99),
Vec2(580.00, 816.99),
Vec2(565.60, 762.99),
Vec2(565.00, 699.99),
Vec2(565.00, 648.99),//estrella1
Vec2(565.00, 592.99),
Vec2(565.00, 536.99),
Vec2(565.00, 480.99),
Vec2(565.00, 424.00),
Vec2(700.99, 424.00),//estrtella2
Vec2(835.00, 424.00),
Vec2(835.00, 480.99),
Vec2(835.00, 536.99),
Vec2(835.00, 592.99),
Vec2(835.00, 648.99),//verde, 
Vec2(835.00, 704.99),
Vec2(835.00, 762.99),
Vec2(835.00, 820.99),/////
Vec2(874.00, 860.99),
Vec2(930.00, 848.99),
Vec2(988.00, 848.99),
Vec2(1048.00, 848.99),//estrella3
Vec2(1106.99, 848.99),/////
Vec2(1165.99, 848.99),////////////
Vec2(1220.00, 848.99), ////////////
Vec2(1280.99, 848.99),/////////////
Vec2(1280.99, 983.99),//estrella4
Vec2(1280.99, 1118.99),
Vec2(1220.00, 1118.99),
Vec2(1165.99, 1118.99),
Vec2(1106.99, 1118.99),
Vec2(1048.00, 1118.99),//amarilla,  
Vec2(988.00,1118.99),///////// 
Vec2(930.0, 1118.99),
Vec2(874.00, 1118.99),
Vec2(835.99, 1179.99),
Vec2(835.99, 1230.99),
Vec2(835.99, 1286.99),
Vec2(835.99, 1346.99),//estrella5,////////////
Vec2(835.99, 1410.99),
Vec2(835.99, 1470.99),
Vec2(835.99, 1525.99),
Vec2(835.99,1580.99),///////////
Vec2(700.00 ,1580.99),//estrella6,//////////
Vec2(565.00, 1580.99), //////////////////////
Vec2(565.00, 1525.99),
Vec2(565.00, 1470.99),
Vec2(565.00, 1410.99),
Vec2(565.00, 1346.99),//azul/////////////
Vec2(565.00, 1286.99),
Vec2(565.00, 1230.99),
Vec2(565.00, 1175.99),
Vec2(526.36, 1106.99),
Vec2(466.24, 1118.99),
Vec2(404.12, 1118.99),
Vec2(348.00, 1118.99),//estrella7
Vec2(291.00, 1118.99),
Vec2(232.00, 1118.99),
Vec2(172.00, 1118.99),
Vec2(116.00,1118.99),
Vec2(116.00, 983.99),//estrella8
Vec2(172.00, 983.99),
Vec2(232.00, 983.99),
Vec2(291.00, 983.99),
Vec2(348.00, 983.99),
Vec2(404.00, 983.99),
Vec2(466.00, 983.99),
Vec2(526.00, 983.99)
};

    const vector<Vec2> CAMINO_AZUL = {
        Vec2(565.00, 1346.99),//azul/////////////
Vec2(565.00, 1286.99),
Vec2(565.00, 1230.99),
Vec2(565.00, 1175.99),
Vec2(526.36, 1106.99),
Vec2(466.24, 1118.99),
Vec2(404.12, 1118.99),
Vec2(348.00, 1118.99),//estrella7
Vec2(291.00, 1118.99),
Vec2(232.00, 1118.99),
Vec2(172.00, 1118.99),
Vec2(116.00,1118.99),
Vec2(116.00, 983.99),//estrella8
Vec2(116.00, 848.99),
Vec2(172.00 ,848.99),
Vec2(232.00, 848.99),
Vec2(291.00, 848.99),
Vec2(348.00, 848.99),//roja
Vec2(404.00, 848.99),
Vec2(466.00, 844.99),
Vec2(526.00, 860.99),
Vec2(580.00, 816.99),
Vec2(565.60, 762.99),
Vec2(565.00, 699.99),
Vec2(565.00, 648.99),//estrella1
Vec2(565.00, 592.99),
Vec2(565.00, 536.99),
Vec2(565.00, 480.99),
Vec2(565.00, 424.00),
Vec2(700.99, 424.00),//estrtella2
Vec2(835.00, 424.00),
Vec2(835.00, 480.99),
Vec2(835.00, 536.99),
Vec2(835.00, 592.99),
Vec2(835.00, 648.99),//verde, 
Vec2(835.00, 704.99),
Vec2(835.00, 762.99),
Vec2(835.00, 820.99),/////
Vec2(874.00, 860.99),
Vec2(930.00, 848.99),
Vec2(988.00, 848.99),
Vec2(1048.00, 848.99),//estrella3
Vec2(1106.99, 848.99),/////
Vec2(1165.99, 848.99),////////////
Vec2(1220.00, 848.99), ////////////
Vec2(1280.99, 848.99),/////////////
Vec2(1280.99, 983.99),//estrella4
Vec2(1280.99, 1118.99),
Vec2(1220.00, 1118.99),
Vec2(1165.99, 1118.99),
Vec2(1106.99, 1118.99),
Vec2(1048.00, 1118.99),//amarilla,  
Vec2(988.00,1118.99),///////// 
Vec2(930.0, 1118.99),
Vec2(874.00, 1118.99),
Vec2(835.99, 1179.99),
Vec2(835.99, 1230.99),
Vec2(835.99, 1286.99),
Vec2(835.99, 1346.99),//estrella5,////////////
Vec2(835.99, 1410.99),
Vec2(835.99, 1470.99),
Vec2(835.99, 1525.99),
Vec2(835.99,1580.99),///////////
Vec2(700.00 ,1580.99),//estrella6
Vec2(700.00 ,1525.99),
Vec2(700.00 ,1470.99),
Vec2(700.00 ,1410.99),
Vec2(700.00 ,1346.99),
Vec2(700.00 ,1286.99),
Vec2(700.00 ,1230.99),
Vec2(700.00 ,1179.99)
    };

    const vector<Vec2> CAMINO_AMARILLO = {
        Vec2(1048.00, 1118.99),//amarilla,  
Vec2(988.00,1118.99), 
Vec2(930.0, 1118.99),
Vec2(874.00, 1118.99),
Vec2(835.99, 1179.99),
Vec2(835.99, 1230.99),
Vec2(835.99, 1286.99),
Vec2(835.99, 1346.99),//estrella5,////////////
Vec2(835.99, 1410.99),
Vec2(835.99, 1470.99),
Vec2(835.99, 1525.99),
Vec2(835.99,1580.99),///////////
Vec2(700.00 ,1580.99),//estrella6,//////////
Vec2(565.00, 1580.99), //////////////////////
Vec2(565.00, 1525.99),
Vec2(565.00, 1470.99),
Vec2(565.00, 1410.99),
Vec2(565.00, 1346.99),//azul/////////////
Vec2(565.00, 1286.99),
Vec2(565.00, 1230.99),
Vec2(565.00, 1175.99),
Vec2(526.36, 1106.99),
Vec2(466.24, 1118.99),
Vec2(404.12, 1118.99),
Vec2(348.00, 1118.99),//estrella7
Vec2(291.00, 1118.99),
Vec2(232.00, 1118.99),
Vec2(172.00, 1118.99),
Vec2(116.00,1118.99),
Vec2(116.00, 983.99),
Vec2(116.00, 848.99),
Vec2(172.00 ,848.99),
Vec2(232.00, 848.99),
Vec2(291.00, 848.99),
Vec2(348.00, 848.99),//roja
Vec2(404.00, 848.99),
Vec2(466.00, 844.99),
Vec2(526.00, 860.99),
Vec2(580.00, 816.99),
Vec2(565.60, 762.99),
Vec2(565.00, 699.99),
Vec2(565.00, 648.99),//estrella1
Vec2(565.00, 592.99),
Vec2(565.00, 536.99),
Vec2(565.00, 480.99),
Vec2(565.00, 424.00),
Vec2(700.99, 424.00),//estrtella2
Vec2(835.00, 424.00),
Vec2(835.00, 480.99),
Vec2(835.00, 536.99),
Vec2(835.00, 592.99),
Vec2(835.00, 648.99),//verde, 
Vec2(835.00, 704.99),
Vec2(835.00, 762.99),
Vec2(835.00, 820.99),/////
Vec2(874.00, 860.99),
Vec2(930.00, 848.99),
Vec2(988.00, 848.99),
Vec2(1048.00, 848.99),//estrella3
Vec2(1106.99, 848.99),/////
Vec2(1165.99, 848.99),////////////
Vec2(1220.00, 848.99), ////////////
Vec2(1280.99, 848.99),/////////////
Vec2(1280.99, 983.99),//estrella4
Vec2(1220.99, 983.99),
Vec2(1165.99, 983.99),
Vec2(1106.99, 983.99),
Vec2(1048.99, 983.99),
Vec2(988.99, 983.99),
Vec2(930.99, 983.99),
Vec2(874.99, 983.99)
    };

    const vector<Vec2> CAMINO_VERDE = {
        Vec2(835.00, 648.99),//verde, 
Vec2(835.00, 704.99),
Vec2(835.00, 762.99),
Vec2(835.00, 820.99),/////
Vec2(874.00, 860.99),
Vec2(930.00, 848.99),
Vec2(988.00, 848.99),
Vec2(1048.00, 848.99),//estrella3
Vec2(1106.99, 848.99),/////
Vec2(1165.99, 848.99),////////////
Vec2(1220.00, 848.99), ////////////
Vec2(1280.99, 848.99),/////////////
Vec2(1280.99, 983.99),//estrella4
Vec2(1280.99, 1118.99),
Vec2(1220.00, 1118.99),
Vec2(1165.99, 1118.99),
Vec2(1106.99, 1118.99),
Vec2(1048.00, 1118.99),//amarilla,  
Vec2(988.00,1118.99),///////// 
Vec2(930.0, 1118.99),
Vec2(874.00, 1118.99),
Vec2(835.99, 1179.99),
Vec2(835.99, 1230.99),
Vec2(835.99, 1286.99),
Vec2(835.99, 1346.99),//estrella5,////////////
Vec2(835.99, 1410.99),
Vec2(835.99, 1470.99),
Vec2(835.99, 1525.99),
Vec2(835.99,1580.99),///////////
Vec2(700.00 ,1580.99),//estrella6,//////////
Vec2(565.00, 1580.99), //////////////////////
Vec2(565.00, 1525.99),
Vec2(565.00, 1470.99),
Vec2(565.00, 1410.99),
Vec2(565.00, 1346.99),//azul/////////////
Vec2(565.00, 1286.99),
Vec2(565.00, 1230.99),
Vec2(565.00, 1175.99),
Vec2(526.36, 1106.99),
Vec2(466.24, 1118.99),
Vec2(404.12, 1118.99),
Vec2(348.00, 1118.99),//estrella7
Vec2(291.00, 1118.99),
Vec2(232.00, 1118.99),
Vec2(172.00, 1118.99),
Vec2(116.00,1118.99),
Vec2(116.00, 983.99),//estrella8
Vec2(116.00, 848.99),
Vec2(172.00 ,848.99),
Vec2(232.00, 848.99),
Vec2(291.00, 848.99),
Vec2(348.00, 848.99),//roja
Vec2(404.00, 848.99),
 Vec2(466.00, 844.99),
 Vec2(526.00, 860.99),
 Vec2(580.00, 816.99),
Vec2(565.60, 762.99),
Vec2(565.00, 699.99),
Vec2(565.00, 648.99),//estrella1
Vec2(565.00, 592.99),
Vec2(565.00, 536.99),
Vec2(565.00, 480.99),
Vec2(565.00, 424.00),
Vec2(700.99, 424.00),//estrella2
Vec2(700.99, 480.00),
Vec2(700.99, 536.00),
Vec2(700.99, 592.00),
Vec2(700.99, 648.00),
Vec2(700.99, 704.00),
Vec2(700.99, 762.00),
Vec2(700.99, 820.00)
        };


    const std::vector<cocos2d::Vec2> POSICIONES_SALIDA = {
        Vec2(348.00, 848.99),    // Rojo
        Vec2(565.00, 1346.99),   // Azul
        Vec2(1048.00, 1118.99),  // Amarillo
        Vec2(835.00, 648.99),  // Verde
    };
    const std::vector<Vec2> CASILLAS_SEGURAS = {
        Vec2(565.00, 648.99),  // estrella1
        Vec2(700.99, 424.00),  // estrella2
        Vec2(1048.00, 848.99), // estrella3
        Vec2(1280.99, 983.99), // estrella4
        Vec2(835.99, 1346.99), // estrella5
        Vec2(700.00, 1580.99), // estrella6
        Vec2(348.00, 1118.99), // estrella7
        Vec2(116.00, 983.99)   // estrella8
    };
    const std::vector<cocos2d::Vec2> ROJOS = {
        cocos2d::Vec2(252, 670), cocos2d::Vec2(252, 540),
        cocos2d::Vec2(377, 670), cocos2d::Vec2(377, 540)
    };
    const std::vector<cocos2d::Vec2> AZULES = {
        cocos2d::Vec2(245, 1460), cocos2d::Vec2(245, 1330),
        cocos2d::Vec2(372, 1460), cocos2d::Vec2(372, 1330)
    };
    const std::vector<cocos2d::Vec2> VERDES = {
        cocos2d::Vec2(1030, 670), cocos2d::Vec2(1030, 540),
        cocos2d::Vec2(1155, 670), cocos2d::Vec2(1155, 540)
    };
    const std::vector<cocos2d::Vec2> AMARILLOS = {
        cocos2d::Vec2(1033, 1460), cocos2d::Vec2(1033, 1330),
        cocos2d::Vec2(1162, 1460), cocos2d::Vec2(1162, 1330)
    };
};

#endif