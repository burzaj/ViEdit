#pragma once

#include <cmath>

#define PI (double)(3.14159265359)
#define E (double)(2.71828182846)

double realLog(double X, double base);

double complexMod(double real, double imag);
double complexArg(double real, double imag);

double complexExpRe(double real, double imag);
double complexExpIm(double real, double imag);
void complexExp(double* Rresult, double* Iresult, double real, double imag);

double complexLnRe(double R1, double I1, int k);
double complexLnIm(double R1, double I1, int k);
void complexLn(double* Rresult, double* Iresult, double R1, double I1, int k);

double complexLogRe(double R1, double I1, double R2, double I2, int k, int m);
double complexLogIm(double R1, double I1, double R2, double I2, int k, int m);
void complexLog(double* Rresult, double* Iresult, double R1, double I1, double R2, double I2, int k, int m);

double complexPowRe(double R1, double I1, double R2, double I2, int k);
double complexPowIm(double R1, double I1, double R2, double I2, int k);
void complexPow(double* Rresult, double* Iresult, double R1, double I1, double R2, double I2, int k);

void complexTimes(double* Rresult, double* Iresult, double R1, double I1, double R2, double I2);
void complexDiv(double* Rresult, double* Iresult, double R1, double I1, double R2, double I2);

int clamp(const int& val,
	const int& lower,
	const int& upper);

float clampf(const float& val, 
	const float& lower, 
	const float& upper);