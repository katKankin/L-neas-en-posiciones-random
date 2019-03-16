/*  -----------------------------------------------
TECNOLÓGICO DE COSTA RICA
UNIDAD DE COMPUTACIÓN
INTRODUCCIÓN A LOS GRÁFICOS POR COMPUTADORA
PROYECTO I
KATHERINE TUZ CARRILLO
--------------------------------------------------- */
#include "stdafx.h"
#include <GL/glut.h>
#include <stdio.h>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <time.h>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace System;
#include <iostream>
using namespace std;
unsigned ta, tb, ta1, tb1, ta2, tb2, ta3, tb3; // Variables for execution time
int resolution, lines, times; // Variables for configuration 
double tBres, tFB, tI, tI2; // Time values of each algorithm
FILE *fp; 

void myInit() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 500, 0, 500);
}

void draw_pixel(int x, int y) {
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

//--------------------------------------- ALGORITHMS ----------------------------------------------------
// BRESENHAM ALGORITHM
void bresenham(int x1, int y1, int x2, int y2) {
	ta = ta + clock(); //start time
	int dx, dy, i, e;
	int incx, incy, inc1, inc2;
	int x, y;
	dx = x2 - x1;
	dy = y2 - y1;
	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	incx = 1;
	if (x2 < x1) incx = -1;
	incy = 1;
	if (y2 < y1) incy = -1;
	x = x1; y = y1;
	tb = tb + clock(); //close time
	if (dx > dy) {
		draw_pixel(x, y);
		ta = ta + clock(); //start time
		e = 2 * dy - dx;
		inc1 = 2 * (dy - dx);
		inc2 = 2 * dy;
		tb = tb + clock(); //close time
		for (i = 0; i < dx; i++) {		
			if (e >= 0) {
				ta = ta + clock(); //start time
				y += incy;
				e += inc1;
				tb = tb + clock(); //close time
			}
			else{
				ta = ta + clock(); //start time
				e += inc2;
				tb = tb + clock(); //close time
			}

			ta = ta + clock(); //start time
			x += incx;
			tb = tb + clock(); //close time
			draw_pixel(x, y);
		}
	}
	else {
		draw_pixel(x, y);
		ta = ta + clock(); //start time
		e = 2 * dx - dy;
		inc1 = 2 * (dx - dy);
		inc2 = 2 * dx;
		tb = tb + clock(); //close time
		for (i = 0; i < dy; i++) {
			if (e >= 0) {
				ta = ta + clock(); //start time
				x += incx;
				e += inc1;
				tb = tb + clock(); //close time
			}
			else{
				ta = ta + clock(); //start time
				e += inc2;
				tb = tb + clock(); //close time
			}
			ta = ta + clock(); //start time
			y += incy;
			tb = tb + clock(); //close time
			draw_pixel(x, y);
		}
	}
	tBres = (double(tb - ta) / CLOCKS_PER_SEC);
}
// FUERZA BRUTA ALGORITHM
void fuerzabruta(int t0, int r0, int t1, int r1) {
	ta1 = ta1 + clock(); //start time
	double m, b, y;
	int i;
	m = ((static_cast<double>(r1) - static_cast<double>(r0)) / (static_cast<double>(t1) - static_cast<double>(t0)));
	b = r0 - m * t0;
	tb1 = tb1 + clock(); //close time
	for (i = t0; i <= t1; i++) {
		ta1 = ta1 + clock(); //start time
		y = m*i + b;
		tb1 = tb1 + clock(); //close time
		draw_pixel(i, round(y));
	}
	tFB = (double(tb1 - ta1) / CLOCKS_PER_SEC);
}
// INCREMENTAL ALGORITHM
void incremento(int t0, int r0, int t1, int r1) {
	ta2 = ta2 + clock(); //start time
	double m, y;
	int i;
	m = ((static_cast<double>(r1) - static_cast<double>(r0)) / (static_cast<double>(t1) - static_cast<double>(t0)));
	y = static_cast<double>(r0);
	tb2 = tb2 + clock(); //close time
	for (i = t0; i <= t1; i++) {
		draw_pixel(i, round(y));
		ta2 = ta2 + clock(); //start time
		y += m;
		tb2 = tb2 + clock(); //close time
	}
	tI = (double(tb2 - ta2) / CLOCKS_PER_SEC);
}
// INCREMENTAL 2 ALGORITHM
void incremento2(int x0, int y0, int x1, int y1) {
	ta3 = ta3 + clock(); //start time
	double x, y, paso_x, paso_y;
	int i, ancho;
	ancho = std::max(abs(x1 - x0), abs(y1 - y0));
	paso_x = (x1 - x0) / ancho;
	paso_y = (y1 - y0) / ancho;
	x = x0;
	y = y0;
	tb3 = tb3 + clock(); //close time
	for (i = 0; i <= ancho; i++) {
		draw_pixel(round(x), round(y));
		ta3 = ta3 + clock(); //start time
		x += paso_x;
		y += paso_y;
		tb3 = tb3 + clock(); //close time
	}
	tI2 = (double(tb3 - ta3) / CLOCKS_PER_SEC);
}


//--------------------------------------- COMPLEMENTS ----------------------------------------------------
void randomXY() { //Generates random position
	int value;
	srand(time(NULL));
	ofstream myfile("positions.txt"); 
	for (int i = 0; i < lines; i++) { 									 								   
		for (int j = 0; j < 4; j++) {  // format: x1, x2, y1, y2
			value = rand() % resolution + 1; // < resolution
			myfile << value;
			myfile << " ";
		}
		myfile << "\n"; 
	}
	myfile.close();
}

void fileExtractor() { //Reads the file that have the positions
	fp = fopen("positions.txt", "r"); 
	int xx, xy, xz, xw;
	if (fp == NULL) {
		printf("-Could'nt open file-");
		return;
	}
	rewind(fp); 
	while (!feof(fp))
	{
		fscanf(fp, "%d %d %d %d", &xx, &xy, &xz, &xw);  
		glColor3f(0.5, 0.5, 0.0);

		bresenham(xx, xy, xz, xw); 
		glFlush();		
	
		fuerzabruta(xx, xy, xz, xw);
		glFlush();		
	
		incremento(xx, xy, xz, xw);
		glFlush();	
		
		incremento2(xx, xy, xz, xw);
		glFlush();
	}
	cout << "-------------------------- " << endl;
	cout << "Bresenham Time: " << tBres << endl;
	cout << "FuerzaBruta Time: " << tFB << endl;
	cout << "Incremental Time: " << tI << endl;
	cout << "Incremental2 Time: " << tI2 << endl;
}

void menu() {
	cout << "---------CONFIGURATION--------" << endl;
	cout << "Resolution: ";
	cin >> resolution;
	cout << "Lines: ";
	cin >> lines;
	cout << "Times: ";
	cin >> times;
	cout << "--PROCESSING CONFIGURATION--" << endl;
}


//---------------------------------------- MAIN ----------------------------------------------------------
void main(int argc, char **argv) {
	glutInit(&argc, argv);
	menu(); // Sets configuration

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(resolution, resolution); // Sets resolution
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Algorithms for lines");
	myInit();
	
	randomXY(); // Sets random positions in a file		

	for (int i = 0; i < times; i++) {
		glutDisplayFunc(fileExtractor); //Algorithm call
	}
	

	glutMainLoop();
}
