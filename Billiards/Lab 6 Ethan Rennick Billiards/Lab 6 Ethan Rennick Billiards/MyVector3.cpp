  /// <summary>
/// Vector 3 class
/// Ethan Rennick
/// </summary>
#include "MyVector3.h"
#include <string.h>
#define PI           3.14159265358979323846

/// <summary>
/// Default constructor set all 3 values to zero
/// </summary>
MyVector3::MyVector3() :
	x{ 0.0 },
	y{ 0.0 },
	z{ 0.0 }
{
}

/// <summary>
/// default destructor
/// nothing to release because we only have
/// primitive type data
/// </summary>
MyVector3::~MyVector3()
{
}

/// <summary>
/// 
/// </summary>
/// <returns>the vector as a string surrounded by square brackets and separated by commas</returns>
std::string MyVector3::toString()
{
	const std::string output = "[" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "]";
	return output;	
}



MyVector3::MyVector3(double t_x, double t_y, double t_z)
{
	x = t_x;
	y = t_y;
	z = t_z;
}

/// <summary>
/// create a vector 3 from an sf::vector3f
/// </summary>
/// <param name="t_sfVector">input vector</param>
MyVector3::MyVector3(sf::Vector3f t_sfVector)
{
	x = static_cast<double>( t_sfVector.x);
	y = static_cast<double>(t_sfVector.y);
	z = static_cast<double>(t_sfVector.z);
}

MyVector3::MyVector3(sf::Vector3i t_sfVector)
{
	x = static_cast<double>(t_sfVector.x);
	y = static_cast<double>(t_sfVector.y);
	z = static_cast<double>(t_sfVector.z);
}

MyVector3::MyVector3(sf::Vector2i t_sfVector)
{
	x = static_cast<double>(t_sfVector.x);
	y = static_cast<double>(t_sfVector.y);
	z = 0.0; // because it's 2d, it has no z
}

MyVector3::MyVector3(sf::Vector2u t_sfVector)
{
	x = static_cast<double>(t_sfVector.x);
	y = static_cast<double>(t_sfVector.y);
	z = 0.0; // because it's 2d, it has no z
}

MyVector3::MyVector3(sf::Vector2f t_sfVector)
{
	x = static_cast<double>(t_sfVector.x);
	y = static_cast<double>(t_sfVector.y);
	z = 0.0;
}

MyVector3::operator sf::Vector2u()
{
	return sf::Vector2u{};
}

MyVector3 MyVector3::operator+(const MyVector3 t_right) const
{
	double newX = x + t_right.x;
	double newY = y + t_right.y;
	double newZ = z + t_right.z;

	return MyVector3(newX, newY, newZ);
}

MyVector3 MyVector3::operator-(const MyVector3 t_right) const
{
	double newX = x - t_right.x;
	double newY = y - t_right.y;
	double newZ = z - t_right.z;

	return MyVector3(newX, newY, newZ);
}

MyVector3 MyVector3::operator*(const double t_scalar) const
{
	double newX = x * t_scalar;
	double newY = y * t_scalar;
	double newZ = z * t_scalar;
	return MyVector3(newX, newY, newZ);
}

//division
MyVector3 MyVector3::operator/(const double t_divisor) const
{
	double newX = x / t_divisor;
	double newY = y / t_divisor;
	double newZ = z / t_divisor;
	return MyVector3(newX, newY, newZ);
}
MyVector3 MyVector3::operator+=(const MyVector3 t_right)
{
	x += t_right.x;
	y += t_right.y;
	z += t_right.z;

	return MyVector3();
}
MyVector3 MyVector3::operator-=(const MyVector3 t_right)
{
	x -= t_right.x;
	y -= t_right.y;
	z -= t_right.z;


	return MyVector3();
}
double MyVector3::length() const
{
	double length = 0;
	length = sqrt(x*x + y*y + z*z);
	return length;
}
//length squared
double MyVector3::lengthSquared() const
{

	double lengthSquared = x * x + y * y + z * z;
	return lengthSquared;
}

//dot product
double MyVector3::dot(const MyVector3 t_other) const
{
	double dotProduct = x * t_other.x + y * t_other.y + z * t_other.z;
	return dotProduct; //unsure if correct
}

//cross Product
MyVector3 MyVector3::crossProduct(const MyVector3 t_other) const
{
	//super difficult to type, mistakes likely, very sorry
	double newX = y * t_other.z - z * t_other.y;
	double newY = z * t_other.x - x * t_other.z;
	double newZ = x * t_other.y - y * t_other.x;

	//should be okay? calculations seem fine
	return MyVector3(newX  ,  newY  ,  newZ);
}

//angle between ( two vectors ) //helped by class
double MyVector3::angleBetween(const MyVector3 t_other) const
{
	double numerator = dot(t_other);
	double denominator = length() * t_other.length();

	double angleInRadians;
	if (denominator != 0)
	{
		double result = numerator / denominator;
		
		if (result > 1)
		{
			result = 1;
		}
		if (result < -1)
		{
			result = -1;
		}
		angleInRadians = std::acos(result);
		double angleInDegreees = angleInRadians * 180.0 / PI;
		return angleInDegreees;
		//helped by patrick
	}

	return 0.0; // so if the whole thing is zero, its nothing !!!
}

//unit vector //helped by old code / google
MyVector3 MyVector3::unit() const
{
	MyVector3 unitVector{};
	double vectorLength = length();
	if (vectorLength != 0.0)
	{
		double newX = x / vectorLength;
		double newY = y / vectorLength;
		double newZ = z / vectorLength;
		unitVector = { newX,newY,newZ };
	}

	//should be fine, test OK

	return unitVector;
}

//normalisation
//used maths notes 
void MyVector3::normalise()
{
	double lengthOne = length(); //diff length variables
	if (lengthOne != 0.0)
	{
		x /= lengthOne;
		y /= lengthOne;
		z /= lengthOne;
	}
}

//projection
//struggled with this in lab 3 - so used pete's lab 3 doc for help
MyVector3 MyVector3::projection(const MyVector3 t_onto) const
{
	double lengthSquared = t_onto.lengthSquared();
	double dotProd = dot(t_onto);
	double nullScale{ 1.0 }; //in case null vectors come along?

	if (lengthSquared != 0)
	{
		nullScale = dotProd / lengthSquared;
	}


	return t_onto * nullScale;
}


//Rejection
MyVector3 MyVector3::rejection(const MyVector3 t_onto) const
{
	//had help from class
	return this ->operator-(projection(t_onto));
}



//helped by richard with the bool operators
bool MyVector3::operator==(const MyVector3 t_right) const
{
	bool equal = false;

	if (x == t_right.x && y==t_right.y && z==t_right.z)
	{
		equal = true;
	}

	return equal;
}

bool MyVector3::operator!=(const MyVector3 t_right) const
{
	bool notEqual = false;

	if (x != t_right.x || y != t_right.y || z != t_right.z)
	{
		notEqual = true;
	}
	return notEqual;
}

MyVector3 MyVector3::operator-()
{
	return MyVector3(x * -1, y * -1, z * -1);
}

void MyVector3::reverseX()
{
	x = x * -1;
}

void MyVector3::reverseY()
{
	y = y * -1;
}
