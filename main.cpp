#include <iostream>
#include <Windows.h>
#include <string>
#include <ctime>

DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress);
void WriteToMemory(HANDLE hProcHandle);
std::string GameName = "AssaultCube";
LPCSTR LGameWindow = "AssaultCube";
std::string GameStatus;
bool IsGameAvail;
bool UpdateOnNextRun;

bool AmmoStatus;
BYTE AmmoValue[] = {0xA3, 0x1C, 0x0, 0x0};
DWORD AmmoBaseAddress = {0x00509B74};
DWORD AmmoOffset[] = { 0x384,0x14,0x0  };

bool HealthStatus;
BYTE HealthValue[] = { 0x39, 0x5, 0x0, 0x0 };
DWORD HealthBaseAddress = { 0x00509B74 };
DWORD HealthOffset[] = { 0xF8 };

int main() {
	HWND hGameWindow = NULL;
	int timeSinceLastUpdate = clock();
	int gameAvailTMR = clock();
	int onePressTMR = clock();
	DWORD dwProcID = NULL;
	HANDLE hProcHandle = NULL;
	UpdateOnNextRun = true;
	std::string sAmmoStatus = "OFF";
	std::string sHealthStatus = "OFF";
	while (!GetAsyncKeyState(VK_INSERT)) {
		if (clock() - gameAvailTMR > 100) {
			gameAvailTMR = clock();
			IsGameAvail = false;
			hGameWindow = FindWindow(NULL, LGameWindow);
			if (hGameWindow) {
				GetWindowThreadProcessId(hGameWindow, &dwProcID);
				if (dwProcID !=0) {
					hProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcID);
					if (hProcHandle == INVALID_HANDLE_VALUE || hProcHandle == NULL) {
						GameStatus = "Faild to open process for valid handle";
					}
					else {
						GameStatus = "AssaultCube Hacking";
						IsGameAvail = true;

					}
				}
				else {
					GameStatus = "Failded to get Process ID";
				}
			}
			else {
				GameStatus = "AssaultCube NOT FOUND";
			}
			if (UpdateOnNextRun || clock() - timeSinceLastUpdate > 5000) {
				system("cls");
				std::cout << "------------------------------------------------" << std::endl;
				std::cout << "                 AssaultCube Mem Hacker" << std::endl;
				std::cout << "------------------------------------------------" << std::endl;
				std::cout << "GAME STATUS:" << GameStatus << std::endl;
				std::cout << "[F1] Unlimited Ammo ->" << sAmmoStatus << "<-" << std::endl;
				std::cout << "[F2] Unlimited Health ->" << sHealthStatus << "<-" << std::endl;
				std::cout << "[INSERT] Exit" << std::endl;
				UpdateOnNextRun = false;
				timeSinceLastUpdate = clock();
			}
			if (IsGameAvail) {
				WriteToMemory(hProcHandle);
			}
		}
		if (clock() - onePressTMR > 400) {
			if (IsGameAvail) {
				if (GetAsyncKeyState(VK_F1)) {
					onePressTMR = clock();
					AmmoStatus = !AmmoStatus;
					UpdateOnNextRun = true;
					if (AmmoStatus)sAmmoStatus = "ON";
					else sAmmoStatus = "OFF";
				}else if (GetAsyncKeyState(VK_F2)) {
					onePressTMR = clock();
					HealthStatus = !HealthStatus;
					UpdateOnNextRun = true;
					if (HealthStatus)sHealthStatus = "ON";
					else sHealthStatus = "OFF";
				}
			}
		}
	}
	CloseHandle(hProcHandle);
	CloseHandle(hGameWindow);
	return 0;

	
}

DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress) {
	DWORD pointer = BaseAddress;
	DWORD pTemp;
	DWORD pointerAddr;
	for (int c = 0; c < PointerLevel; c++) {
		if (c == 0) {
			ReadProcessMemory(hProcHandle, (LPCVOID)pointer, &pTemp, sizeof(pTemp), NULL);
		}
		pointerAddr = pTemp + Offsets[c];
		ReadProcessMemory(hProcHandle, (LPCVOID)pointerAddr, &pTemp, sizeof(pTemp), NULL);
	}
	return pointerAddr;
}

void WriteToMemory(HANDLE hProcHandle) {
	DWORD AddressToWrite;
	if (AmmoStatus) {
		AddressToWrite = FindDmaAddy(3, hProcHandle,AmmoOffset,AmmoBaseAddress);
		WriteProcessMemory(hProcHandle, (BYTE*)AddressToWrite, &AmmoValue, sizeof(AmmoValue), NULL);

	}
	if (HealthStatus) {
		AddressToWrite = FindDmaAddy(1, hProcHandle, HealthOffset, HealthBaseAddress);
		WriteProcessMemory(hProcHandle, (BYTE*)AddressToWrite, &HealthValue, sizeof(HealthValue), NULL);

	}
}