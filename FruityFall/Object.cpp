#include "Object.h"

Object::Object(float posX, float posY)
	: m_x(posX), m_y(posY)
{
}

void Object::setTexture(const std::string& filePath)
{
	m_texture = LoadTexture(filePath.c_str());
}

void Object::onRender() const
{
	DrawTextureEx(m_texture, { m_x, m_y }, m_rotation, m_scale, m_color);
}