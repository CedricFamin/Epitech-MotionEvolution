#pragma once

#include <ModaCPP.h>
#include "conio.h"
#include <stdlib.h> 
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <list>

namespace ApiBlli
{
	struct	Position
	{
		Position()
		{
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
		}

		Position(float _x, float _y, float _z)
		{
			this->x = _x;
			this->y = _y;
			this->z = _z;
		}

		Position(const Position& _position)
		{
			this->x = _position.x;
			this->y = _position.y;
			this->z = _position.z;
		}

		float	x;
		float	y;
		float	z;
	};

	class Leg
	{
	public:
		Leg(ModaCPP::RobotPHX *pPHX, const xkode::lib::String& name, bool left);
		~Leg(void);
		void	SetAngle(int servo, float angle, float speed);
		float	GetAngle(int servo);
		void	SetIndex(int servo, int index, float speed);
		int		GetIndex(int servo);
		Position*	GetPosition(int servo);
		Position*	GetFingerPosition();

		float	GetTorque(int servo);
		const xkode::lib::String&	GetName();
		bool	IsInColision();
		bool	IsTuchingGround();

		enum
		{
			FINGER = 0,
			ARM,
			SHOULDER
		};

	private:
		ModaCPP::RobotPHX*							_pPHX;
		std::map<int, ModaCPP::DeviceServoMotor*>	_Servos;
		std::map<int, ModaCPP::DeviceForceSensor*>	_Sensors;
		std::list<ModaCPP::DeviceContact*>			_ContactBox;
		std::map<int, ModaCPP::Geom*>				_ContactBoxServo;

		ModaCPP::DeviceContact*						_GroundBoxContact;
		ModaCPP::Geom*								_FingerBox;

		xkode::lib::String			  _Name;
		bool						  _Left;
	};
}
