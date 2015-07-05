#include "MapLoader.h"

#define MAP_FILE_NAME "TMX/myScene.tmx"
//#define MAP_FILE_NAME "TMX/Map.tmx"

namespace TileMap
{
   MapLoader::MapLoader()
   {
   }

   MapLoader::~MapLoader()
   {
      for (int i = 0; i < tileset.size(); i++)
      {
         delete tileset[i];
      }

      for (int i = 0; i < tileLayers.size(); i++)
      {
         delete tileLayers[i];
      }
   }

   bool MapLoader::init()
   {
      Node::init();

      return true;
   }

   void MapLoader::loadXML()
   {
      // load file
      // xmlPath gets path to DEBUG folder
      std::string xmlPath = FileUtils::getInstance()->getWritablePath() + MAP_FILE_NAME;

      tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument();

      ssize_t nSize;
      const char* pXmlBuffer = (const char*)FileUtils::getInstance()->getFileData(xmlPath.c_str(), "rb", &nSize);

      if (NULL == pXmlBuffer)
      {
         return;
      }

      xmlDoc->Parse(pXmlBuffer);

      // LOAD UP THE DATA

      // root node
      tinyxml2::XMLElement* rootNode = (tinyxml2::XMLElement*)xmlDoc->FirstChildElement("map");

      // read all tilesets with images
      for (tinyxml2::XMLElement* child = (tinyxml2::XMLElement*)rootNode->FirstChildElement("tileset"); child != nullptr; child = (tinyxml2::XMLElement*)child->NextSiblingElement("tileset"))
      {
         TileInfo* tempTile = new TileInfo();
         tempTile->firstgid = child->IntAttribute("firstgid");
         tempTile->tileWidth = child->IntAttribute("tilewidth");
         tempTile->tileHeight = child->IntAttribute("tileheight");
       
         tinyxml2::XMLElement* imageNode = (tinyxml2::XMLElement*)child->FirstChildElement("image");

         tempTile->image.imageWidth = imageNode->IntAttribute("width");
         tempTile->image.imageHeight = imageNode->IntAttribute("height");
         tempTile->image.imagePath = imageNode->Attribute("source");

         tileset.push_back(tempTile);
      }


      int tileIndex = 0;
      // read all layers
      for (tinyxml2::XMLElement* child = (tinyxml2::XMLElement*)rootNode->FirstChildElement("layer"); child != nullptr; child = (tinyxml2::XMLElement*)child->NextSiblingElement("layer"))
      {
         TileLayer* temp = new TileLayer();
         temp->width = child->IntAttribute("width");
         temp->height = child->IntAttribute("height");

         tinyxml2::XMLElement* dataNode = (tinyxml2::XMLElement*)child->FirstChildElement("data");

         for (tinyxml2::XMLElement* dataChild = (tinyxml2::XMLElement*)dataNode->FirstChildElement("tile"); dataChild != nullptr; dataChild = (tinyxml2::XMLElement*)dataChild->NextSiblingElement("tile"))
         {
            temp->tileIDs.push_back(dataChild->IntAttribute("gid"));
            tileIndex++;
         }

         tileLayers.push_back(temp);
      }

      // save tile layers
      for (int i = 0; i < tileLayers.size(); i++)
      {
         tileLayers[i]->retain();
      }

      delete xmlDoc;
   }

   void MapLoader::printTiles(Size& _size, Point& _origin)
   {
      Point offset = Point(0, 0);

      for (int j = 0; j < tileLayers.size(); j++)
      {
         for (int i = 0; i < tileLayers[j]->tileIDs.size(); i++)
         {
            // transform gid to x / y tile offset for a tilemap image
            int iteration = 0;                        // y offset
            int result = tileLayers[j]->tileIDs[i];   // x offset

            // determine the tileset based on gid
            int tilsetIdx = 0;

            for (; tilsetIdx < tileset.size()-1; tilsetIdx++)
            {
               if (((result >= tileset[tilsetIdx]->firstgid)
                  && (result < tileset[tilsetIdx + 1]->firstgid)) 
                  || (result == 0))
               {
                  break;
               }
            }

            int imageSetWidth = tileset[tilsetIdx]->image.imageWidth / tileset[tilsetIdx]->tileWidth;

            // y offset counter in tilemap image
            //int iter = tileset[tilsetIdx]->firstgid;
            //for (int k = 0; iter != result; k++)
            //{
            //   if (k % imageSetWidth == 0)
            //   {
            //      ++iteration;
            //   }

            //   ++iteration;
            //   
            //} // y offset found

            while (result > imageSetWidth)
            {
               // first tile map check
               if (result == tileset[tilsetIdx]->firstgid)
               {
                  result = 1;
                  break;
               }
               result -= imageSetWidth;
               ++iteration; // y offset found
            }

            --result; // x offset found

            if (i == 0 & result >= 0)
            {
               Sprite* sprite = Sprite::create("TMX/" + tileset[tilsetIdx]->image.imagePath, Rect(tileset[tilsetIdx]->tileWidth * result, tileset[tilsetIdx]->tileHeight * iteration, tileset[tilsetIdx]->tileWidth, tileset[tilsetIdx]->tileHeight));
               sprite->setPosition(_origin.x + tileset[tilsetIdx]->tileWidth / 2 + offset.x, _origin.y + _size.height - tileset[tilsetIdx]->tileHeight / 2 + offset.y);
               tileLayers[j]->sprites.push_back(sprite);
            }
            else
            {
               // add offset if tile row is completed...
               if (offset.x < tileLayers[j]->width * tileset[tilsetIdx]->tileWidth - tileset[tilsetIdx]->tileWidth)
               {
                  offset.x += tileset[tilsetIdx]->tileWidth;
               }
               else
               {
                  offset.x = 0;
                  offset.y -= tileset[tilsetIdx]->tileHeight;
               }

               // skip drawing if tile is empty
               if (result < 0)
               {
                  continue;
               }

               Sprite* sprite = Sprite::create("TMX/" + tileset[tilsetIdx]->image.imagePath, Rect(tileset[tilsetIdx]->tileWidth * result, tileset[tilsetIdx]->tileHeight * iteration, tileset[tilsetIdx]->tileWidth, tileset[tilsetIdx]->tileHeight));
               sprite->setPosition(_origin.x + tileset[tilsetIdx]->tileWidth / 2 + offset.x, _origin.y + _size.height - tileset[tilsetIdx]->tileHeight / 2 + offset.y);
               tileLayers[j]->sprites.push_back(sprite);
            }
         }

         // add sprites as children to layer
         for (int i = 0; i < tileLayers[j]->sprites.size(); i++)
         {
            tileLayers[j]->addChild(tileLayers[j]->sprites[i]);
         }

         addChild(tileLayers[j], j);
         tileLayers[j]->isActive = true;
         
         // resert tile offset for new layer
         offset.x = 0;
         offset.y = 0;
      }
   }

   void MapLoader::hideLayers()
   {
      for (int i = tileLayers.size()-1; i >= 0; i--)
      {
         if (tileLayers[i]->isActive)
         {
            removeChild(tileLayers[i]);
            tileLayers[i]->isActive = false;
            break;
         }
      }
   }
}