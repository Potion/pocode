//
//  main.cpp
//  data dump
//
//  Created by Joshua Fisher on 8/16/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include <cmath>
#include <cstdio>
#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>

void theCWay();
void theCPPWay();
void whatIWant();

int main (int argc, const char * argv[]) {
	theCWay();
	theCPPWay();
	whatIWant();
	
    return 0;
}

void theCWay() {
	float *floats = (float*)malloc(sizeof(float)*100);
	for(int i=0; i<100; i++)
		floats[i] = i + 0.5f;
	
	FILE *f = fopen("testC.dat", "wb");
	fwrite(floats, sizeof(float), 100, f);
	fclose(f);
	
	memset(floats, 0, sizeof(float)*100);
	
	f = fopen("testC.dat", "rb");
	fread(floats, sizeof(float), 100, f);
	fclose(f);
	
	for(int i=0; i<100; i++) {
		assert(fabs(i+0.5f - floats[i]) < 0.000001);
	}
	
	free(floats);
}

void theCPPWay() {
	using namespace std;
	float *floats = new float[100];
	for(int i=0; i<100; i++)
		floats[i] = i + 0.5f;

	ofstream out_f("testCPP.dat", ios::binary);
	out_f.write((char*)floats, sizeof(float)*100);
	out_f.close();
	
	fill(floats, floats+100, 0.f);
	
	ifstream in_f("testCPP.dat", ios::binary);
	in_f.read((char*)floats, sizeof(float)*100);
	in_f.close();
	
	for(int i=0; i<100; i++) {
		assert(fabs(i+0.5f - floats[i]) < 0.000001);
	}
	
	delete [] floats;
}

struct Point {
	float x, y, z;
};

void whatIWant() {
	using namespace std;
	Point points[100];
	for(int i=0; i<100; i++) {
		points[i].x = i;
		points[i].y = i;
		points[i].z = i;
	}
	
	stringstream ss;
	ss.write((char*)points, sizeof(Point)*100);
	ss.seekg(ios::beg);
	
	fill(points, points+100, Point());
	
	string str = ss.str();
	printf("%lu bytes in binary string\n", str.size());
	printf("%lu bytes in c_str\n", strlen(str.c_str()));
	
	ss.read((char*)points, sizeof(Point)*100);
	ss.seekg(ios::beg);

	for(int i=0; i<100; i++) {
		assert(fabs(i - points[i].x) < 0.000001);
	}
	
	ofstream out_f("testStr.dat", ios::binary);
	out_f << ss.rdbuf();
	out_f.close();
}
