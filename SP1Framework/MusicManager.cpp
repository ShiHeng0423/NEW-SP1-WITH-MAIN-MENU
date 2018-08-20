#include <Windows.h>
#include "MusicManager.h"
#include <iostream>
#pragma comment( lib, "winmm.lib" )
using namespace std;
vector<MLoaded> LoadedFiles;
void Mshutdown()
{
	mciSendCommand(MCI_ALL_DEVICE_ID, MCI_CLOSE, MCI_WAIT, NULL);
}
void MusicInit()
{
	MLoaded loaded;
	// mpegvideo is Audio for MP3 (*.mp3) [VOLUME WILL NOT WORK IS WAVEAUDIO IS USED (*.wav)!!]
	// volume is a string eg "0" - "100"
	// &loaded is a MLoaded Struct.
	MusicLoad("Raining Tacos - Parry Gripp  BooneBum.mp3", "mpegvideo", "tacos", "100", &loaded);
	MusicLoad("Wii Music - Gaming Background Music (HD).mp3", "mpegvideo", "alias", "100", &loaded);
	LoadedFiles.push_back(loaded);
}
bool MusicLoad(string file, string type, string alias, string initalvolume, MLoaded* Returnfile)
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