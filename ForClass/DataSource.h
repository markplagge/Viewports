#include "Angel.h"
#include <cstring>
#include <iostream>
#include <string>
using namespace std;
typedef Angel::vec4 point4;
typedef Angel::vec4 color4;


class DataSource
{
public:

	GLfloat Size;
	//GLfloat Rotation;
	//GLfloat RotationSpeed;
	GLfloat Tilt;

	GLfloat Translation; 
	GLfloat TranslationSpeed;
	GLfloat Rotation ;
	GLfloat RotationSpeed ;

	GLfloat Distance ;//5.0;//4.0;
	GLfloat Inclination ;
	string planetName;
	char planetCategory;
	GLuint program;
	color4 ambient;
	color4 diffuse;
	color4 specular;
	GLfloat shininess;
	
	DataSource(int planet, string NewPlanetName);
};






DataSource::DataSource(int planet, string NewPlanetName)
{
	//saturn moon distance adjuster:
int	smoon = 9.6436;
	planetName = NewPlanetName;
	planetCategory = 'M';
	program = NULL;
	bool customMaterials = false;
	//
	//*************Material Setup:**************************************************
		GLfloat materials[29][4][4] = {{{0.0215, 0.1745, 0.0215, 1.0}, {0.07568, 0.61424, 0.07568, 1.0}, {0.633, 0.727811, 0.633, 1.0}, {0.6, 0.6, 0.6, 0.6}},
		{{0.135, 0.2225, 0.1575, 1.0}, {0.54, 0.89, 0.63, 1.0}, {0.316228, 0.316228, 0.316228, 1.0}, {0.1, 0.1, 0.1, 0.1}},
		{{0.05375, 0.05, 0.06625, 1.0}, {0.18275, 0.17, 0.22525, 1.0}, {0.332741, 0.328634, 0.346435, 1.0}, {0.3, 0.3, 0.3, 0.3}},
		{{0.25, 0.20725, 0.20725, 1.0}, {1, 0.829, 0.829, 1.0}, {0.296648, 0.296648, 0.296648, 1.0}, {0.088, 0.088, 0.088, 0.088}},
		{{0.1745, 0.01175, 0.01175, 1.0}, {0.61424, 0.04136, 0.04136, 1.0},{0.727811, 0.626959, 0.626959, 1.0}, {0.6, 0.6, 0.6, 0.6}},
		{{0.1, 0.18725, 0.1745, 1.0}, {0.396, 0.74151, 0.69102, 1.0}, {0.297254, 0.30829, 0.306678, 1.0}, {0.1, 0.1, 0.1, 0.1}},
		{{0.329412, 0.223529, 0.027451, 1.0}, {0.780392, 0.568627, 0.113725, 1.0}, {0.992157, 0.941176, 0.807843, 1.0}, {0.21794872, 0.21794872, 0.21794872, 0.21794872}},
		{{0.2125, 0.1275, 0.054, 1.0}, {0.714, 0.4284, 0.18144, 1.}, {0.393548, 0.271906, 0.166721, 1.0}, {0.2, 0.2, 0.2, 0.2}},
		{{0.25, 0.25, 0.25, 1.0}, {0.4, 0.4, 0.4, 1.0}, {0.774597, 0.774597, 0.774597, 1.0}, {0.6, 0.6, 0.6, 0.6}},
		{{0.19125, 0.0735, 0.0225, 1.0}, {0.7038, 0.27048, 0.0828, 1.0}, {0.256777, 0.137622, 0.086014, 1.0}, {0.1, 0.1, 0.1, 0.1}},
		{{0.24725, 0.1995, 0.0745, 1.0}, {0.75164, 0.60648, 0.22648, 1.0}, {0.628281, 0.555802, 0.366065, 1.0}, {0.4, 0.4, 0.4, 0.4}},
		{{0.19225, 0.19225, 0.19225, 1.0}, {0.50754, 0.50754, 0.50754, 1.0}, {0.508273, 0.508273, 0.508273, 1.0}, {0.4, 0.4, 0.4, 0.4}},
		{{0.0, 0.0, 0.0, 1.0}, {0.01, 0.01, 0.01, 1.0}, {0.50, 0.50, 0.50, 1.0}, {0.25, 0.25, 0.25, 0.25}},
		{{0.0, 0.1, 0.06, 1.0}, {0.0, 0.50980392, 0.50980392, 1.0}, {0.50196078, 0.50196078, 0.50196078, 1.0}, {0.25, 0.25, 0.25, 0.25}},
		{{0.0, 0.0, 0.0, 1.0}, {0.1, 0.35, 0.1, 1.0}, {0.45, 0.55, 0.45, 1.0}, {0.25, 0.25, 0.25, 0.25}},
		{{0.0, 0.0, 0.0, 1.0}, {0.5, 0.0, 0.0, 1.0}, {0.7, 0.6, 0.6, 1.0}, {0.25, 0.25, 0.25, 0.25}},
		{{0.0, 0.0, 0.0, 1.0}, {0.55, 0.55, 0.55, 1.0}, {0.70, 0.70, 0.70, 1.0}, {0.25, 0.25, 0.25, 0.25}},
		{{0.0, 0.0, 0.0, 1.0}, {0.5, 0.5, 0.0, 1.0}, {0.60, 0.60, 0.50, 1.0}, {0.25, 0.25, 0.25, 0.25}},
		{{0.02, 0.02, 0.02, 1.0}, {0.01, 0.01, 0.01, 1.0}, {0.4, 0.4, 0.4, 1.0}, {0.078125, 0.078125, 0.078125, 0.078125}},
		{{0.0, 0.05, 0.05, 1.0}, {0.4, 0.5, 0.5, 1.0}, {0.04, 0.7, 0.7, 1.0}, {0.078125, 0.078125, 0.078125, 0.078125}},
		{{0.0, 0.05, 0.0, 1.0}, {0.4, 0.5, 0.4, 1.0}, {0.04, 0.7, 0.04, 1.0}, {0.078125, 0.078125, 0.078125, 0.078125}},
		{{0.05, 0.0, 0.0, 1.0}, {0.5, 0.4, 0.4, 1.0}, {0.7, 0.04, 0.04, 1.0}, {0.078125, 0.078125, 0.078125, 0.078125}},
		{{0.05, 0.05, 0.05, 1.0}, {0.5, 0.5, 0.5, 1.0}, {0.7, 0.7, 0.7, 1.0}, {0.078125, 0.078125, 0.078125, 0.078125}},
		{{0.05, 0.05, 0.0, 1.0}, {0.5, 0.5, 0.4, 1.0}, {0.7, 0.7, 0.04, 1.0}, {0.078125, 0.078125, 0.078125, 0.078125}},
		{{0.07, 0.07, 0.11, 0.2}, {0.35, 0.35, 0.53, 0.3}, {0.21, 0.21, 0.39, 0.1}, {0.01, 0.01, 0.01, 0.01}},
		{{0.1, 0.1, 0.1, 1.0}, {0.5, 0.5, 0.1, 1.0}, {0.9, 0.9, 0.9, 0.2}, {0.6, 0.6, 0.6, 0.6}},
		{{0.5, 0.5, 0.1, 0.2}, {0.7, 0.7, 0.1, 0.3}, {0.9, 0.9, 0.9, 0.2}, {0.6, 0.6, 0.6, 0.6}},
		{{0.2, 0.2, 0.2, 0.9}, {0.7, 0.7, 0.7, 0.9}, {0.9, 0.9, 0.9, 0.9}, {0.1, 0.1, 0.1, 0.1}},
		{{0.07, 0.07, 0.07, 1.0}, {0.7, 0.7, 0.7, 1.0}, {0.21, 0.21, 0.21, 1.0}, {0.01, 0.01, 0.01, 0.01}}};
		/**
		*  First column:  emerald, jade, obsidian, pearl, ruby, turquoise
		*  2nd column:  brass, bronze, chrome, copper, gold, silver
		*  3rd column:  black, cyan, green, red, white, yellow plastic
		*  4th column:  black, cyan, green, red, white, yellow rubber
		*  And other miscellaneous materials
		*/

	// = InitShader( "vShader.glsl", "fShader.glsl" )
	// thar be dragons
	// Sun's data
		switch(planet)
	{

	case 0:
		Size = 10.0;//2.0;
		Rotation = 0.0;
		RotationSpeed = 0.1 * 10;
		Tilt = 7.25;
		planetName = "Sun";
		planetCategory = 'P';
		Translation = 0.0;
		TranslationSpeed = 0.0;
		Distance = 0.0;
		Inclination = 0.0;
		break;


		// Mercury's data
	case 1:
		Size = 0.38251;
		Translation = 0.0;
		TranslationSpeed = 4.152019/10;
		Rotation = 0.0;
		RotationSpeed = 0.23310 * 10;
		Tilt = 0.01;
		Distance = 20.0;//5.0;//4.0;
		Inclination = 7.00487;
		planetName = "Mercury";
		planetCategory = 'P';

		break;
		// Venus' data
	case 2:
		Size = 0.94885;
		Translation = 40.0;
		TranslationSpeed = 1.6254949/10;
		Rotation = 0.0;
		RotationSpeed = -1.0 * 10;
		Tilt = 177.36;
		Distance = 7.0;
		Inclination = 3.39471;
		planetName = "Venus";
		planetCategory = 'P';

		break;

	case 3:
		planetName = "Earth";
		// Earth's data
		Size = 1.0;
		Translation = 80.0;
		TranslationSpeed = 0.99998260/10;
		Rotation = 0.0;
		RotationSpeed = 0.41038 * 10;//0.0041038 * 10;
		Tilt = 23.45;
		Distance = 11.0;
		Inclination = 0.00005;
		planetCategory = 'P';
		break;


		// Moon's Data
	case 4:
		planetName = "Moon";
		Size = 0.27287;
		Translation = 80.0;
		TranslationSpeed = 13.368/10;
		Rotation = 0.0;
		RotationSpeed = 0.00013013 * 10;//0.0000013013 * 10;
		Tilt = 0.0;
		Distance = 1.5;
		Inclination = 5.16;
		break;

		// Mars' Data
	case 5:
		planetName = "Mars";
		planetCategory = 'P';
		Size = 0.5318;
		Translation = 120.0;
		TranslationSpeed = 0.53167519/10;
		Rotation = 0.0;
		RotationSpeed = 0.42219 * 10;//0.0042219 * 10;
		Tilt = 25.19;
		Distance = 14.0;
		Inclination = 1.85061;

		break;
		// Phobos' Data


	case 6:
		planetName = "Phobos";
		Size = 0.0174;
		Translation = 0.0;
		TranslationSpeed = 11.40/10;// Divided by 100 also
		Rotation = 0.0;
		RotationSpeed = 0.000000152 * 10;//0.00000000152 * 10;
		Tilt = 0.046;
		Distance = 1.5;
		Inclination = 1.075;
		break;


	case 7:
		// Deimos' Data
		planetName = "Deimos";
		Size = 0.0097;
		Translation = 180.0;
		TranslationSpeed = 2.894/10;// Divided by 10 also
		Rotation = 0.0;
		RotationSpeed = 0.0000006011 * 10;//0.000000006011 * 10;
		Tilt = 0.897;
		Distance = 2.7633;
		Inclination = 1.793;
		break;
		//Jupiter's Data
	case 8:
		planetName = "Jupiter";
		planetCategory = 'P';
		Size = 10.863;
		Translation = 160.0;
		TranslationSpeed = 0.084298445/10;
		Rotation = 0.0;
		RotationSpeed = 0.17017 * 10;//0.0017017 * 10;
		Tilt = 3.13;
		Distance = 28.0;
		Inclination = 1.30530;
		Distance = Distance + 3; // Stuff is close together.
		break;

	case 9:
		planetName = "Io";
		// Io's Data
		Size = 0.28608;
		Translation = 90.0;
		TranslationSpeed = 206.5/10;
		Rotation = 0.0;
		RotationSpeed = 0.0000008425 * 10;//0.000000008425 * 10;
		Tilt = 0.0;
		Distance = 3.049 + 10.863;
		Inclination = 0.036;
		break;

	case 10:
		planetName = "Europa";
		// Europa's Data
		Size = 0.24512;
		Translation = 180.0;
		TranslationSpeed = 102.9/10;
		Rotation = 0.0;
		RotationSpeed = 0.000001691 * 10;//0.00000001691 * 10;
		Tilt = 0.016;
		Distance = 4.851+ 10.863;
		Inclination = 0.466;
		break;

	case 11:
		planetName = "Ganymede";
		// Ganymede's Data
		Size = 0.41323;
		Translation = 270.0;
		TranslationSpeed = 51.05/10;
		Rotation = 0.0;
		RotationSpeed = 0.000003408 * 10;//0.00000003408 * 10;
		Tilt = 0.068;
		Distance = 7.737+ 10.863;
		Inclination = 0.177;
		break;

	case 12:
		// Callisto's Data
		planetName = "Callisto";
		Size = 0.37853;
		Translation = 0.0;
		TranslationSpeed = 21.88/10;
		Rotation = 0.0;
		RotationSpeed = 0.000007949 * 10;//0.00000007949 * 10;
		Tilt = 0.356;
		Distance = 13.609 + 10.863;
		Inclination = 0.192;
		break;

	case 13:
		planetName = "Saturn";
		planetCategory = 'P';
		//Saturn's Data
		Size = 9.001;
		Translation = 0.0;
		TranslationSpeed = 0.033958742/10;
		Rotation = 0.0;
		RotationSpeed = 0.1800 * 10;//0.001800 * 10;
		Tilt = 26.73;
		Distance = 52.0;
		Inclination = 2.48446;
		Distance = Distance + 5; // Stuff is close together.
		break;

	case 14:
		planetName = "Titan";
		// Titan's Data
		Size = 0.40448;
		Translation = 90.0;
		TranslationSpeed = 22.90/10;
		Rotation = 0.0;
		RotationSpeed = 0.000007597 * 10;//0.00000007597 * 10;
		Tilt = 0.6;
		Distance = 10.659 +smoon;
		Inclination = 0.28;
		break;

	case 15:
		planetName = "Iapetus";
		// Iapetus's Data
		Size = 0.1154;
		Translation = 180.0;
		TranslationSpeed = 4.604/10;
		Rotation = 0.0;
		RotationSpeed = 0.00003778 * 10;//0.0000003778 * 10;
		Tilt = 14.968;
		Distance = 31.063 +smoon;
		Inclination = 7.489;
		break;

	case 16:
		planetName = "Rhea";
		// Rhea's Data
		Size = 0.1201;
		Translation = 270.0;
		TranslationSpeed = 80.84/10;
		Rotation = 0.0;
		RotationSpeed = 0.000002152 * 10;//0.00000002152 * 10;
		Tilt = 0.029;
		Distance = 4.5979 +smoon ;
		Inclination = 0.331;
		break;

	case 17:
		planetName = "Mima";
		// Mimas's Data
		Size = 0.03122;
		Translation = 270.0;
		TranslationSpeed = 388/10;
		Rotation = 0.0;
		RotationSpeed = 0.00000449 * 10;//-0.0000000449 * 10;
		Tilt = 0.001;
		Distance = 1.6186 +smoon;
		Inclination = 1.572;
		break;
		// Enceladus's Data
	case 18:
		planetName = "Enceladus";
		Size = 0.03962;
		Translation = 270.0;
		TranslationSpeed = 267/10;
		Rotation = 0.0;
		RotationSpeed = 0.00000653 * 10;//-0.0000000653 * 10;
		Tilt = 0.001;
		Distance = 2.0766+smoon ;
		Inclination = 0.009;
		break;

	case 19:
		planetName = "Tethys";
		// Tethys's Data
		Size = 0.08422;
		Translation = 270.0;
		TranslationSpeed = 193.5/10;
		Rotation = 0.0;
		RotationSpeed = 0.000008992 * 10;//-0.00000008992 * 10;
		Tilt = 0.002;
		Distance = 2.5706+smoon;
		Inclination = 1.091;
		break;

		// Dione's Data
	case 20:
		planetName = "Dione";
		Size = 0.08834;
		Translation = 270.0;
		TranslationSpeed = 133.4/10;
		Rotation = 0.0;
		RotationSpeed = 0.00001304 * 10;//-0.0000001304 * 10;
		Tilt = 0.005;
		Distance = 3.2924+smoon;
		Inclination = 0.028;
		break;

	case 21:
		planetName = "Uranus";
		planetCategory = 'P';
		//Uranus's Data
		Size = 3.9680;
		Translation = 240.0;
		TranslationSpeed = 0.011902375/10;
		Rotation = 0.0;
		RotationSpeed = 0.27 * 10;//0.0027 * 10;
		Tilt = 97.77;
		Distance = 65.0;
		Inclination = 0.76986;
		break;

	case 22:
		planetName = "Neptune";
		planetCategory = 'P';
		//Neptune's Data
		Size = 3.8559;
		Translation = 280.0;
		TranslationSpeed = 0.0060682808/10;
		Rotation = 0.0;
		RotationSpeed = 0.316 * 10;//0.00316 * 10;
		Tilt = 28.32;
		Distance = 77.677723;//77.677723
		Inclination = 1.76917;
		break;

	case 23:
		planetName = "Pluto";
		planetCategory = 'P';
		//Pluto' Data
		Size = 0.1842;
		Translation = 320.0;
		TranslationSpeed = 0.0040335486/10;
		Rotation = 0.0;
		RotationSpeed = 2.6282 * 10;//0.026282 * 10;
		Tilt = 122.53;
		Distance = 101.99379;//101.99379
		Inclination = 17.14175;
		break;

	case 24:
		planetName = "Charon";
		planetCategory = 'M';
		//Charon's Data
		Size = 0.0931;
		Translation = 320.0;
		TranslationSpeed = 57.19/10;
		Rotation = 0.0;
		RotationSpeed = -0.00003042 * 10;//0.026282 * 10;-0.0000003042
		Tilt = 0.001;
		Distance = 8.354;//8.354
		Inclination = 6.151;//96.151;
		break;
	}
	
	//The planets are too close:
	if (planetCategory == 'P')
	{
		Distance = Distance  + 15;
	}


	if (!customMaterials)
	{
		if (planetCategory == 'P')
		{//Planet standard materials:
			ambient = color4(0.0215, 0.1745,0.0215,1.0);
			diffuse = color4(0.07568, 0.61424, 0.07568, 1.0);
			specular = color4(0.633, 0.727811, 0.633, 1.0);
			shininess =  0.6;
		}
		else
		{
			ambient = color4(0.25, 0.20725, 0.20725, 1.0);
			diffuse = color4(1, 0.829, 0.829, 1.0);
			specular = color4(0.296648, 0.296648, 0.296648, 1.0);
			shininess =  0.6;

		}
			
	}
}
