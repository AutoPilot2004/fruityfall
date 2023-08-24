#include "Object.h"

std::unordered_map<std::string, Texture2D> Object::m_objectTextures;

Object::Object(float posX, float posY)
	: m_x(posX), m_y(posY) {}

void Object::setTexture(const std::string& filePath)
{
	m_texture = getFromObjectTextures(filePath);
}

void Object::onRender() const
{
	DrawTextureEx(m_texture, { m_x - (m_texture.width * m_scale / 2.0f), m_y - (m_texture.height * m_scale / 2.0f)}, m_rotation, m_scale, m_color);
}

const Texture2D& Object::getFromObjectTextures(const std::string& filePath)
{
	if (m_objectTextures.find(filePath) == m_objectTextures.end())
		m_objectTextures[filePath] = LoadTexture(filePath.c_str());

	return m_objectTextures.at(filePath);
}