#ifndef MYVECTOR3 
#define MYVECTOR3


/// <summary>
///Ethan Rennick 
/// 

#include <iostream>
#include <SFML/Graphics.hpp>


class MyVector3
{
public:
	double x;
	double y;
	double z;

	MyVector3();
	~MyVector3();
	std::string toString();


	/// <summary>
	/// ALL COMPLETED
	/// </summary>
	MyVector3(double t_x, double t_y, double t_z); // done with pete
	MyVector3(sf::Vector3f t_sfVector); // done by pete
	MyVector3(sf::Vector3i t_sfVector); //done with pete
	MyVector3(sf::Vector2i t_sfVector); //complete
	MyVector3(sf::Vector2u t_sfVector); //all good
	MyVector3(sf::Vector2f t_sfVector); //complete
	
	/// <summary>
	/// ALL DONE
	/// </summary>
	MyVector3 operator +(const MyVector3 t_right) const; //done with pete
	MyVector3 operator -(const MyVector3 t_right) const; //done
	MyVector3 operator *(const double t_scalar) const; //helped by michael + done
	MyVector3 operator /(const double t_divisor) const; //done
	MyVector3 operator +=(const MyVector3 t_right); //helped by michael + done
	MyVector3 operator -=(const MyVector3 t_right); //done

	/// <summary>
	/// COMPLETED
	/// </summary>
	bool operator == (const MyVector3 t_right) const; // done
	bool operator != (const MyVector3 t_right) const; //done

	/// <summary>
	/// DONE
	/// </summary>
	// negative of a vector 
	MyVector3 operator -();  // this changes the vector //done
	//new methods for bouncing off sides of table in billiards //done
	void reverseX(); // this negates the x element of the vector //done
	void reverseY();  // this negates the y element of the vector //done

	double length()const; //done
	double lengthSquared()const; //done with pete
	double dot(const MyVector3 t_other) const; //done
	MyVector3 crossProduct(const MyVector3 t_other)const; //ATTEMPTED BUT INCOMPLETE
	double angleBetween(const MyVector3 t_other)const;  // answer in degrees //done
	MyVector3 unit() const;  // vector doesn't change
	void normalise(); // vector changes and will have length 1 
	MyVector3 projection(const MyVector3 t_onto)const;
	// projection onto the other vector so answer will parallel to other vector.
	MyVector3 rejection(const MyVector3 t_onto)const;

	
	//incomplete >.>
	operator sf::Vector2f() { return sf::Vector2f{static_cast<float>(x),static_cast<float>(y) }; }; // {2.4,-2.6,3.0} ->  {2.4~,-2.6~}
	operator sf::Vector2i() { return sf::Vector2i{static_cast<int>(x), 4 }; };// {2.4,-2.6,3.0} ->  {2,-3}
	operator sf::Vector2u();// {2.4,-2.6,3.0} ->  {2,3}  needs more space than an inline
	operator sf::Vector3i() { return sf::Vector3i{ }; };// {2.4,-2.6,3.0} ->  {2,-3,3}
	operator sf::Vector3f() { return sf::Vector3f{ }; };// {2.4,-2.6,3.0} ->  {2.4~,-2.6~, 3.0}

};



#endif // !MYVECTOR3