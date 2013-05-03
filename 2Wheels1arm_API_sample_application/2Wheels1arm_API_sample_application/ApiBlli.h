#pragma once

#include <list>
#include "Robot.h"

namespace ApiBlli
{
	class ApiBlli
	{
	public:
		/// Recuperer l'instance de l'API
		static ApiBlli*				GetInstance();
		// Initialise les connexions avec marilou
		void						Init(const xkode::lib::String& ip);
		// Recuperer la liste des robots disponible selon le nombre d'instance de marilou
		const std::list<Robot*>&	GetRobots();

	private:
		ApiBlli(void);
		~ApiBlli(void);

		std::list<Robot*>	_Robots;
		static ApiBlli*		_Instance;
	};
}

