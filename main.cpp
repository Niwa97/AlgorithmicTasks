#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <string.h>

//1.********************************************************************************************************************************************************

struct vec3 //operator = is deafult
{ 

float x,y,z;

vec3(){ 
x = 0;
y = 0;
z = 0;
}

vec3(float X, float Y, float Z):x(X), y(Y), z(Z) {} 
};

vec3 operator+(vec3 v, vec3 w){ //adding vectors
 vec3 u(v.x+w.x,v.y+w.y,v.z+w.z);
return u;
}

vec3 operator-(vec3 v, vec3 w){ //subracting vectors
 vec3 u(v.x-w.x,v.y-w.y,v.z-w.z);
return u;
}

vec3 operator*(float a, vec3 v){ //multiplying by scalar
vec3 u(a*v.x,a*v.y,a*v.z);
return u;
}

float operator*(vec3 v, vec3 w){ //inner procuct
	return v.x*w.x+v.y*w.y+v.z*w.z;
}

vec3 operator^(vec3 v, vec3 w){ //croos procuct 
vec3 u(v.y*w.z - v.z*w.y, v.z*w.x - v.x*w.z, v.x*w.y - v.y*w.x);
return u;
}

float norm(vec3 v){ // returns norm of vector
	return sqrt(pow(v.x,2) + pow(v.y,2) + pow(v.z,2));
}

float cosangle(vec3 v, vec3 w){ //returns cosinus of angle between ponts 
	return (v*w/(norm(v)*norm(w)));
}

float parametercnt(float A, float B, float C, float D, vec3 start, vec3 direction){ //counts parmeter t which is needed for finding intersection of plane and halfline
	vec3 n(A,B,C); //normal vector of plane
	vec3 temp(1,1,1);
	float t = (D - n*start )/(temp*direction);
	if(t>0){ //we are not interested in points in oposite direction than v
		return t;
	}
	else{
		return 0;
	}
}

vec3 intersect(float t, vec3 start, vec3 direction){ //gives the point of intersection

if(t==0){ // if halfline does not intersect we do not count this sollution 
	vec3 N(0,0,0);
	return N;
}
vec3 searched = start + t*direction;
 return searched;	
}

// Implement this function for the following shapes:
struct Sphere
{
     vec3 center;
     float radius;
};

// Axis-aligned bounding box. Cube with edges aligned to x,y,z axis.
struct Aabb
{
     vec3 min;
     vec3 max;
};

// Triangular pyramid.
struct Tetrahedron
{
     vec3 points[4];
};

// Cylinder with hemispherical ends.
struct Capsule
{
     vec3 points[2];
     float radius;
};

vec3 support(vec3 v, Sphere S){
	if(norm(v) == 0){
		return v;
	}
	vec3 n = (S.radius/(norm(v)))*v; //"normalized" to the length of radius vector v
	return n + S.center;  // we translate normalized v to center of sphere
}



vec3 support(vec3 v, Aabb a){ //i assume that first vector is diagonal of base and second iss height of cuboid
	if(norm(v) == 0){
		return v;
	}
	vec3 diag = a.max + a.min; //center of diagonal of cuboid
	vec3 center = 0.5*diag; //center of diagonal of cuboid
	vec3 arr[6]; //array of vectors
	float array[6]; //array of norms 
	
	//we check all possible planes z = 0, z = diag.z, y = 0, y = diag.y etc. Then we chhose one wector of smallest nonzero norm. If we "hit" edge or vertex point is counted twice ore trree times   
	float t1, t2, t3, t4, t5, t6;
	
	t1 = parametercnt(0,0,1,0,center, v);
	t2 = parametercnt(0,1,0,0,center, v);
	t3 = parametercnt(1,0,0,0,center, v);
	t4 = parametercnt(0,0,diag.z,0,center, v);
	t5 = parametercnt(0,diag.y,0,0,center, v);
	t6 = parametercnt(diag.x,0,0,0,center, v);
	
	arr[0] = intersect(t1,center, v);
	arr[1] = intersect(t2,center, v);
	arr[2] = intersect(t3,center, v);
	arr[3] = intersect(t4,center, v);
	arr[4] = intersect(t5,center, v);
	arr[5] = intersect(t6,center, v);
	
	array[0] = norm(arr[0]);
	array[1] = norm(arr[1]);
	array[2] = norm(arr[2]);
	array[3] = norm(arr[3]);
	array[4] = norm(arr[4]);
	array[5] = norm(arr[5]);
	
	float smallest = 0;
	int index;
	
	for(int i = 0; i < 6; ++i){
		if(array[i] < smallest && array[i] > 0){
			smallest = array[i];
			index = i;
		}
	}
	
	return arr[index];
}

vec3 support(vec3 v, Tetrahedron t ){
	
	if(norm(v) == 0){
		return v;
	}
	vec3 center = 0.25*(t.points[0] + t.points[1] + t.points[2] + t.points[3] ); //famous formula for triangles - works also in higher dimension
	vec3 n1 = (t.points[0] - t.points[1])^(t.points[0] - t.points[3]);
	vec3 n2 = (t.points[0] - t.points[2])^(t.points[0] - t.points[3]);
	vec3 n3 = (t.points[0] - t.points[1])^(t.points[0] - t.points[2]);
	vec3 n4 = (t.points[1] - t.points[2])^(t.points[1] - t.points[3]); //normal vectors of planes containing sides of tetrahedron
	//we can act the same as with cuboid but we need to find D parameter in equation of plane
	float D1 = (-1)*t.points[0] * n1;
	float D2 = (-1)*t.points[0] * n2;
	float D3 = (-1)*t.points[0] * n3;
	float D4 = (-1)*t.points[1] * n4;
	
	vec3 arr[4]; //array of vectors
	float array[4]; //array of norms 
	
	float t1, t2, t3, t4;
	
	t1 = parametercnt(n1.x,n1.y,n1.z,D1,center, v);
	t2 = parametercnt(n2.x,n2.y,n2.z,D2,center, v);
	t3 = parametercnt(n3.x,n3.y,n3.z,D3,center, v);
	t4 = parametercnt(n4.x,n4.y,n4.z,D4,center, v);

	
	arr[0] = intersect(t1,center, v);
	arr[1] = intersect(t2,center, v);
	arr[2] = intersect(t3,center, v);
	arr[3] = intersect(t4,center, v);

	
	array[0] = norm(arr[0]);
	array[1] = norm(arr[1]);
	array[2] = norm(arr[2]);
	array[3] = norm(arr[3]);

	
	float smallest = 0;
	int index;
	
	for(int i = 0; i < 4; ++i){
		if(array[i] < smallest && array[i] > 0){
			smallest = array[i];
			index = i;
		}
	}
	
	return arr[index];
	
	return v;
}


vec3 support(vec3 v, Capsule c){
	if(norm(v) == 0){
		return v;
	}
	vec3 center = 0.5*(c.points[0] + c.points[1]); //center of capsule
	//we need to iplement rotation so that axis of capsule would OZ axis and points[0] = (0,0,0)
	// we must check if intersection is with two semispheres or with cyllinder
	// points[1] is height we can easly calculate maximal sinus where intersection is with cyllinder
	//sinus is increasing on [0;pi/2]
	// if sin <0 then intersection is with lowes semisphere
	// if intersection is with semispheres we use funtion support(vec3,Sphere)
	//if with cyllinder then we "normalize" v to c.radius  and normalized vector n we swap n.z to v.z.
	
	return v;
}


//2.********************************************************************************************************************************************************

std::string compress(std::string str){ //works for small letters

	int array[26];
	for(int i = 0; i < 26; ++i){
		array[i] = 0;
	}
	
	char arr[str.length() + 1]; 
    strcpy(arr, str.c_str()); 
    char holder;	

	for(int i = 0; i < str.length() + 1; ++i){ //apperances of i-th letter is remembered  under index i
		holder = arr[i];
		array[(int)holder - 'a']++;	
	}
	
	std::string result = "";
	std::stringstream converter;
	
	for(int i = 0; i < 26; ++i){
		if(array[i] != 0){
			holder = i + 'a'; //letter and number of apperances are written to string
			converter << holder;
			converter << array[i];
			result += converter.str();
		}
	converter.str(std::string());
	}
	
	
	return result;
}

std::string decompress(std::string str){ //string must be in form aibj... etc
	char arr[str.length() + 1]; 
    strcpy(arr, str.c_str()); 
    std::string coresult = "";
    std::stringstream converter;
    
    for(int i = 0; i < str.length() + 1; i+=2){  //counting every two - index i is always a letter and i+1 number of iterations
    	for(int j = 0; j < (arr[i+1] - '0'); ++j){
    		converter << arr[i];
		}
		coresult += converter.str();
		converter.str(std::string());
	}
return coresult;
}

//3.******************************************************************************************************************************************************** 

std::vector<int> del(std::vector<int> V, int index){ //implemented for integeers deletes in time O(1) element of given index
	if(index < 0 || index >= V.size()){
		return V;
	}
	else if(index  == V.size() - 1){
		V.pop_back();
		return V;
	}
	else{
		V[index] = V[V.size() - 1]; //if element is not last and in range we assign value of last one into given index nd then delete the last one
		V.pop_back(); 
		return V;
	} //function works the same with any size so complexity is constant
}


int main(int argc, char** argv) {

	return 0;
}
