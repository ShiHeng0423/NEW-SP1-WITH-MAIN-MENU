#pragma once
#ifndef MUSIC_MAN
#define MUSIC_MAN
#include <vector>
#include <string>
using namespace std;
void Mshutdown();
void MusicInit();
bool MusicPlay(string alias, string arguments);
bool MusicStop(string alias);
struct MLoaded
{
	std::string file = ""; // Can be empty!
	std::string type = ""; // Can be empty!
	std::string alias = ""; // Can be empty!
	int status;
};
bool MusicLoad(std::string file, std::string type, std::string alias, std::string initalvolume, MLoaded* Returnfile);
#endif // !MUSIC_MAN

