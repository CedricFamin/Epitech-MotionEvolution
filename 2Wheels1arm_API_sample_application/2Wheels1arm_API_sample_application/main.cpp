
#include <iostream>
#include <fstream>
#include <sstream>
#include "RobotManager.h"

void CreateInstance()
{
	ModaCPP::Connection *connection=new ModaCPP::Connection(true);

	STARTUPINFO ProcessStartupInfo;
	PROCESS_INFORMATION ProcessInformation;

	memset(&ProcessStartupInfo,0,sizeof(ProcessStartupInfo));
	memset(&ProcessInformation,0,sizeof(ProcessInformation));
	ProcessStartupInfo.cb=sizeof(ProcessStartupInfo);

	//creates the Exec process
	std::string marilouExec = "D:\\Dev\\Marilou Robotics\\Bin\\Exec.exe"; //"%MARILOU_INSTALL_DIR%bin\\Exec.exe";
	LPTSTR mbsps[] = {
		TEXT(" /p:E:\\Work\\github\\Epitech-MotionEvolution\\2wheels1arm_simulation\\2wheels1armByBLLI.mdsp /port:13000"),
	};

	for (LPTSTR mbsp : mbsps)
	{
		if(!CreateProcess(marilouExec.c_str(), mbsp, NULL, NULL, TRUE, 
			NORMAL_PRIORITY_CLASS, NULL, NULL, &ProcessStartupInfo, &ProcessInformation))
		{
			DWORD errorCode = GetLastError();
			// Translate ErrorCode to String.
			LPTSTR Error = 0;
			if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
							NULL, errorCode, 0, (LPTSTR)&Error, 0, NULL) == 0)
			{
			  return ;
			}
			std::cout << Error << std::endl;
			// Free the buffer.
			if(Error)
			{
			   LocalFree(Error);
			   Error = 0;
			}
		}
	}

	connection->WaitMODAServerIsRunningOnLocalhost(5000);

}
int main(int argc, char* argv[])
{
	//CreateInstance();
	RobotManager * R = new RobotManager();
}