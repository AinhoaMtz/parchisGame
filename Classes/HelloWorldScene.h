#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "musicaFondo.h"

using namespace std;

class HelloWorld : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HelloWorld);

private:
    void openJuegoInicio(Ref* sender);
    void openHistorial(Ref* sender);
    void openConfiguracion(Ref* sender);
    void openPerfil(Ref* sender);
    void cerrarVentana(Ref* sender);
    void cerrarHistorial(Ref* sender);
    void cargarContenidoHistorial(Node* ventanaHistorial);
    Layer* capaHistorial;
    bool isMusicPlaying;
    Sprite* musicButton;
    Layer* capaTutorial;

    int paginaActualTutorial;
    const int TOTAL_PAGINAS_TUTORIAL = 5;
    Node* contenedorImagenTutorial;
    void openTutorial(Ref* sender);
    void cerrarTutorial(Ref* sender);
    Sprite* imagenTutorialActual;
    void realizarTransicionImagen (const string& nombreNuevaImagen);
    void siguientePaginaTutorial(Ref* sender);
    void anteriorPaginaTutorial(Ref* sender);
    void actualizarPaginaTutorial();

    cocos2d::ui::Slider* volumeSlider;
    void toggleMusic(Ref* sender);
    void onVolumeChanged(Ref* sender, cocos2d::ui::Slider::EventType type);

    cocos2d::Layer* capaPopup;
};

#endif // __HELLOWORLD_SCENE_H__