// fragment shading of sphere model

#include "Angel.h"
//Added these lines to support the datasource functionality (semi-oo)
#include "DataSource.h"
#include <iostream>
#include <string>
#include <vector>
#include <varargs.h>

//// Some inclusions for textureific stuff:
#include "SOIL.h"


#include <atlstr.h>
#include <stdio.h>


static GLfloat lens = 60.0;
const int NumTimesToSubdivide = 5;
const unsigned int NumTriangles        = 263840;  
const unsigned long NumVertices         = 3 * NumTriangles;

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

//Some camera handling code:
bool spaceShipWarp = false;


typedef struct Sphere
{
	int firstPoint;
	int numPoints;
	int pointsPerStrip;
	int numStrips;
	int mode;
};

// Earth's data
static color4 earthAmbient(0.0215, 0.1745, 0.0215, 1.0);
static color4 earthDiffuse(0.07568, 0.61424, 0.07568, 1.0);
static color4 earthSpecular(0.633, 0.727811, 0.633, 1.0);
static GLfloat earthShininess = 0.6;
static GLfloat earthSize = 1.0;
static GLfloat earthTranslation = 80.0;
static GLfloat earthTranslationSpeed = 0.99998260/10;
static GLfloat earthRotation = 0.0;
static GLfloat earthRotationSpeed = 0.41038 * 10;//0.0041038 * 10;
static GLfloat earthTilt = 23.45;
static GLfloat earthDistance = 11.0;
static GLfloat earthInclination = 0.00005;
// Moon's Data
static color4 moonAmbient(0.25, 0.20725, 0.20725, 1.0);
static color4 moonDiffuse(1, 0.829, 0.829, 1.0);
static color4 moonSpecular(0.296648, 0.296648, 0.296648, 1.0);
static GLfloat moonShininess = 0.088;
static GLfloat moonSize = 0.27287;
static GLfloat moonTranslation = 80.0;
static GLfloat moonTranslationSpeed = 13.368/10;
static GLfloat moonRotation = 0.0;
static GLfloat moonRotationSpeed = 0.00013013 * 10;//0.0000013013 * 10;
static GLfloat moonTilt = 0.0;
static GLfloat moonDistance = 1.5;
static GLfloat moonInclination = 5.16;

point4 points[NumVertices];
vec3   normals[NumVertices];
vec2	texCoords[NumVertices];
GLuint Program;
GLfloat ww, wh;
point4 at, eye;
GLfloat t = 0.0, theta = 0.0;
GLfloat cameraDistance = 6.0;
point4 light_position( 1.0, 1.0, 1.0, 1.0 );
// Initialize shader lighting parameters
color4 light_ambient( 0.2, 0.2, 0.2, 1.0 );
color4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
color4 light_specular( 1.0, 1.0, 1.0, 1.0 );

color4 material_ambient( 1.0, 0.0, 1.0, 1.0 );
color4 material_diffuse( 1.0, 0.8, 0.0, 1.0 );
color4 material_specular( 1.0, 0.0, 1.0, 1.0 );
float  material_shininess = 100.0;


// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;

//----------------------------------------------------------------------------

int Index = 0;


/*
Create a sphere centered at the origin, with radius r, and precision n
Draw a point for zero radius spheres
Use CCW facet ordering
"method" is 0 for quads, 1 for triangles
(quads look nicer in wireframe mode)
Partial spheres can be created using theta1->theta2, phi1->phi2
in radians 0 < theta < 2pi, -pi/2 < phi < pi/2
*/
Sphere CreateSphere(double r, int n, int method, double theta1, double theta2, double phi1, double phi2)
{
	Sphere sphere;
	int i, j;
	vec3 e, e2;
	vec4 p, p2;
	double jdivn, j1divn, idivn, dosdivn, unodivn=1/(double)n, ndiv2=(double)n/2, t1, t2, t3, cost1, cost2, cte1, cte3;
	cte3 = (theta2-theta1)/n;
	cte1 = (phi2-phi1)/ndiv2;
	dosdivn = 2*unodivn;
	/* Handle special cases */
	if (r < 0)
		r = -r;
	if (n < 0){
		n = -n;
		ndiv2 = -ndiv2;
	}
	if (n < 4 || r <= 0) {
		points[Index] = vec4(0.0, 0.0, 0.0, 1.0);
		sphere.firstPoint = Index++;
		sphere.mode = GL_POINTS;
		sphere.numPoints = 1;
		sphere.numStrips = 1;
		sphere.pointsPerStrip = 1;
		return sphere;
	}

	t2 = phi1;
	cost2 = cos(phi1);
	j1divn = 0;
	sphere.firstPoint = Index;
	sphere.mode = method;
	sphere.numStrips = ndiv2;
	for (j = 0; j < ndiv2; j++) {
		t1 = t2;//t1 = phi1 + j * cte1;
		t2 += cte1;//t2 = phi1 + (j + 1) * cte1;
		t3 = theta1 - cte3;
		cost1 = cost2;//cost1=cos(t1);
		cost2 = cos(t2);
		e.y = sin(t1);
		e2.y = sin(t2);
		p.y = r * e.y;
		p2.y = r * e2.y;

		//if (method == 0)
		//   glBegin(GL_QUAD_STRIP);
		//else
		//   glBegin(GL_TRIANGLE_STRIP);

		idivn = 0;
		jdivn = j1divn;
		j1divn += dosdivn;//=2*(j+1)/(double)n;
		for (i = 0; i <= n; i++) {
			//t3 = theta1 + i * (theta2 - theta1) / n;
			t3 += cte3;
			e.x = cost1 * cos(t3);
			//e.y = sin(t1);
			e.z = cost1 * sin(t3);
			p.x = r * e.x;
			//p.y = c.y + r * e.y;
			p.z = r * e.z;
			normals[Index] = e;
			p.w = 1.0;
			points[Index] = p;
			texCoords[Index].x = 1.0 - idivn;
			texCoords[Index].y = jdivn;
			Index++;
			//glNormal3f(e.x,e.y,e.z);
			//glTexCoord2f(idivn,jdivn);
			//glVertex3f(p.x,p.y,p.z);

			e2.x = cost2 * cos(t3);
			//e.y = sin(t2);
			e2.z = cost2 * sin(t3);
			p2.x = r * e2.x;
			//p.y = c.y + r * e.y;
			p2.z = r * e2.z;
			normals[Index] = e2;
			p2.w = 1.0;
			points[Index] = p2;
			texCoords[Index].x = idivn;
			texCoords[Index].y = j1divn;
			Index++;
			//glNormal3f(e2.x,e2.y,e2.z);
			//glTexCoord2f(idivn,j1divn);
			//glVertex3f(p2.x,p2.y,p2.z);
			idivn += unodivn;
		}
		//printf("i is %i.\n", i);
		//glEnd();
	}
	sphere.numPoints = Index - sphere.firstPoint;
	sphere.pointsPerStrip = 2 * n + 2;
	return sphere;
}

class Planet{
public:
	Sphere sphere;
	GLfloat planetSize;
	Planet * satellite;
	Planet * nextPlanet;
	color4 ambient, diffuse, specular;
	GLfloat shininess;
	GLint program, modelView;
	GLint mAmbientID, mDiffuseID, mSpecularID, mShininessID;
	GLfloat rotationAngle, rotationSpeed;
	GLfloat translationAngle, translationSpeed;
	GLfloat distance, tiltAngle;
	GLfloat inclination;
	point4 position;
	//Texture Items:
	GLint textureID, mTextureID;

	Planet(GLfloat size = 1.0);
	void setProgram(GLint newProgram);
	void setMaterial(color4 theAmbient, color4 theDiffuse, color4 theSpecular, GLfloat theShininess);
	void update();
	void draw(mat4 * ModelView, mat4 * cummulative);
	void addPlanet(Planet * newPlanet);
	void addSatellite(Planet * newSatellite);
	Planet * getPlanet(GLint index);
	Planet * getSatellite(GLint index);
	//Additional support for the partially contained planetoid data:
	DataSource * thisPlanetInfo;
	string planetName;
	char planetCategory;
	//Additional Support - new Constructor
	Planet(int officialPlanetNumber);
	// Additional support : Texture setters
	void setTextureID(GLint newTextureID);
};


// Can be used to print:
// PrintSpaced(1, 2, "three");
Planet::Planet(GLfloat size)
{
	planetSize = size;
	sphere = CreateSphere( size, 24, GL_TRIANGLE_STRIP,0.0, 2*M_PI, -M_PI/2, M_PI/2);
	rotationAngle = 0.0;
	translationAngle = 0.0;
	distance = 0.0;
	tiltAngle = 0.0;
	satellite = NULL;
	nextPlanet = NULL;
	mTextureID = 0;
}

//ADDITIONAL METHOD FOR EXTERNAL DATA SOURCE:
Planet::Planet(int officialPlanetNumber)
{

	DataSource * thisIslandEarth = new DataSource(officialPlanetNumber,"S");
	thisPlanetInfo = thisIslandEarth;
	distance = thisPlanetInfo->Distance;
	inclination = thisPlanetInfo->Inclination;
	planetName = thisPlanetInfo->planetName;
	rotationAngle= thisPlanetInfo->Rotation;
	rotationSpeed = thisPlanetInfo->RotationSpeed;
	tiltAngle = thisPlanetInfo->Tilt;
	planetSize = thisPlanetInfo->Size;
	translationAngle = thisPlanetInfo->Translation;
	translationSpeed = thisPlanetInfo->TranslationSpeed;
	planetCategory = thisPlanetInfo->planetCategory;

	//Need to configure this for our class based planet system:
	mTextureID = 0;


	//Declared vars.. Create planets
	sphere = CreateSphere( planetSize, 24, GL_TRIANGLE_STRIP,0.0, 2*M_PI, -M_PI/2, M_PI/2);
	satellite = NULL;
	nextPlanet = NULL;
	//Added code to here; compiles just peachey!
	//Added self induced material selection:
	this->setMaterial(thisIslandEarth->ambient, thisIslandEarth->diffuse, thisIslandEarth->specular, thisIslandEarth->shininess);
}


void Planet::setProgram(GLint newProgram)
{
	program = newProgram;
	mAmbientID = glGetUniformLocation(program, "MaterialAmbient");
	mDiffuseID = glGetUniformLocation(program, "MaterialDiffuse");
	mSpecularID = glGetUniformLocation(program, "MaterialSpecular");
	mShininessID = glGetUniformLocation(program, "Shininess");
	modelView = glGetUniformLocation( program, "ModelView" );

	mTextureID = glGetUniformLocation( program, "texture" );
}

void Planet::setMaterial(color4 theAmbient, color4 theDiffuse, color4 theSpecular, GLfloat theShininess)
{
	ambient = theAmbient;
	diffuse = theDiffuse;
	specular = theSpecular;
	shininess = theShininess;
}

void Planet::addPlanet(Planet * newPlanet)
{
	if(nextPlanet)
	{
		nextPlanet->addPlanet(newPlanet);
	}
	else
	{
		nextPlanet = newPlanet;
	}

}
// Set up planetoid texture:

void Planet::setTextureID(GLint newTextureID)
{
	textureID = newTextureID;
}

void Planet::addSatellite(Planet * newSatellite)
{
	if(satellite)
	{
		satellite->addPlanet(newSatellite);
	}
	else
	{
		satellite = newSatellite;
	}
	z

}
Planet * Planet::getPlanet(GLint index)
{
	if(index == 0)
	{
		return this;
	}
	else
	{
		if(nextPlanet)
		{
			return nextPlanet->getPlanet(index - 1);
		}
		else
		{
			return NULL;
		}
	}
}

Planet * Planet::getSatellite(GLint index)
{
	if(satellite)
		return satellite->getPlanet(index);
	else
		return NULL;
}

void Planet::update()
{
	rotationAngle += rotationSpeed;
	translationAngle += translationSpeed;
	if(satellite)
	{
		satellite->update();
	}
	if(nextPlanet)
	{
		nextPlanet->update();
	}
}

void Planet::draw(mat4 * ModelView, mat4 * cummulative)
{
	glUseProgram(program);
	//Added texture item:
	glUniform1i( mTextureID, 0);

	mat4 rotationMatrix = RotateY(rotationAngle);
	mat4 translationMatrix = RotateY(translationAngle);
	mat4 distanceMatrix = Translate(distance, 0.0, 0.0);
	mat4 tiltMatrix = RotateZ(tiltAngle);
	mat4 inclinationMatrix = RotateX(inclination);
	mat4 furtherCummulative = *cummulative * inclinationMatrix * translationMatrix * distanceMatrix * tiltMatrix * rotationMatrix;
	mat4 newModelView = *ModelView * furtherCummulative; 
	glUniformMatrix4fv( modelView, 1, GL_TRUE, newModelView );
	//this;

	//New code to try to do the inverse that was in the shader:

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf( (GLfloat *)&newModelView );

	position = furtherCummulative * point4(0,0,0,1);

	glUniform4fv( mAmbientID, 1, ambient );
	glUniform4fv( mDiffuseID, 1, diffuse );
	glUniform4fv( mSpecularID, 1, specular );
	glUniform1f( mShininessID, shininess );

	//TextureRific:
	//1. Enable the texture, then activate the texture, then bind the texture to this drawing system.
	glEnable(GL_TEXTURE_2D);
	glActiveTexture( GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Bind each texture to the sphere:
	int currentVertex = sphere.firstPoint;
	for(int i = 0; i < sphere.numStrips; i++)
	{
		glDrawArrays( sphere.mode, currentVertex, sphere.pointsPerStrip);
		currentVertex += sphere.pointsPerStrip;
	}
	//and disable the binding of textures:
	glDisable(GL_TEXTURE_2D);

	if(satellite)
	{
		satellite->draw(ModelView, &furtherCummulative);
	}
	if(nextPlanet)
	{
		nextPlanet->draw(ModelView, cummulative);
	}

}

Planet  * sun, * target;
//Manual initialization for debug:
Planet * mercury = new Planet(1);
Planet * venus = new Planet(2);
Planet * earth = new Planet(3);
Planet * mars = new Planet(5);
Planet * jupiter = new Planet(8);
Planet * saturn = new Planet(13);
Planet * uranus = new Planet(21);
Planet * neptune = new Planet(22);
Planet * pluto = new Planet(23);

//Moons: Manual debug. Loops don't work like the do elsewhere. C++ is a strange land indeed.
// Why don't they work?!?! Whatever, unrolling my loops seems to make everyone happy.
	//lets add some manual moons:
	Planet *theMoon = new Planet(4);
	Planet *charon = new Planet(24);
	Planet *phobos = new Planet(6);
	Planet *deimos = new Planet(7);
	Planet *io = new Planet(9);//I no longer like jupiter.
	Planet *europa = new Planet(10);
	Planet *ganymede = new Planet(11);
	Planet *callisto = new Planet(12);

	// Moons o saturn
static	Planet *titan = new Planet(14);
static	Planet *iapetus = new Planet(15);
static	Planet *rhea = new Planet(16);
static	Planet *mima = new Planet(17);
static	Planet *enceladus = new Planet(18);
static	Planet *tethys = new Planet(19);
static	Planet *dione = new Planet(20);


static Planet* jupiterMoons[4] = 
{ io, europa, ganymede, callisto
};

static Planet *marsMoons[2] =
{
	phobos, deimos
};

static Planet *saturnMoons[7] =
{
	titan, iapetus, rhea, mima, enceladus, tethys, dione
};
//and some C++ magic:
static int jupiterMoonCount = 4;
static Planet **jupiterMoon = &jupiterMoons[0];
static int currentJupiterMoon = 0;
static int currentMarsMoon = 0;
static int currentSaturnMoon = 0;


///Our space ship is on Earth by default:
Planet * planetOfLanding = earth;

//----------------------------------------------------------------------------

// OpenGL initialization
// Create a solar system. Since the moon, earth, and the sun are pre-created - first, we test the method.
void
	init()
{
	printf("SUN LOAD");
	///TEXTURES:
	GLuint sunTexture = SOIL_load_OGL_texture
	(
		"earth.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_POWER_OF_TWO
		| SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_INVERT_Y
		| SOIL_FLAG_MULTIPLY_ALPHA
		| SOIL_FLAG_DDS_LOAD_DIRECT
	);
	printf("EARTH LOAD");
	GLuint earthTexture = SOIL_load_OGL_texture
	(
		"earth.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_POWER_OF_TWO
		| SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_INVERT_Y
		| SOIL_FLAG_MULTIPLY_ALPHA
		| SOIL_FLAG_DDS_LOAD_DIRECT
	);
	//AFTER YOU LOAD ALL THE TEXTURES, WE HAVE TO SET UP THE ACTIVE TEXTURES:
	glEnable(GL_TEXTURE_2D); //ENABLE TEXTURE MODE
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sunTexture);
	// Give it a name! You enable it, you bind it, then you activate it.
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_2D, earthTexture);

	// Give it a name! You enable it, you bind it, then you activate it.
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	glDisable(GL_TEXTURE_2D);
	
	
	//Moved the shading setup to the beginning, right after we create the sun. This lets me
	//assign shaders to each planet.

	sun = new Planet(0);
	sun->textureID = sunTexture;
	/////////////////// New Shading Shader ////////////////////////////
	// Load shaders and use the resulting shader program
	GLuint program = InitShader( "vShader.glsl", "fShaderTest.glsl" );
	Program = program;
	sun->setProgram(program);
	// have to talk to the sun to set the proper texture!!!!!!!
	



	//Manual Code Replacements
	//Planet * currentPlanet = new Planet(3);
	//Planet * currentSatellite = new Planet(4);



	/*
	sun->satellite = currentPlanet;
	currentPlanet->satellite = currentSatellite;
	*/
	//New code - done in class to take advantage of the linked list Dr. Obando createD:
	/*
	sun->addSatellite(currentPlanet);
	currentPlanet->addSatellite(currentSatellite);
	*/


	//Loop Code to init polanets:

	//Planet* planetArray = new Planet[24];


	int i = 0;
	Planet* currentPlanet = sun;
	// Lets init the array first:
	//std::vector<Planet> solarSystem;
	//solarSystem.begin();


	//and populate the solar system!
	/* Loop disabled for debug purposes.
	while (i < 24)
	{
	Planet * newPlanet = new Planet(i);	
	//For the time being, everyone has the same shader:
	//MegaWeapon -- SHADER SETUP!!!
	//Don't panic. Add this to the datasource.h file? nop4/
	if(newPlanet->thisPlanetInfo->program)
	{
	newPlanet->setProgram(newPlanet->thisPlanetInfo->program);
	}
	else
	{
	newPlanet->setProgram(program);
	}

	//Next set up the materials. Right now its a simple switch case thing:
	//Included inside the planet creation code:

	if(newPlanet->planetCategory == 'M')
	{
	// Its no space station
	currentPlanet->addSatellite(newPlanet);
	currentPlanet->setMaterial(earthAmbient, earthDiffuse, earthSpecular, earthShininess);

	}
	else
	{
	currentPlanet->setMaterial(moonAmbient, moonDiffuse, moonSpecular, moonShininess);
	sun->addSatellite(newPlanet);
	currentPlanet = (newPlanet);
	}
	i ++;
	} */


	//Manual addition of planets for debug:
	sun->addSatellite(mercury);
	sun->addSatellite(venus);
	sun->addSatellite(earth);
	sun->addSatellite(mars);
	sun->addSatellite(jupiter);
	sun->addSatellite(saturn);
	sun->addSatellite(uranus);
	sun->addSatellite(neptune);
	sun->addSatellite(pluto);

	//Ugh lets add the moons now.
	for(int j = 0; i < 4; i ++)
		jupiter->addSatellite(jupiterMoons[i]);

	//saturn moons:
	//for(int j = 0; i <7; i++)
//		saturn->addSatellite(saturnMoons[i]);
	saturn->addSatellite(titan);
	saturn->addSatellite(iapetus);
	saturn->addSatellite(rhea);
	saturn->addSatellite(mima);
	saturn->addSatellite(enceladus);
	saturn->addSatellite(tethys);
	saturn->addSatellite(dione);
	



	theMoon->setProgram(program);
	phobos->setProgram(program);
	deimos->setProgram(program);
	io->setProgram(program);
	europa->setProgram(program);
	ganymede->setProgram(program);
	callisto->setProgram(program);
	titan->setProgram(program);
	iapetus->setProgram(program);
	rhea->setProgram(program);
	mima->setProgram(program);
	enceladus->setProgram(program);
    tethys->setProgram(program);
	dione->setProgram(program);
	charon->setProgram(program);
	earth->addSatellite(theMoon);
	pluto->addSatellite(charon);

	mars->addSatellite(phobos);
	mars->addSatellite(deimos);

	Planet *moonSystem[16] = {theMoon, charon ,phobos ,deimos,io ,europa ,ganymede ,callisto, titan, iapetus, rhea,
		mima, enceladus, tethys, dione};
	Planet *solarSystem[9] = {mercury, venus, earth, mars, jupiter ,saturn,uranus,neptune,pluto};
	//preBuilt program-o-meter:
	for(int i = 0; i < 9; i ++)
	{
		if(solarSystem[i]->thisPlanetInfo->program)
		{
			solarSystem[i]->setProgram(solarSystem[i]->thisPlanetInfo->program);
		}
		else
		{
			solarSystem[i]->setProgram(program);
			printf("ERROR ! UNDEFINED COLOR PROGRAM");
		}

	}	//We have new planet!


	//***TARGETING SYSTEM ********************************************************************************************
	// This is where the rendering target is pointing:
	//OLD:
	//target = sun;
	target = sun;
	//***TARGETING SYSTEM ********************************************************************************************




	// ***** END OF NEW CODE ATM **
	sun->rotationSpeed = 0.0;
	sun->distance = 0.0;
	sun->translationSpeed = 0.0;
	sun->tiltAngle = 0.0;
	sun->inclination = 0.0;

	/* OLD EARTH AND MOON CODE:
	currentPlanet->rotationSpeed = earthRotationSpeed;
	currentSatellite->rotationSpeed = moonRotationSpeed;


	currentPlanet->translationSpeed = earthTranslationSpeed;
	currentSatellite->translationSpeed = moonTranslationSpeed;



	currentSatellite->distance = moonDistance;
	currentPlanet->distance = earthDistance;


	currentPlanet->tiltAngle = earthTilt;
	currentSatellite->tiltAngle = moonTilt;
	//Added again:


	currentPlanet->inclination = 45.0;//earthInclination;
	currentSatellite->inclination = moonInclination;
	*/




	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers( 1, &buffer );
	glBindBuffer( GL_ARRAY_BUFFER, buffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals) + sizeof(texCoords),
		NULL, GL_STATIC_DRAW );
	//glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals),
	//	NULL, GL_STATIC_DRAW ); // changed to display textures!
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points),  sizeof(normals), normals );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals), sizeof(normals), texCoords);
	// In this code, we init the buffer data -> we put in the points from the
	// first pset of points, then we add the next set of normals, starting at the size
	// of points, then we add data from size of points + size of normals, then we start at
	// size of points + size of normals + size of textures

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0) );

	GLuint vNormal = glGetAttribLocation( program, "vNormal" ); 
	glEnableVertexAttribArray( vNormal );
	glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)) );

	// Arrays of stuff for textures
	GLuint vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord);
	glVertexAttribPointer (vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points) + sizeof(normals)));

	glUniform4fv( glGetUniformLocation(program, "LightAmbient"),
		1, light_ambient );
	glUniform4fv( glGetUniformLocation(program, "LightDiffuse"),
		1, light_diffuse );
	glUniform4fv( glGetUniformLocation(program, "LightSpecular"),
		1, light_specular );

	glUniform4fv( glGetUniformLocation(program, "LightPosition"),
		1, light_position );

	// Retrieve transformation uniform variable locations
	ModelView = glGetUniformLocation( program, "ModelView" );
	Projection = glGetUniformLocation( program, "Projection" );

	sun->setMaterial(material_ambient, material_diffuse, material_specular, material_shininess);





	glEnable( GL_DEPTH_TEST );
	at = point4(0.0, 0.0, 0.0, 1.0);
	eye = point4(2.0, 0.0, 0.0, 1.0);

	glClearColor( 0.2, 0.2, 0.2, 1.0 ); /* gray background */
}

//----------------------------------------------------------------------------
///**********Heads up display:
void 
	displayHeadsUp(int value)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, ww, 0.0, wh);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(0.0,0.0,1.0);
	glRasterPos2i(5,5);

	string s = (target->planetName);
	void * font = GLUT_BITMAP_HELVETICA_18;
	
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char *)  "Current Target: "); 

	for (string::iterator i = s.begin(); i != s.end(); ++i)
	{
		
		char c = *i;
		glColor3f(0.0,0.0,1.0);
		glutBitmapCharacter(font, c);
	}
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	//glutTimerFunc(1,displayHeadsUp,-1);
}

//Camera Lens Angle Adjustor:
void
	adjustCameraAngle(int value)
{
}



void
	display( void )
{
	displayHeadsUp(-1);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	vec4   up( 0.0, 1.0, 0.0, 0.0 );

	mat4 model_view = LookAt( eye, at, up );
	mat4 initial = mat4(1.0);
	sun->draw(&model_view, &initial);
	
	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
	moveCamera()
{
	t += 0.01;
	eye.x = cameraDistance * cos(t);
	eye.z = cameraDistance * sin(t);
	at = target->position;
	if (spaceShipWarp == true)
	{
		GLfloat x = planetOfLanding->position.x;
		GLfloat y = planetOfLanding->position.y;
		GLfloat z = planetOfLanding->position.z;
		x += planetOfLanding->planetSize;
		y += planetOfLanding->planetSize;
		z += planetOfLanding->planetSize;
		eye.x = x ;
		eye.y = y;//planetOfLanding->(position.y+ planetOfLanding->size);
		eye.z = z;//planetOfLanding->(position.z+ planetOfLanding->size);
		
		//eye = planetOfLanding->position;

	}
	
}


//----------------------------------------------------------------------------

void
	animate(int value)
{
	//Heads up display:
	
	moveCamera();
	sun->update();
	displayHeadsUp(-1);
	glutTimerFunc(33, animate, -1);
	
	glutPostRedisplay();
}
//----------------------------------------------------------------------------
void
	adjustOrtho()
{
	glViewport( 0, 0, ww, wh );

	GLfloat left = -2.0, right = 2.0;
	GLfloat top = 2.0, bottom = -2.0;
	GLfloat zNear = -20.0, zFar = 20.0;

	GLfloat aspect = GLfloat(ww)/wh;

	if ( aspect > 1.0 ) {
		left *= aspect;
		right *= aspect;
	}
	else {
		top /= aspect;
		bottom /= aspect;
	}

	mat4 projection = Ortho( left, right, bottom, top, zNear, zFar );
	//projection = Ortho2D( -2.0, 2.0, -2.0, 2.0);
	projection = Perspective(lens, aspect, 0.1, 1000);
	glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );
}
void menuItemHandler(int ID)
{
	switch (ID)
	{
	case 0:
		planetOfLanding = sun;
		spaceShipWarp = true;
		break;
		case 1:
		planetOfLanding = mercury;
		spaceShipWarp = true;
		break;
		case 2:
		planetOfLanding = venus;
		spaceShipWarp = true;
		break;
		case 3:
		planetOfLanding = earth;
		spaceShipWarp = true;
		break;
		case 4:
		planetOfLanding = mars;
		spaceShipWarp = true;
		break;
		case 5:
		planetOfLanding = jupiter;
		spaceShipWarp = true;
		break;
		case 6:
		planetOfLanding = saturn;
		spaceShipWarp = true;
		break;
		case 7:
		planetOfLanding = uranus;
		spaceShipWarp = true;
		break;
		case 8:
		planetOfLanding = neptune;
		spaceShipWarp = true;
		break;
		case 9:
		planetOfLanding = pluto;
		spaceShipWarp = true;
		break;
		case 102:
		lens <= 0 ? lens = 0 : lens -= 10 ;
		adjustOrtho();
		glutPostRedisplay();
		break;
		case 101:
		lens <= 200 ? lens += 10 : lens = 200;
		adjustOrtho();
		glutPostRedisplay();
		break;

	}

}

void marsItemHandler(int ID)
{
	planetOfLanding = marsMoons[ID];
	spaceShipWarp =  true;
}
void jupiterItemHandler(int ID)
{
	planetOfLanding = jupiterMoons[ID];
	spaceShipWarp = true;
}
void saturnItemHandler(int ID)
{
	planetOfLanding = saturnMoons[ID];
	spaceShipWarp = true;
}
//********************************************************************//********************************************************************
//********************************************************************Mouse Menu:
// Since the keyboard handles the targeting system, this menu will allow the user to select camerea lens angles and set the viewing from.
void buildMenus()
{
	//Great, so I have to do submenus because I'm a completionist:
	GLuint earthSubMenu = glutCreateMenu(menuItemHandler);
	glutAddMenuEntry("The Moon!", 14);
	GLuint marsSubMenu = glutCreateMenu(marsItemHandler);
	for(int i =0; i < 2; i ++) {
		string myString = marsMoons[i]->planetName;
		const char *p = myString.c_str();
		glutAddMenuEntry(p,i);
	}
		GLuint jupiterSubMenu = glutCreateMenu(jupiterItemHandler);
	for(int i =0; i < 4; i ++) {
		string myString = jupiterMoons[i]->planetName;
		const char *p = myString.c_str();
		glutAddMenuEntry(p,i);
	}
		GLuint saturnSubMenu = glutCreateMenu(saturnItemHandler);
	for(int i =0; i < 7; i ++) {
		string myString = marsMoons[i]->planetName;
		const char *p = myString.c_str();
		glutAddMenuEntry(p,i);
	}
	/////////

	GLuint menu = glutCreateMenu(menuItemHandler);
	glutAddMenuEntry("Change your stellar position:",9999999);
	glutAddMenuEntry("Sun",0);
	glutAddMenuEntry("Mercury", 1);
	glutAddMenuEntry("Venus",2);
	glutAddMenuEntry("Mars",3);
	glutAddMenuEntry("Jupiter",4);
	glutAddMenuEntry("Saturn",5);
	glutAddMenuEntry("Uranus",6);
	glutAddMenuEntry("Neptune",7);
	glutAddMenuEntry("Pluto",8);
	/// Planets above, moons below:
	glutAddSubMenu("Moons of Earth", earthSubMenu);
	glutAddSubMenu("Moons of Mars", marsSubMenu);
	glutAddSubMenu("Moons of Jupiter", jupiterSubMenu);
	glutAddSubMenu("Moons of Saturn", saturnSubMenu);
	glutAddMenuEntry("Change Camera Angle:",9999);
	glutAddMenuEntry("Press [ or ] for fine lens angle control, or select from below:", 8888);
	glutAddMenuEntry("Increase Camera Angle by 10", 101);
	glutAddMenuEntry("Decrease Camera Angle by 10", 102);

	glutAttachMenu(GLUT_RIGHT_BUTTON);


}

//----------------------------------------------------------------------------
//Quck and dirty planet switching. I will enhance this if time permits:
int moonIndex = 0;

void keyboard( unsigned char key, int x, int y )
{
	switch( key ) {

		//Added some lens angle stuff here:
	case '[':
		lens <= 0 ? lens = 0 : lens -- ;
		adjustOrtho();
		glutPostRedisplay();
		break;
	case ']':
		lens <= 200 ? lens++ : lens = 200;
		adjustOrtho();
		glutPostRedisplay();
		break;

	case 033: // Escape Key
	case 'q': case 'Q':
		exit( EXIT_SUCCESS );
		break;
	case 'd': case 'D':
		light_position.w = 0.0;
		glUniform4fv( glGetUniformLocation(Program, "LightPosition"), 1, light_position );
		break;
	case 'p': case 'P':
		light_position.w = 1.0;
		glUniform4fv( glGetUniformLocation(Program, "LightPosition"), 1, light_position );
		break;
	case 'f': case 'F':
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_POINT);
		break;
	case 'w': case 'W':
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
		break;
	case '+':
		cameraDistance += 0.1;
		break;
	case '-':
		cameraDistance -= 0.1;
		break;
	case '0':
		target = sun;
		break;
	case '1':
		target = mercury;
		break;
	case '2':
		target = venus;
		break;
	case '4':
		target = mars;
		break;
	case '$':
		if (currentMarsMoon > 1)
			currentMarsMoon = 0;
		target = marsMoons[currentMarsMoon];
		currentMarsMoon ++;
		break;



	case '3':
		target = earth;
		break;
	case '#':
		target = theMoon;
		break;
	case '5':
		target = jupiter;
		break;
	case '%':
		if (currentJupiterMoon >  3)
			currentJupiterMoon = 0;
		target = jupiterMoons[currentJupiterMoon];
		currentJupiterMoon ++;
		break;

	case '6':
		target = saturn;
		break;
	case '^':
		if (currentSaturnMoon > 6)
			currentSaturnMoon = 0;
		target = saturnMoons[currentSaturnMoon];
		currentSaturnMoon++;
		break;

	case '7':
		target = uranus;
		break;
	case '8':
		target = neptune;
		break;
	case '9':
		target = pluto;
		break;

	case '(':
		target = charon;
		break;
		
		//reset thingy:
		case'r':
		spaceShipWarp = false;


	//Quick and Dirty planetary selection tool:
		case '`':
			if (target != sun)
			{
				if (target->planetCategory = 'P')
				{
					if(target->getSatellite(moonIndex))
					{
					
						target = target->getSatellite(moonIndex);
					}
					else
					{
						moonIndex = 0;
						if(target->getSatellite(moonIndex)) 
							target = target->getSatellite(moonIndex);
						else
							target = sun;
					}
				}
				else
				{//We are already in some moon action here:
					target=mars;
				
				}
			}
			else
				(target = earth->satellite);


	}
	//and update the other thing:
	
	
}

//----------------------------------------------------------------------------
void
	reshape( int width, int height )
{
	ww = width;
	wh = height;

	adjustOrtho();

}

//----------------------------------------------------------------------------

int
	main( int argc, char **argv )
{

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize( 512, 512 );
	glutInitContextVersion( 3, 0 );
	//glutInitContextProfile( GLUT_COMPATIBILITY_PROFILE );
	glutCreateWindow( "Viewports" );

	init();
	buildMenus();

	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	glutKeyboardFunc( keyboard );
	glutTimerFunc( 33, animate, -1);

	glutMainLoop();
	return 0;
}
