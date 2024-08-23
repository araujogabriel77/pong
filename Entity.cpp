#include "Entity.hpp"

Entity::Entity(const size_t id, const std::string& tag)
	: m_id(id)
	, m_tag(tag)
{}

bool Entity::isActive() const
{
	return m_active;
}
const std::string& Entity::tag() const
{
	return m_tag;
}
const size_t Entity::id() const {
	return m_id;
}
void Entity::destroy()
{
	m_active = false;
}

Vec2 Entity::overlap(const Entity& entity) const
{
  Vec2 delta = Vec2(
    abs(cTransform->pos.x - entity.cTransform->pos.x),
    abs(cTransform->pos.y - entity.cTransform->pos.y)
  );

  return Vec2(
    (entity.cBoundingBox->halfSize.x + cBoundingBox->halfSize.x) - delta.x,
    (entity.cBoundingBox->halfSize.y + cBoundingBox->halfSize.y) - delta.y
  );
}