#pragma once


#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;		//����·��
	unsigned char* m_LocalBuffer;	//�����ı��ش洢
	int m_Width, m_Height, m_BPP;	//ʵ��������ÿ����λ
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	inline int GetWidth() { return m_Width; }
	inline int GetHeight() { return m_Height; }
};