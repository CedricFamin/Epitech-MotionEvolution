#include "Leg.h"

namespace ApiBlli
{
	Leg::Leg(ModaCPP::RobotPHX *pPHX, const xkode::lib::String& name, bool left)
	{
		this->_pPHX = pPHX;
		this->_Left = left;
		this->_Name = name;

		this->_Servos[this->FINGER] = pPHX->QueryDeviceServoMotor(name + "_doigt/a1/servo");
		this->_Servos[this->ARM] = pPHX->QueryDeviceServoMotor(name + "_bras/a1/servo");
		this->_Servos[this->SHOULDER] = pPHX->QueryDeviceServoMotor(name + "_epaule/a1/servo");

		this->_Sensors[this->FINGER] = pPHX->QueryDeviceForceSensor(name + "_doigt/a1/torque");
		this->_Sensors[this->ARM] = pPHX->QueryDeviceForceSensor(name + "_bras/a1/torque");
		this->_Sensors[this->SHOULDER] = pPHX->QueryDeviceForceSensor(name + "_epaule/a1/torque");

		this->_ContactBox.push_back(pPHX->QueryDeviceContact(name + "_doigt0/ctct"));
		this->_ContactBox.push_back(pPHX->QueryDeviceContact(name + "_doigt1/ctct"));
		this->_ContactBox.push_back(pPHX->QueryDeviceContact(name + "_doigt2/ctct"));
		this->_ContactBox.push_back(pPHX->QueryDeviceContact(name + "_doigt3/ctct"));

		this->_GroundBoxContact = pPHX->QueryDeviceContact(name + "_doigt4/ctct");

		this->_ContactBox.push_back(pPHX->QueryDeviceContact(name + "_bras0/ctct"));
		this->_ContactBox.push_back(pPHX->QueryDeviceContact(name + "_bras1/ctct"));
		this->_ContactBox.push_back(pPHX->QueryDeviceContact(name + "_bras2/ctct"));
		this->_ContactBox.push_back(pPHX->QueryDeviceContact(name + "_bras3/ctct"));

		this->_ContactBox.push_back(pPHX->QueryDeviceContact(name + "_epaule0/ctct"));
		this->_ContactBox.push_back(pPHX->QueryDeviceContact(name + "_epaule1/ctct"));

		this->_ContactBoxServo[this->FINGER] = pPHX->QueryGeom(name + "_doigt0");
		this->_ContactBoxServo[this->ARM] = pPHX->QueryGeom(name + "_epaule0");
		this->_ContactBoxServo[this->SHOULDER] = pPHX->QueryGeom(name + "_epaule1");

		this->_FingerBox = pPHX->QueryGeom(name + "_doigt4");
	}

	Leg::~Leg(void)
	{
	}

	const xkode::lib::String&	Leg::GetName()
	{
		return this->_Name;
	}

	void	Leg::SetAngle(int servo, float angle, float speed)
	{
		if (this->_Left && servo == this->SHOULDER)
			this->_Servos[servo]->GoPositionDeg(-angle, speed);
		else
			this->_Servos[servo]->GoPositionDeg(angle, speed);
	}

	float	Leg::GetAngle(int servo)
	{
		if (this->_Left && servo == this->SHOULDER)
			return -(this->_Servos[servo]->GetPositionDeg());
		return this->_Servos[servo]->GetPositionDeg();
	}

	void	Leg::SetIndex(int servo, int index, float speed)
	{
		//std::cout << "Speed : " << speed << std::endl;
		if (this->_Left && servo == this->SHOULDER)
			this->_Servos[servo]->GoPositionIndex(1024 - index, speed);
		else
			this->_Servos[servo]->GoPositionIndex(index, speed);
	}

	int		Leg::GetIndex(int servo)
	{
		if (this->_Left && servo == this->SHOULDER)
			return 1024 - this->_Servos[servo]->GetPositionIndex();
		return this->_Servos[servo]->GetPositionIndex();
	}

	Position*	Leg::GetPosition(int servo)
	{
		float x, y, z;
		this->_ContactBoxServo[servo]->GetPosition(x ,y, z);
		return new Position(x, y, z);
	}

	float	Leg::GetTorque(int servo)
	{
		return this->_Sensors[servo]->GetTorque();
	}

	// Pour savoir si une patte est en colision on test la colision de toutes 
	// les contactbox qui represente une patte
	bool	Leg::IsInColision()
	{
		std::list<ModaCPP::DeviceContact*>::const_iterator itb = this->_ContactBox.begin();
		std::list<ModaCPP::DeviceContact*>::const_iterator ite = this->_ContactBox.end();

		for (; itb != ite; ++itb)
		{
			if ((*itb)->IsTutching())
			{
				return true;
			}
		}
		return false;
	}

	bool	Leg::IsTuchingGround()
	{
		return this->_GroundBoxContact->IsTutching();
	}

	Position*	Leg::GetFingerPosition()
	{
		float x, y, z;
		this->_FingerBox->GetPosition(x ,y, z);
		return new Position(x, y, z);
	}
}

