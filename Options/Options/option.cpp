#include "option.h"
#include <math.h>
#include <cmath>
#include <iostream>
#include <vector>

Option::Option(double K, double T, double sigma, double r) {
    this->K = K;
    this->T = T;
    this->sigma = sigma;
    this->r = r;
}

double Option::getBinomialTreeValue(double s, int N, double contDivYield) {
    std::vector<double> treeValues(N + 1); // holds tree node values
    double deltaT = this->T / N;
    double up = exp(this->sigma * sqrt(deltaT)); // by how much go up when goes up
    double down = 1 / up;
    double p0;
    if (contDivYield == -1.0) {
        p0 = (exp(r * deltaT) - down) / (up - down); // RNP up
    } else {
        p0 = (exp((r - contDivYield) * deltaT) - down) / (up - down); // RNP up
    }
    double p1 = 1 - p0; // RNP down
    // get exercice values at expiration
    for(int i = 0; i <= N; i++) {
        // going up i times out of N times
        treeValues[i] = this->getExerciseValue(s * pow(up, 2.0 * i - N), this->T);
    }
    // move to ealier times
    double t = this->T; // current time
    for(int j = N - 1; j >= 0; j--) {
        t -= deltaT; // current time goes down by one timeStep
        // considering we went up i times out of j times...
        for(int i = 0; i <= j; i++) {
            double currentSpot = s * pow(up, 2 * i - j);
            // exercise value at this time (this current spot, this time)
            double exercise = getExerciseValue(currentSpot, t);
            // at this node, tree values are...
            treeValues[i] = (p0 * treeValues[i + 1] + p1 * treeValues[i]) * exp(-r * deltaT);
            // check for early exercise
            if (treeValues[i] < exercise) {
                treeValues[i] = exercise;
            }
        }
    }
//    std::cout << "position 1   " << treeValues[1] << std::endl;
//    std::cout << "position 2   " << treeValues[2] << std::endl;
    return treeValues[0];
}

double Option::getd1(double s) {
    double d1 = (1 / (this->sigma * sqrt(this->T))) * (log(s / this->K) + (this->r + this->sigma * this->sigma / 2) * this->T);
    return d1;
}

double Option::getd2(double s) {
    double d2 = this->getd1(s) - this->sigma * sqrt(this->T);
    return d2;
}

double Option::normalCDF(double x) {
    return std::erfc(-x/std::sqrt(2))/2;
}

EuropeanOption::EuropeanOption(double K, double T, double sigma, double r): Option(K, T, sigma, r) {
}

double EuropeanOption::getValue(double s) {
    return getBlackScholesValue(s);
}

AmericanOption::AmericanOption(double K, double T, double sigma, double r): Option(K, T, sigma, r) {
}

double AmericanOption::getValue(double s) {
    return getBinomialTreeValue(s, 250);
}

KnockOutOption::KnockOutOption(double K, double T, double sigma, double r, double B): Option(K, T, sigma, r) {
    this->B = B;
}

double KnockOutOption::getValue(double s) {
    return getBinomialTreeValue(s, 250);
}

bool KnockOutOption::isAcrossBarrier(double s) {
    // If we have a down-and-out option...
    if (this->K > this->B) {
        // then isAcrossBarrier returns true if s < B.
        return (s < this->B);
    }
    
    // If we have an up-and-out option, isAcrossBarrier returns true if s > B
    return (s > this->B);
}

EuropeanCall::EuropeanCall(double K, double T, double sigma, double r): EuropeanOption(K, T, sigma, r) {
}

EuropeanPut::EuropeanPut(double K, double T, double sigma, double r): EuropeanOption(K, T, sigma, r) {
}

AmericanCall::AmericanCall(double K, double T, double sigma, double r): AmericanOption(K, T, sigma, r) {
}

AmericanPut::AmericanPut(double K, double T, double sigma, double r): AmericanOption(K, T, sigma, r) {
}

KnockOutCall::KnockOutCall(double K, double T, double sigma, double r, double B): KnockOutOption(K, T, sigma, r, B) {
}

CompoundEuropeanCall::CompoundEuropeanCall(double K, double T, double sigma, double r): EuropeanOption(K, T, sigma, r) {
}

KnockOutPut::KnockOutPut(double K, double T, double sigma, double r, double B): KnockOutOption(K, T, sigma, r, B) {
}

ReloadableCallOption::ReloadableCallOption(double K, double T, double sigma, double r):AmericanOption(K, T, sigma, r) {
}

ExtendibleCall::ExtendibleCall(double K, double T, double sigma, double r):AmericanOption(K, T, sigma, r) {
}

double AmericanCall::getExerciseValue(double s, double t) {
    return std::max(0.0, s - this->K);
}

double AmericanPut::getExerciseValue(double s, double t) {
    return std::max(0.0, this->K - s);
}

// the payoff comes in two parts
double ReloadableCallOption::getExerciseValue(double s, double t) {
    // 1) 'Call payoff' part: max(price - strike, 0)
    double callPart = std::max(0.0, s - this->K);
    if (t == this->T) {
        return callPart;
    }
    // 2) new option part: get new options...
    // number: the number of shares it takes to pay the strike when exercising
    double numNewOptions = this->K / s;
    // Compute the value of the new option you would receive a fraction of
    // same strike and expiration
    EuropeanCall newCall(this->K, this->T - t, this->sigma, this->r);
    double valNewCall = newCall.getBinomialTreeValue(s, 100);
    return numNewOptions * valNewCall + callPart;
}

/*
PURPOSE: calculate exercise (= intrinsic) value of Extendible American call
INPUT: two doubles
    s: the spot price
    t: time
OUTPUT: a double, intrinsic value for the call
 */
double ExtendibleCall::getExerciseValue(double s, double t) {
    // Payoff if you choose to exercise at t
    // /!\ Assumptions: The extended call has maturity same as initial call, and
    // strike 1 higher
    double exercise = s - this->K;
    // Payoff if you choose to extend until 2 t (ie add t years)
    AmericanCall extended(this->K + 1, this->T, this->sigma, this->r);
    double extendVal = extended.getBlackScholesValue(s);
    return std::max(exercise, extendVal);
}

double ReloadableCallOption::getBlackScholesValue(double s) {
    throw "Do not use Black-Scholes for Reloadable Call yet";
    return -1;
}

double EuropeanCall::getExerciseValue(double s, double t) {
    if (this->T != t) {
        return 0;
    }
    return std::max(0.0, s - this->K);
}

double EuropeanPut::getExerciseValue(double s, double t) {
    if (this->T != t) {
        return 0;
    }
    return std::max(0.0, this->K - s);
}

double CompoundEuropeanCall::getExerciseValue(double s, double t) {
    double t2 = 6.0/12;
    double k2 = 90.0;
    // If this is not maturity yet, the value is 0
    if (this->T != t) {
        return 0;
    }
    // Computing the price of the underlying European call at maturity
    double remainingTimeForUnderlying = t2 - this->T; // time before maturity of underlying option
    // Create the object corresponding to the underlying European call option
    EuropeanCall underlyingCall(k2, remainingTimeForUnderlying, sigma, r);
    // Value the underlying European call option with Binomial tree
    double val = underlyingCall.getBinomialTreeValue(s, 30);
    // consider we can buy this underlying option for the strike price K
//    std::cout << "The value of the underlying option is " << val << std::endl;
    return std::max(0.0, val - this->K);
}

double EuropeanCall::getBlackScholesValue(double s) {
    return normalCDF(getd1(s)) * s - normalCDF(getd2(s)) * this->K * exp(- r * this->T);
}

double CompoundEuropeanCall::getBlackScholesValue(double s) {
    std::cout << "Standard Black Scholes should not be used on Compound Call";
    return -1;
}

double ExtendibleCall::getBlackScholesValue(double s) {
    std::cout << "Standard Black Scholes should not be used on Extendible Call";
    return -1;
}

double EuropeanPut::getBlackScholesValue(double s) {
    return normalCDF(- getd2(s)) * this->K * exp(-r * this->T) - normalCDF(- getd1(s)) * s;
}

double AmericanCall::getBlackScholesValue(double s) {
    return normalCDF(getd1(s)) * s - normalCDF(getd2(s)) * this->K * exp(- r * this->T);
}

double AmericanPut::getBlackScholesValue(double s) {
    return normalCDF(- getd2(s)) * this->K * exp(-r * this->T) - normalCDF(- getd1(s)) * s;
}

double KnockOutCall::getExerciseValue(double s, double t) {
    if (this->T != t) {
        return 0;
    }
    if (isAcrossBarrier(s)) {
        return 0;
    }
    return std::max(0.0, s - this->K);
}

double KnockOutCall::getBlackScholesValue(double s) {
    return normalCDF(getd1(s)) * s - normalCDF(getd2(s)) * this->K * exp(- r * this->T);
}

double KnockOutPut::getExerciseValue(double s, double t) {
    if (this->T != t) {
        return 0;
    }
    if (isAcrossBarrier(s)) {
        return 0;
    }
    return std::max(0.0, this->K - s);
}

double KnockOutPut::getBlackScholesValue(double s) {
    return normalCDF(- getd2(s)) * this->K * exp(-r * this->T) - normalCDF(- getd1(s)) * s;
}
