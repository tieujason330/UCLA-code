//
// raytrace.cpp
//

#define _CRT_SECURE_NO_WARNINGS
#include "matm.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int g_width;
int g_height;

struct Ray
{
	vec4 origin;
	vec4 dir;
};

// TODO: add structs for spheres, lights and anything else you may need.
struct Color
{
	float r, g, b;
};

struct Scaling
{
	float x, y, z;
};

//struct for sphere
struct Sphere
{
	string name;
	//position
	vec4 position;
	//scaling (non-uniform)
	vec4 scaling;
	//color
	Color color;
	//(specular) surface reflectance coefficient
	float ka, kd, ks, kr;
	//shininess factor
	float spec_exp;
};

//struct for light
struct Light
{
	string name;
	//pos
	vec4 position;
	//intensity
	Color intensity;
};

vector<vec4> g_colors;

float g_left;
float g_right;
float g_top;
float g_bottom;
float g_near;

vec4 m_ambient;
vec4 m_background;
string m_output_name;

//up to 5 Sphere/Light objects so using vector
const int MAX_SPHERES = 5, MAX_LIGHTS = 5;
Sphere m_spheres[MAX_SPHERES];
Light m_lights[MAX_LIGHTS];
int sphere_count = 0;
int light_count = 0;

// -------------------------------------------------------------------
// Input file parsing

vec4 toVec4(const string& s1, const string& s2, const string& s3)
{
	stringstream ss(s1 + " " + s2 + " " + s3);
	vec4 result;
	ss >> result.x >> result.y >> result.z;
	result.w = 1.0f;
	return result;
}

float toFloat(const string& s)
{
	stringstream ss(s);
	float f;
	ss >> f;
	return f;
}

void parseLine(const vector<string>& vs)
{
	//TODO: add parsing of NEAR, LEFT, RIGHT, BOTTOM, TOP, SPHERE, LIGHT, BACK, AMBIENT, OUTPUT.
	//for resolution
	if (vs[0] == "RES")
	{
		g_width = (int)toFloat(vs[1]);
		g_height = (int)toFloat(vs[2]);
		g_colors.resize(g_width * g_height);
	}
	//for NEAR
	if (vs[0] == "NEAR")
	{
		g_near = toFloat(vs[1]);
	}
	//for LEFT
	if (vs[0] == "LEFT")
	{
		g_left = toFloat(vs[1]);
	}
	//for RIGHT
	if (vs[0] == "RIGHT")
	{
		g_right = toFloat(vs[1]);
	}
	//for BOTTOM
	if (vs[0] == "BOTTOM")
	{
		g_bottom = toFloat(vs[1]);
	}
	//for TOP
	if (vs[0] == "TOP")
	{
		g_top = toFloat(vs[1]);
	}
	//for SPHERE
	if (vs[0] == "SPHERE")
	{
		Sphere s;
		m_spheres[sphere_count].name = vs[1];

		m_spheres[sphere_count].position.x = toFloat(vs[2]);
		m_spheres[sphere_count].position.y = toFloat(vs[3]);
		m_spheres[sphere_count].position.z = toFloat(vs[4]);
		m_spheres[sphere_count].position.w = 1.0f;

		m_spheres[sphere_count].scaling.x = toFloat(vs[5]);
		m_spheres[sphere_count].scaling.y = toFloat(vs[6]);
		m_spheres[sphere_count].scaling.z = toFloat(vs[7]);

		// Import Color
		m_spheres[sphere_count].color.r = toFloat(vs[8]);
		m_spheres[sphere_count].color.g = toFloat(vs[9]);
		m_spheres[sphere_count].color.b = toFloat(vs[10]);

		m_spheres[sphere_count].ka = toFloat(vs[11]);
		m_spheres[sphere_count].kd = toFloat(vs[12]);
		m_spheres[sphere_count].ks = toFloat(vs[13]);
		m_spheres[sphere_count].kr = toFloat(vs[14]);
		m_spheres[sphere_count].spec_exp = toFloat(vs[15]);

		sphere_count++;
		//add to spheres vector
		//m_spheres.push_back(s);
	}
	//for LIGHT
	if (vs[0] == "LIGHT")
	{
		Light l;
		m_lights[light_count].name = vs[1];
		m_lights[light_count].position.x = toFloat(vs[2]);
		m_lights[light_count].position.y = toFloat(vs[3]);
		m_lights[light_count].position.z = toFloat(vs[4]);
		m_lights[light_count].position.w = 1.0f;

		m_lights[light_count].intensity.r = toFloat(vs[5]);
		m_lights[light_count].intensity.g = toFloat(vs[6]);
		m_lights[light_count].intensity.b = toFloat(vs[7]);

		light_count++;
		//add to lights vector
		//m_lights.push_back(l);
	}
	//for BACK
	if (vs[0] == "BACK")
	{
		m_background = toVec4(vs[1], vs[2], vs[3]);
	}
	//for AMBIENT
	if (vs[0] == "AMBIENT")
	{
		m_ambient = toVec4(vs[1], vs[2], vs[3]);
	}
	if (vs[0] == "OUTPUT")
	{
		m_output_name = vs[1];

	}
}

void loadFile(const char* filename)
{
	ifstream is(filename);
	if (is.fail())
	{
		cout << "Could not open file " << filename << endl;
		exit(1);
	}
	string s;
	vector<string> vs;
	while (!is.eof())
	{
		vs.clear();
		getline(is, s);
		istringstream iss(s);
		while (!iss.eof())
		{
			string sub;
			iss >> sub;
			vs.push_back(sub);
		}
		parseLine(vs);
	}
}

// -------------------------------------------------------------------
// Utilities

void setColor(int ix, int iy, const vec4& color)
{
	int iy2 = g_height - iy - 1; // Invert iy coordinate.
	g_colors[iy2 * g_width + ix] = color;
}

// -------------------------------------------------------------------
// Intersection routine

vec4 sphereintersection(vec4 ee, vec4 dd, int& index){
	index = 0; // set to 0 for no intersection initially
	float  t0, t1;
	float tmin = -1;
	vec4 e;
	vec4 d;
	
	//check each sphere
	for (int i = 0; i < sphere_count; i++) {
		// find matrix of sphere's trans
		//want Minv = Sinv * Tinv
		mat4 T = Translate(m_spheres[i].position);
		mat4 S = Scale(m_spheres[i].scaling.x, m_spheres[i].scaling.y, m_spheres[i].scaling.z);
		mat4 M = T * S;
		mat4 MInv;
		InvertMatrix(M, MInv);

		// inverse transformation of ray
		e.w = 1.0f; //first turn into point
		e = MInv * ee;
		e.w = 0.0f; //turn into vector

		d = MInv * dd;
		d.w = 0.0f; //turn into vector

		// Quadratic Equation
		float A = dot(d, d);
		float B = 2 * dot(e, d);
		float C = dot(e, e) - 1;
		float disc = (B*B) - (4*A*C);
		if (disc < 0.0) // no intersection
			continue;
		// 1 intersection
		else if (disc == 0.0) 
		{ 
			t0 = -B / (2 * A);
			if (t0 > 0 && (t0 < tmin)) 
			{
				index = 1 + i;
				tmin = t0;
			}
		}
		// 2 intersections
		else if (disc > 0.0) 
		{ 
			t0 = (-B + sqrt(disc)) / (2 * A);
			t1 = (-B - sqrt(disc)) / (2 * A);
			if (t0 < t1) 
			{
				if (t0 > 0 && (tmin == -1 || t0 < tmin)) 
				{
					index = 1 + i;
					tmin = t0;
				}
			}
			else 
			{
				if (t1 > 0 && (tmin == -1 || t1 < tmin)) 
				{
					index = i + 1;
					tmin = t1;
				}
			}
		}
	}

	vec4 closest_inter(ee + (dd *tmin));
	return closest_inter;
}

// -------------------------------------------------------------------

//local illumination
vec4 localIllum (vec4 intersection_pt, vec4 n, vec4 reflected, int index)
{
	vec4 v = normalize(intersection_pt);
	float ir = 0, ig = 0, ib = 0;
	int spot = index - 1;

	//get intensity from light sources that aren't blocked
	for (int i = 0; i < light_count; i++)
	{
		//apply local illum at intersection point if not blocked

		//find intersections with light srcs
		vec4 lsrc(m_lights[i].position.x, m_lights[i].position.y, m_lights[i].position.z, 1.0f);
		lsrc = lsrc - intersection_pt;
		lsrc.w = 0; //make it a vector
	
		vec4 nlsrc(normalize(lsrc));
		//if angle > 90 deg (inside sphere) do nothing
		if (dot(nlsrc, n) < 0)
			continue;

		int shadow;
		vec4 normls_orig(nlsrc);
		sphereintersection(intersection_pt, nlsrc, shadow);
		//no intersection so apply local illumination
		if (shadow == 0)
		{
			//find reflect vector
			vec4 refl((n*(2 * dot(n, nlsrc))) - (nlsrc));
			vec4 r = normalize(refl);
			//specular
			ir += m_lights[i].intensity.r * m_spheres[spot].ks * pow(dot(r, v), m_spheres[spot].spec_exp);
			ig += m_lights[i].intensity.g * m_spheres[spot].ks * pow(dot(r, v), m_spheres[spot].spec_exp);
			ib += m_lights[i].intensity.b * m_spheres[spot].ks * pow(dot(r, v), m_spheres[spot].spec_exp);

			//diffuse
			ir += m_lights[i].intensity.r * m_spheres[spot].kd * dot(n, normls_orig) * m_spheres[spot].color.r;
			ig += m_lights[i].intensity.g * m_spheres[spot].kd * dot(n, normls_orig) * m_spheres[spot].color.g;
			ib += m_lights[i].intensity.b * m_spheres[spot].kd * dot(n, normls_orig) * m_spheres[spot].color.b;
			}
	}
	vec4 colors(ir, ig, ib, 0.0f);
	return colors;
}

//normal
vec4 normal(vec4 i, int& index)
{
	//want Minv = invS * invT
	mat4 T = Translate(m_spheres[index - 1].position);
	mat4 S = Scale(m_spheres[index - 1].scaling.x, m_spheres[index - 1].scaling.y, m_spheres[index - 1].scaling.z);
	mat4 M = T * S;
	mat4 MInv;
	InvertMatrix(M, MInv);
	mat4 MInvT;
	MInvT = transpose(MInv);

	i.w = 1.0f;
	i = MInv * i;
	i.w = 1.0f; //make sure is a point

	vec4 r = MInvT * normalize(i);
	r.w = 0; //make it into a vector
	return normalize(r);
}

// Ray tracing
vec4 trace(Ray& ray, int counter)
{
	vec4 zero(0.0f, 0.0f, 0.0f, 0.0f);

	// TODO: implement your ray tracing routine here.
	if (counter > 1)
		return zero;

	int index;

	//first we find the closest intersection point
	vec4 intersection_pt = sphereintersection(ray.origin, ray.dir, index);

	//no intersection then return background color
	if (index == 0)
	{
		if (counter > 0)
			return zero;
		return m_background;
	}
	//light source
	else if (index >= 10)
	{
		int temp = index - 10;
		vec4 l(m_lights[temp].intensity.r, m_lights[temp].intensity.g, m_lights[temp].intensity.b, 1.0f);
		return l;
	}

	//get actual index
	int spot = index - 1;

	vec4 colors(m_spheres[spot].color.r, m_spheres[spot].color.g, m_spheres[spot].color.b, 1.0f);

	//add ambient intensity
	colors *= m_ambient *(m_spheres[spot].ka);

	//find normal ray
	vec4 r;
	vec4 n = normal(intersection_pt, index);
	
	vec4 color_reflected;
	//find ref vec from each light src
	for (int i = 0; i < light_count; i++)
	{
		//light source
		vec4 lsrc(m_lights[i].position.x, m_lights[i].position.y, m_lights[i].position.z, 1.0f);
		lsrc = lsrc - intersection_pt;
		lsrc.w = 0; //make into vector

		vec4 nlsrc(normalize(lsrc));
		//angle is > 90 deg, lightsrc ray goes through sphere
		if (dot(nlsrc, n) < 0)
			continue; //no reflection from this light src
		//reflect  vector
		vec4 refl((n*(2 * dot(n, nlsrc)))-nlsrc);
		r =  normalize(refl);

		Ray ref_ray;
		ref_ray.origin += intersection_pt;
		ref_ray.dir += r;
		color_reflected = trace(ref_ray, counter + 1);
		colors += color_reflected * m_spheres[spot].kr;
	}

	//diffuse and specular illum - local illumination
	vec4 local_color = localIllum(intersection_pt, n, r, index);

	colors += local_color;
	colors.w = 0; //make it into a vector

	return colors;
}

vec4 getDir(int ix, int iy)
{
	// TODO: modify this. This should return the direction from the origin
	// to pixel (ix, iy), normalized.
	
	vec4 dir;
	float uc = g_left + ((float)ix / (g_width - 1)) * (g_right - g_left);
	float vr = g_bottom + ((float)iy / (g_height - 1)) * (g_top - g_bottom);
	dir = vec4(uc, vr, -g_near, 0.0f);
	
	return normalize(dir);
}

//(1a)determine ray from eye through pixel
void renderPixel(int ix, int iy)
{
	Ray ray;
	ray.origin = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	ray.dir = getDir(ix, iy);
	vec4 color = trace(ray, 0);

	//cout << color << endl;
	//cout << color.x << "," << color.y << "," << color.z << endl;
	setColor(ix, iy, color);
}

//(1)for each pixel on screen
void render()
{
	for (int iy = 0; iy < g_height; iy++)
	for (int ix = 0; ix < g_width; ix++)
		renderPixel(ix, iy);
}

// -------------------------------------------------------------------
// PPM saving

void savePPM(int Width, int Height, char* fname, unsigned char* pixels)
{
	FILE *fp;
	const int maxVal = 255;

	printf("Saving image %s: %d x %d\n", fname, Width, Height);
	fp = fopen(fname, "wb");
	if (!fp) {
		printf("Unable to open file '%s'\n", fname);
		return;
	}
	fprintf(fp, "P6\n");
	fprintf(fp, "%d %d\n", Width, Height);
	fprintf(fp, "%d\n", maxVal);

	for (int j = 0; j < Height; j++) {
		fwrite(&pixels[j*Width * 3], 3, Width, fp);
	}

	fclose(fp);
}

void saveFile()
{
	// Convert color components from floats to unsigned chars.
	// TODO: clamp values if out of range.
	unsigned char* buf = new unsigned char[g_width * g_height * 3];
	for (int y = 0; y < g_height; y++)
	for (int x = 0; x < g_width; x++)
	for (int i = 0; i < 3; i++){
		float pc = ((float*)g_colors[y*g_width + x])[i];
		if (pc > 1.0)
			pc = 1.0;
		buf[y*g_width * 3 + x * 3 + i] = (unsigned char)(pc * 255.9f);
	}
	
	// TODO: change file name based on input file name
	//converts output name to char pointer
	char* c = &m_output_name[0];
	savePPM(g_width, g_height, c, buf);
	delete[] buf;
}

/*
//Debug
void debug()
{
cout << "NEAR " << g_near << endl;
cout << "LEFT " << g_left << endl;
cout << "RIGHT " << g_right << endl;
cout << "BOTTOM " << g_bottom << endl;
cout << "TOP " << g_top << endl;
cout << "RES " << g_width << " " << g_height << endl;
for (int i = 0; i < sphere_count; i++)
{
cout << "SPHERE " << m_spheres[i].name << " " << m_spheres[i].position << " " << m_spheres[i].scale << " " << m_spheres[i].color
<< " " << m_spheres[i].ka << " " << m_spheres[i].kd << " " << m_spheres[i].ks << " " << m_spheres[i].kr
<< " " << m_spheres[i].spec_exp << endl;
}
for (int y = 0; y < light_count; y++)
{
cout << "LIGHT " << m_lights[y].name << " " << m_lights[y].position << " " << m_lights[y].intensity << endl;
}
cout << "BACK " << m_back.r << " " << m_back.g << " " << m_back.b << endl;
cout << "AMBIENT " << m_ambient.Ir << " " << m_ambient.Ig << " " << m_ambient.Ib << endl;
cout << "OUTPUT " << m_output_name << endl;
}
*/

// -------------------------------------------------------------------
// Main

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "Usage: template-rt <input_file.txt>" << endl;
		exit(1);
	}
	loadFile(argv[1]);
	//debug();
	render();
	saveFile();
	//system("pause");
	return 0;
}

