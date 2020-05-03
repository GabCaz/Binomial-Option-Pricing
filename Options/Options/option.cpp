//  Created by Gabriel Cazaubieilh on 01/02/2020.

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

double Option::getBinomialTreeValue(double s, int N) {
    std::vector<double> treeValues(N + 1); // holds tree node values
    double deltaT = this->T / N;
    double up = exp(this->sigma * sqrt(deltaT)); // by how much go up when goes up
    double p0 = (up - exp(-r * deltaT)) / (up * up - 1);
    double p1 = exp(-r * deltaT) - p0; 
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
            treeValues[i] = p0 * treeValues[i + 1] + p1 * treeValues[i];
            // check for early exercise
            if (treeValues[i] < exercise) {
                treeValues[i] = exercise;
            }
        }
    }
    return treeValues[0];
}

double Option::getd1(double s) {
    return (1 / (this->sigma * sqrt(this->T))) * (log(s / this->K) + (this->r + this->sigma * this->sigma / 2) * this->T);
}

double Option::getd2(double s) {
    return this->getd1(s) - this->sigma * sqrt(this->T);
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

KnockOutPut::KnockOutPut(double K, double T, double sigma, double r, double B): KnockOutOption(K, T, sigma, r, B) {
}

double AmericanCall::getExerciseValue(double s, double t) {
    return std::max(0.0, s - this->K);
}

double AmericanPut::getExerciseValue(double s, double t) {
    return std::max(0.0, this->K - s);
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

double EuropeanCall::getBlackScholesValue(double s) {
    return normalCDF(getd1(s)) * s - normalCDF(getd2(s)) * this->K * exp(- r * this->T);
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
