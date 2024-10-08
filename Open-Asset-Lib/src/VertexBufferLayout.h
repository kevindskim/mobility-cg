#pragma once

#include <vector>

#include "Renderer.h"

//Layout별로, 데이터를 어떻게 읽어와야 하는지에 대한 정보를 가지고있는 구조체
struct VertexBufferElement
{
	unsigned int type; //각 데이터 타입이 무엇인지 (ex, vertex의 위치면 float)
	unsigned int count; //데이터가 몇 개인지
	unsigned char normalized; //데이터의 normalization이 필요한지

	static unsigned int GetSizeOfType(unsigned int type) //타입별로 적절한 메모리 사이즈를 반환하는 static 함수
	{
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(0);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements; //하나의 layout은 여러개의 element를 갖고 있음(ex, position, normal, color, etc...)
	unsigned int m_Stride; //vertex하나당 데이터가 얼마나 떨어져있는지 stride를 멤버변수로 갖고 있음

public:
	VertexBufferLayout()
		: m_Stride{ 0 }
	{}

	// 김형기 교수 코드: static_assert 에러 발생
	//template<typename T>
	//void Push(unsigned int count)
	//{
	//	static_assert(false);
	//}

	// chat GPT 김형기 교수 코드: static_assert 에러 발생하지 않음
	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(std::is_same<T, float>::value ||
			std::is_same<T, unsigned int>::value ||
			std::is_same<T, unsigned char>::value,
			"Unsupported type");
	}

	//template specializations
	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT); //vertex 하나당 float 데이터가 count개 추가될수록, count * size(GL_FLOAT)씩 stride가 커져야 함
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT); //위와 마찬가지
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement>& GetElement() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};