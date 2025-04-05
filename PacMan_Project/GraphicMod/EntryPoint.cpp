#include <iostream>
#include "Entity.h"

class GraphicsComponent;

#ifdef __linux__

# define DYNAMIC_LIBRARY_API __attribute__((visibility("default")))

#else // Windows

# ifdef DYNAMIC_LIBRARY_EXPORTS
#   define DYNAMIC_LIBRARY_API __declspec(dllexport)
# else 
#   define DYNAMIC_LIBRARY_API __declspec(dllimport)
# endif // DYNAMIC_LIBRARY_EXPORTS

#endif // __linux__

typedef void (*ComponentScript)(const Entity* parent);

extern "C" DYNAMIC_LIBRARY_API ComponentScript loadMod()
{
  std::cout << "Loading mod" << std::endl;

  static ComponentScript componentScript = [](const Entity* parent)
    {
      //std::weak_ptr<GraphicsComponent> graphicsWeak = parent->GetComponent<GraphicsComponent>();
      //std::shared_ptr<GraphicsComponent> graphics = graphicsWeak.lock();
      //if (graphics) 
      //{
      //  sf::Texture newTexture;
      //  if (!newTexture.loadFromFile("../../../../Assets/ghost35.png"))
      //  {
      //    std::cerr << "Failed to load pacMan.png" << std::endl;
      //    return;
      //  }
      //  graphics->SetTexture(newTexture);
      //}
    };

  return componentScript;
}