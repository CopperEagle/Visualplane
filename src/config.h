#ifndef CONFIG_H
#define CONFIG_H

#include <string>


struct Config
{
	std::string name = "test";

	int width = 1920;//360;
	int height = 1080;//202;
	int fps = 30;
	bool fullscreen = false;

	bool show = true;
	bool record = false;
	bool use_H265 = false;
	std::string preset = "ultrafast";
	std::string video_format = "mp4";

	std::string fonts_dir = "../rsc/fonts/";
	std::string shaders_dir = "../rsc/";

};


#endif // CONFIG_H