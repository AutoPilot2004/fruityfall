#pragma once

#include <raylib.h>

#include <string>
#include <unordered_map>

class Object
{
public:
	Object() = default;
	Object(float posX, float posY);
	virtual ~Object() = default;

	void setTexture(const std::string& filePath);

	void onRender() const;

	//Setters
	inline void setX(float x) { m_x = x; }
	inline void setY(float y) { m_y = y; }
	inline void offsetX(float x) { m_x += x; }
	inline void offsetY(float y) { m_y += y; }

	inline void setRot(float rotation) { m_rotation = rotation; }
	inline void offsetRot(float rotation) { m_rotation += rotation; }

	inline void setScale(float scale) { m_scale = scale; }

	inline void setColor(Color color) { m_color = color; }

	//Getters
	inline float getX() const { return m_x; }
	inline float getY() const { return m_y; }

	inline float getWidth() const { return m_texture.width * m_scale; }
	inline float getHeight() const { return m_texture.height * m_scale; }

	inline float getRot() const { return m_rotation; }

	inline float getScale() const { return m_scale; }

	inline const Texture2D& getTex() const { return m_texture; }

protected:
	static const Texture2D& getFromObjectTextures(const std::string& filePath);

protected:
	float m_x = 0.0f, m_y = 0.0f;
	float m_rotation = 0.0f;
	float m_scale = 0.05f;

	Color m_color = { 255, 255, 255, 255 };

	Texture2D m_texture{};

	static std::unordered_map<std::string, Texture2D> m_objectTextures;

private:
};