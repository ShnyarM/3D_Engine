#pragma once

//Default vertex shader which applies rotation and translation
//Templated on input vertex type of effect, so it can be used with any effect
class DefaultVertexShader
{
public:
	//Set vertex class as input class
	class Output
	{
	public:
		Output() = default;
		Output(const Vec4& pos)
			:
			pos(pos)
		{
		}
		Output(const Vec4& pos, const Output& v)
			:
			pos(pos)
		{
		}
		Output& operator=(const Output& rhs)
		{
			pos = rhs.pos;
			return *this;
		}
		Output& operator+=(const Output& rhs)
		{
			pos += rhs.pos;
			return *this;
		}
		Output& operator-=(const Output& rhs)
		{
			pos -= rhs.pos;
			return *this;
		}
		Output operator+(const Output& rhs) const
		{
			return Output(*this) += rhs;
		}
		Output operator-(const Output& rhs) const
		{
			return Output(*this) -= rhs;
		}
		Output& operator*=(const float rhs)
		{
			pos *= rhs;
			return *this;
		}
		Output operator*(const float rhs) const
		{
			return Output(*this) *= rhs;
		}
		Output& operator/=(const float rhs)
		{
			pos /= rhs;
			return *this;
		}
		Output operator/(const float rhs) const
		{
			return Output(*this) /= rhs;
		}
		Vec4 pos;
	};
public:
	void BindWorldTransformation(const Mat4& transform)
	{
		worldTransform = transform;
	}
	void BindProjection(const Mat4& projection)
	{
		worldProjection = projection;
	}
	const Mat4 GetProj() const
	{
		return worldProjection;
	}
	template<class V>
	Output operator()(const V& input)
	{
		return Vertex{ Vec4(input.pos) * worldTransform * worldProjection };
	}

private:
	Mat4 worldTransform = Mat4::Identity();
	Mat4 worldProjection = Mat4::Identity();
};