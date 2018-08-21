#pragma once
#include <vector>
#include <string>
using namespace std;
void MusicInit();
bool MusicPlay(string alias, string arguments);
bool MusicStop(string alias);
struct mLoad
{
	string file = "";
	string type = "";
	string alias = "";
	int status;
};
bool MusicLoad(std::string file, std::string type, std::string alias, std::string initalvolume, mLoad* Returnfile);


