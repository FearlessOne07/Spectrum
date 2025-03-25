#include "base/Event.hpp"
#include "base/System.hpp"
#include <memory>

class BulletSystem : public Base::System
{
private:
  void Start() override;
  void Stop() override;

private: // Hanlders
  void EntityCollisionHandler(const std::shared_ptr<Base::Event> &);

public:
  virtual void Update(float dt, Base::EntityManager *entityManager) override;
};
