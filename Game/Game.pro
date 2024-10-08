TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp
INCLUDEPATH += "C:/SFML-2.6.1/include"
LIBS += -L"C:/SFML-2.6.1/lib"
CONFIG(debug, debug|release){
    LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-system-d -lsfml-window-d
} else {
    LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
}

HEADERS += \
    Enemy.h \
    Game.h \
    MainCharacter.h \
    Powerupcoin.h \
    Scenery.h \
    constants.h \
    gameobject.h \
    obstacle.h
