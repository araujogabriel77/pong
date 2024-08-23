#pragma once

#include "Components.hpp"
#include<memory>
#include<string>

class Entity
{
	friend class EntityManager;

	bool m_active = true;
	size_t m_id = 0;
	std::string m_tag = "default";

	// constructor and destructor
	Entity(const size_t id, const std::string& tag);

public:

	// component pointers
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CRectShape> rectShape;
	std::shared_ptr<sf::Sprite> sprite;
	std::shared_ptr<CShape> circleShape;
	std::shared_ptr<CCollision> cCollision;
	std::shared_ptr<CBoundingBox> cBoundingBox;
	std::shared_ptr<CInput> cInput;
	std::shared_ptr<CScore> cScore;
	std::shared_ptr<CLifespan> cLifeSpan;

	// private member access functions
	bool isActive() const;
	const std::string& tag() const;
	const size_t id() const;
	void destroy();

  /* delta = [abs(x1 - x2), abs(y1 - y2)]
  * ox = (w1/2) + (w2/2) - delta.x
  * oy = (h1/2) + (h2/2) - delta.y
  * overlap = [ox, oy]
  */
  Vec2 overlap(const Entity& entity) const;
};

