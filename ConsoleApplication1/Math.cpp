#include "Math.h"

#include <cmath>
#include <limits>

int clamp(const int& val, const int& lower, const int& upper)
{
    return val < lower ? lower : val > upper ? upper : val;
}

float clampf(const float& val, const float& lower, const float& upper)
{
    return val < lower ? lower : val > upper ? upper : val;
}

double realLog(double X, double base)
{
    double lnX = std::log(X);
    double lnbase = std::log(base);

    if (lnbase < 0)
    {
        return std::numeric_limits<double>::infinity();
    }

    return lnX / lnbase;
}

double complexMod(double real, double imag)
{
    return std::hypot(real, imag);
}

double complexArg(double real, double imag)
{
    return std::atan2(imag, real);
}

double complexExpRe(double real, double imag)
{
    return std::exp(real) * std::cos(imag);
}

double complexExpIm(double real, double imag)
{
    return std::exp(real) * std::sin(imag);
}

void complexExp(double* Rresult, double* Iresult, double real, double imag)
{
    double exp = std::exp(real);
    *Rresult = exp * std::cos(imag);
    *Iresult = exp * std::sin(imag);
}

double complexLnRe(double R1, double I1, int k)
{
    return std::log(complexMod(R1, I1));
}

double complexLnIm(double R1, double I1, int k)
{
    return complexArg(R1, I1) + 2 * k * PI;
}

void complexLn(double* Rresult, double* Iresult, double R1, double I1, int k)
{
    *Rresult = std::log(complexMod(R1, I1));
    *Iresult = complexArg(R1, I1) + 2 * k * PI;
}

double complexLogRe(double R1, double I1, double R2, double I2, int k, int m)
{
    double re1, im1, re2, im2;

    // ln(z1) / ln(z2)
    complexLn(&re1, &im1, R1, I1, k);
    complexLn(&re2, &im2, R2, I2, m);

    double denominator = (re2 * re2) + (im2 * im2);
    // Real part of division: (ac + bd) / (c^2 + d^2)

    if (denominator == 0)
        return std::numeric_limits<double>::infinity();

    return (re1 * re2 + im1 * im2) / denominator;
}

double complexLogIm(double R1, double I1, double R2, double I2, int k, int m)
{
    double re1, im1, re2, im2;

    // ln(z1) / ln(z2)
    complexLn(&re1, &im1, R1, I1, k);
    complexLn(&re2, &im2, R2, I2, m);

    double denominator = (re2 * re2) + (im2 * im2);
    // Imaginary part of division: (bc - ad) / (c^2 + d^2)

    if (denominator == 0)
        return std::numeric_limits<double>::infinity();

    return (im1 * re2 - re1 * im2) / denominator;
}

void complexLog(double* Rresult, double* Iresult, double R1, double I1, double R2, double I2, int k, int m)
{
    // ln(z1) / ln(z2)
    double re1, im1, re2, im2;

    // ln(z1) / ln(z2)
    complexLn(&re1, &im1, R1, I1, k);
    complexLn(&re2, &im2, R2, I2, m);

    double denominator = (re2 * re2) + (im2 * im2);
    // Real part of division: (ac + bd) / (c^2 + d^2)

    if (denominator != 0)
    {
        *Rresult = (re1 * re2 + im1 * im2) / denominator;
        *Iresult = (im1 * re2 - re1 * im2) / denominator;

        return;
    }

    *Rresult = std::numeric_limits<double>::infinity();
    *Iresult = std::numeric_limits<double>::infinity();
}

double complexPowIm(double R1, double I1, double R2, double I2, int k)
{
    // 1. Calculate the components of ln(base)
    double ln_r = std::log(complexMod(R1, I1));
    double fullTheta = complexArg(R1, I1) + 2.0 * k * PI;

    // 2. Real and Imaginary parts of the product (power * ln(base))
    // Real Part: (R2 * ln_r) - (I2 * fullTheta)
    double R_exp = (R2 * ln_r) - (I2 * fullTheta);

    // Imaginary Part: (I2 * ln_r) + (R2 * fullTheta)
    double I_exp = (I2 * ln_r) + (R2 * fullTheta);

    // 3. Return the imaginary part of exp(R_exp + i*I_exp)
    // formula: exp(Real) * sin(Imaginary)
    return std::exp(R_exp) * std::sin(I_exp);
}

double complexPowRe(double R1, double I1, double R2, double I2, int k)
{
    // 1. Calculate the natural log components of the base (z1)
    // Real part of ln(z1) is log of magnitude
    double ln_r = std::log(complexMod(R1, I1));
    // Imaginary part of ln(z1) is the angle + 2k*PI
    double fullTheta = complexArg(R1, I1) + 2.0 * k * PI;

    // 2. Multiply the exponent (z2 = R2 + iI2) by the log of the base
    // This is FOIL: (R2 + iI2) * (ln_r + i*fullTheta)
    // Real part of the resulting exponent:
    double R_exp = (R2 * ln_r) - (I2 * fullTheta);

    // Imaginary part of the resulting exponent:
    double I_exp = (I2 * ln_r) + (R2 * fullTheta);

    // 3. Return the real part of exp(R_exp + i*I_exp)
    // formula: exp(Real) * cos(Imaginary)
    return std::exp(R_exp) * std::cos(I_exp);
}

void complexPow(double* Rresult, double* Iresult, double R1, double I1, double R2, double I2, int k)
{
    double lnRe, lnIm;
    // Get both components of the log in one go
    complexLn(&lnRe, &lnIm, R1, I1, k);

    double targetExpRe = R2 * lnRe - I2 * lnIm;
    double targetExpIm = R2 * lnIm + I2 * lnRe;

    complexExp(Rresult, Iresult, targetExpRe, targetExpIm);
}

void complexTimes(double* Rresult, double* Iresult, double R1, double I1, double R2, double I2)
{
    double tempR = (R1 * R2) - (I1 * I2);
    double tempI = (R1 * I2) + (I1 * R2);

    *Rresult = tempR;
    *Iresult = tempI;
}

void complexDiv(double* Rresult, double* Iresult, double R1, double I1, double R2, double I2)
{
    double denominator = (R2 * R2) + (I2 * I2);

    if (denominator == 0.0) {
        // Handle division by zero
        *Rresult = std::numeric_limits<double>::infinity();
        *Iresult = std::numeric_limits<double>::infinity();
        return;
    }

    double tempR = (R1 * R2 + I1 * I2) / denominator;
    double tempI = (I1 * R2 - R1 * I2) / denominator;

    *Rresult = tempR;
    *Iresult = tempI;
}