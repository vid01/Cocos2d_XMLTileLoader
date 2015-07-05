#ifndef _MAP_LOADER_
#define _MAP_LOADER_

#include "cocos2d.h"
#include "tinyxml2.h"
#include "Layer.h"
#include <fstream>

USING_NS_CC;

namespace TileMap
{
   // image element contents
   struct Image
   {
      std::string imagePath;
      int imageWidth;
      int imageHeight;
   };

   // tileset element contents
   struct TileInfo
   {
      int tileWidth;
      int tileHeight;
      int firstgid;

      Image image;
   };


   class MapLoader : public Node
   {
      std::vector<TileLayer*> tileLayers;
      std::vector<TileInfo*> tileset;

   public:

      MapLoader();
      ~MapLoader();

      virtual bool init();

      void loadXML();
      void printTiles(Size& _size, Point& _origin);
      void hideLayers();

      CREATE_FUNC(MapLoader);
   };
}

#endif // _MAP_LOADER_