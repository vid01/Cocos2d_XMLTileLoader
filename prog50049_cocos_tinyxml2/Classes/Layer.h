#ifndef _TILE_LAYER_
#define _TILE_LAYER_

#include "cocos2d.h"

USING_NS_CC;

namespace TileMap
{
   class TileLayer : public Node
   {
      friend class MapLoader;
   private:
      std::vector<Sprite*> sprites;
      std::vector<int> tileIDs;        // tile gid

      int width;
      int height;   

      bool isActive;

      TileLayer();
      ~TileLayer();

      virtual bool init();

      CREATE_FUNC(TileLayer);
   };
}

#endif