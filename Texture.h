#pragma once


#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;		//纹理路径
	unsigned char* m_LocalBuffer;	//纹理的本地存储
	int m_Width, m_Height, m_BPP;	//实际纹理的每像素位
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	inline int GetWidth() { return m_Width; }
	inline int GetHeight() { return m_Height; }
};