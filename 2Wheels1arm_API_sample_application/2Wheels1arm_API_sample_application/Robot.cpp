#include "Robot.h"

namespace ApiBlli
{
	Robot::Robot(ModaCPP::RobotPHX* robot, const xkode::lib::String& name)
	{
		this->_Robot = robot;
		this->_OneLeg = true;
		this->_Legs[0] = new Leg(robot, "p0", true);
		this->_Gyro = robot->QueryDeviceAccelGyro("basPlatine/gyro");
		this->_Platine = robot->QueryDeviceContact("basPlatine/ctct");
		this->_PlatineBox = robot->QueryGeom("basPlatine");
	}

	Robot::~Robot(void)
	{
		std::map<int, Leg*>::iterator itb = this->_Legs.begin();
		std::map<int, Leg*>::iterator ite = this->_Legs.end();

		for (; itb != ite; ++itb)
		{
			delete itb->second;
		}
		this->_Legs.clear();
	}

	void		Robot::Init()
	{
		this->_Robot->GetConnection()->ResetSimulation();
	}

	void		Robot::SetAngle(int servo, float angle, float speed)
	{
		this->_Legs[0]->SetAngle(servo, angle, speed);
	}

	float	Robot::GetAngle(int servo)
	{
		return this->_Legs[0]->GetAngle(servo);
	}

	void	Robot::SetIndex(int servo, int index, float speed)
	{
		this->_Legs[0]->SetIndex(servo, index, speed);
	}

	int		Robot::GetIndex(int servo)
	{
		return this->_Legs[0]->GetIndex(servo);
	}

	Position*		Robot::GetFingerPosition()
	{
		return this->_Legs[0]->GetFingerPosition();
	}

	Position*	Robot::GetPosition(int servo)
	{
		return this->_Legs[0]->GetPosition(servo);
	}

		bool	Robot::IsLegTuchingGround()
	{
		return this->_Legs[0]->IsTuchingGround();
	}

	bool	Robot::IsInColision()
	{
		return this->_Legs[0]->IsInColision();
	}

	float Robot::GetTorque(int servo)
	{
		return this->_Legs[0]->GetTorque(servo);
	}

	const std::string Robot::GetLegName()
	{
		return std::string( (const char *)new xkode::lib::String(this->_Legs[0]->GetName()));
	}

	void	Robot::Wait(int _ms)
	{
		this->_Robot->Sleep(_ms);
	}

	Position*	Robot::GetPosition()
	{
		float x, y, z;
		this->_PlatineBox->GetPosition(x ,y, z);
		return new Position(x, y, z);
	}

	bool	Robot::IsTuchingGround()
	{
		return this->_Platine->IsTutching();
	}

	GyroValues*		Robot::GetGyroValues()
	{
		GyroValues* res = new GyroValues();

		Moda::Commons::AXESXYZValues values =  this->_Gyro->GetXYZInstantValues();
		res->LinearAccelerations[0] = values.LinearAccelerations[0];
		res->LinearAccelerations[1] = values.LinearAccelerations[1];
		res->LinearAccelerations[2] = values.LinearAccelerations[2];
		res->Angles[0] = values.Angles[0];
		res->Angles[1] = values.Angles[1];
		res->Angles[2] = values.Angles[2];
		return res;
	}
}

