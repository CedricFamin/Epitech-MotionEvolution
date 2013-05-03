#include "ApiBlli.h"

namespace ApiBlli
{
	ApiBlli*	ApiBlli::_Instance = NULL;

	ApiBlli*	ApiBlli::GetInstance()
	{
		if (ApiBlli::_Instance == NULL)
			ApiBlli::_Instance = new ApiBlli();
		return ApiBlli::_Instance;
	}

	ApiBlli::ApiBlli(void)
	{
	}

	ApiBlli::~ApiBlli(void)
	{
		if (this->_Robots.size() != 0)
		{
			std::list<Robot*>::iterator itb = this->_Robots.begin();
			std::list<Robot*>::iterator ite = this->_Robots.end();

			for (; itb != ite; ++itb)
			{
				delete (*itb);
			}
			this->_Robots.clear();
		}
	}

	void	ApiBlli::Init(const xkode::lib::String& ip)
	{
		int i = 0;
		bool run = true;
		ModaCPP::Connection *connection = NULL;

		// On test le port 13000 et ceux qui suivent directement pour recuperer tout les robots disponible
		while (run)
		{
			connection = new ModaCPP::Connection(true);
			if ((run = connection->Connect(ip, 13000 + i, true)))
			{
				#ifdef DEBUG
				std::cout << "Connection etablished to Moda Server , on port : " << 13000 + i << std::endl;
				#endif
				ModaCPP::RobotPHX *robot = connection->QueryRobotPHX("/blli");
				if (robot)
				{
					this->_Robots.push_back(new Robot(robot, "blli"));
				}
				i++;
			}
			else
			{
				delete connection;
			}
		}
	}

	const std::list<Robot*>&	ApiBlli::GetRobots()
	{
		return this->_Robots;
	}
}
