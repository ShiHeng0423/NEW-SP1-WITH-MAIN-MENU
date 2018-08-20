#include <Windows.h>
#include "mALL.h"
#include <iostream>

using namespace std;

vector<mLoad> loadFiles;
void Mshutdown()
{
	mciSendCommand(MCI_ALL_DEVICE_ID, MCI_CLOSE, MCI_WAIT, NULL);
}
void MusicInit()
{
	mLoad load;
	MusicLoad("Raining Tacos - Parry Gripp  BooneBum.mp3", "mpegvideo", "tacos", "100", &load);
	MusicLoad("Wii Music - Gaming Background Music (HD).mp3", "mpegvideo", "alias", "100", &load);
	loadFiles.push_back(load);
}
bool MusicLoad(string file, string type, string alias, string initalvolume, mLoad* Returnfile)
{
	MCIERROR me;
	string buff;
	buff = "open \"" + file + "\" type " + type + " alias " + alias;
	me = mciSendString(buff.c_str(), NULL, 0, NULL);
	if (me == 0)
	{
		(*Returnfile).status = 0;
		(*Returnfile).file = file;
		(*Returnfile).type = type;
		(*Returnfile).alias = alias;
		buff = "setaudio " + alias + " volume to " + initalvolume;
		me = mciSendString(buff.c_str(), NULL, 0, NULL);
		return true;
	}
	else
	{
		(*Returnfile).status = me;
		return false;
	}

}
bool MusicPlay(string alias, string arguments)
{
	MCIERROR ME;
	string constructstring = "play ";
	constructstring += alias;
	constructstring += " " + arguments;
	ME = mciSendString(constructstring.c_str(), NULL, 0, NULL);
	if (ME != 0)
		return false;
	else
		return true;
}
bool MusicStop(string alias)
{
	MCIERROR ME;
	string constructstring = "stop ";
	constructstring += alias;
	ME = mciSendString(constructstring.c_str(), NULL, 0, NULL);
	if (ME != 0)
		return false;
	else
		return true;
}