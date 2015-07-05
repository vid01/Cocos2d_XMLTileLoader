#ifndef __PROG50049_COCOS_TINYXML2_SCENE_H__
#define __PROG50049_COCOS_TINYXML2_SCENE_H__

#include "cocos2d.h"
#include "MapLoader.h"

class prog50049_cocos_tinyxml2 : public cocos2d::Layer
{
   TileMap::MapLoader* mLoader;

public:

    static cocos2d::Scene* createScene();
    virtual bool init();  
    
    void onMouseDown(Event* event);

    // implement the "static create()" method manually
    CREATE_FUNC(prog50049_cocos_tinyxml2);
};

#endif // __PROG50049_COCOS_TINYXML2_SCENE_H__
