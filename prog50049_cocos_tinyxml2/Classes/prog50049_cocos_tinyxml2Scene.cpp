#include "prog50049_cocos_tinyxml2Scene.h"
//#include "tinyxml2.h"
//#include <fstream>

USING_NS_CC;

Scene* prog50049_cocos_tinyxml2::createScene()
{
    auto scene = Scene::create();
    auto layer = prog50049_cocos_tinyxml2::create();
    scene->addChild(layer);
    return scene;
}

bool prog50049_cocos_tinyxml2::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    Size size = Director::getInstance()->getWinSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    mLoader = TileMap::MapLoader::create();
    mLoader->loadXML();
    mLoader->printTiles(size, origin);

    addChild(mLoader);

    EventListenerMouse* mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(prog50049_cocos_tinyxml2::onMouseDown, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    return true;
}

void prog50049_cocos_tinyxml2::onMouseDown(Event* event)
{
   mLoader->hideLayers();
}