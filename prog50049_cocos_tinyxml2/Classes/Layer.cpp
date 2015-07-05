#include "Layer.h"

namespace TileMap
{
   TileLayer::TileLayer()
   {
   }

   TileLayer::~TileLayer()
   {
      removeAllChildren();
   }

   bool TileLayer::init()
   {
      Node::init();

      return true;
   }
}