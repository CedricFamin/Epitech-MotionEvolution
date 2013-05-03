#pragma once

#include <ModaCPP.h>
#include <map>
#include "Leg.h"

namespace ApiBlli
{
	enum
	{
		LEFT_FRONT_LEG = 0,
		LEFT_MIDDLE_LEG,
		LEFT_REAR_LEG,
		RIGHT_FRONT_LEG,
		RIGHT_MIDDLE_LEG,
		RIGHT_REAR_LEG
	};

	struct GyroValues
	{
		float LinearAccelerations[3];
		float Angles[3];
	};

	class Robot
	{
	public:
		Robot(ModaCPP::RobotPHX* robot, const xkode::lib::String& name);
		Robot(ModaCPP::RobotPHX* robot, const xkode::lib::String& name, bool oneLeg);
		~Robot(void);

		// Reset du robot
		void				Init();

		// Positionne un servo d'une patte a l'angle donne avec la vitesse angulaire
		void				SetAngle(int servo, float angle, float speed);

		// Recuperation de l'angle d'un servo sur une patte
		float				GetAngle(int servo);

		// Positionne un servo d'une patte a lindex donne
		void				SetIndex(int servo, int index, float speed);

		// Recuperation de l'index d'un servo sur une patte
		int					GetIndex(int servo);

		
		// Es-ce que le bout de la pattte touche le sol
		bool				IsLegTuchingGround();
		
		// Es-ce que la pattes est en colision avec un objet ou le sol ?
		// Different de IsTuchingGround(int leg) car la colision peut etre dangereuse pour les omposants
		bool				IsInColision();

		// Recuperation du coupe d'un servo
		float				GetTorque(int servo);
		
		// Recuperation du nom d'une patte
		const std::string	GetLegName();
		
		// Recuperation de la position du bout de la patte passe en parametre
		Position*			GetFingerPosition();
		
		// Recuperation de la position du servo de la patte passe en parametre
		Position*			GetPosition(int servo);

		// Recuperation de la position du robot
		Position*			GetPosition();
		
		// Es-ce que le chassis du robot touche le sol ?
		bool				IsTuchingGround();
		
		// Recuperation des valeurs du gyroscope
		GyroValues*			GetGyroValues();
		
		// mettre en pause le thread en milliseconde simulee
		void				Wait(int _ms);


	private:
		bool						_OneLeg;
		ModaCPP::RobotPHX*			_Robot;
		std::map<int, Leg*>			_Legs;
		ModaCPP::DeviceAccelGyro*	_Gyro;
		ModaCPP::DeviceContact*		_Platine;
		ModaCPP::Geom*				_PlatineBox;
	};
}
