//  Created by Gabriel Cazaubieilh on 01/02/2020.

#ifndef option_hpp
#define option_hpp

// LEVEl: 1
class Option {
public:
    // constructor
    Option(double K, double T, double sigma, double r);
    // returns the exercise value of the option for spot price s at time t
    virtual double getExerciseValue(double s, double t) = 0;
    // returns the Black Scholes value of the option
    virtual double getBlackScholesValue(double s) = 0;
    // returns the value of the option using a binomial tree of depth N
    double getBinomialTreeValue(double s, int N);
    // returns the option value for spot value s
    virtual double getValue(double s) = 0;
    
protected:
    double K; // strike
    double T; // time-to-maturity
    double sigma; // stock volatility
    double r; // risk-free rate
    // given spot (double), compute the d1 factor in the BS formula
    double getd1(double s);
    double getd2(double s);
    double normalCDF(double x);
};

// LEVEL: 2
class EuropeanOption: public Option {
public:
    EuropeanOption(double K, double T, double sigma, double r);
    // returns the Black-Scholes value
    virtual double getValue(double s);
};

class AmericanOption: public Option {
public:
    AmericanOption(double K, double T, double sigma, double r);
    // returns the binomial tree value of the option, with depth = 250
    virtual double getValue(double s);
};

class KnockOutOption: public Option {
public:
    KnockOutOption(double K, double T, double sigma, double r, double B);
    // returns the binomial tree value of the option, with depth = 250
    virtual double getValue(double s);
protected:
    double B; // barrier
    bool isAcrossBarrier(double s); // returns whether spot > barrier
    
};

// LEVEL: 3
class EuropeanCall: public EuropeanOption {
public:
    EuropeanCall(double K, double T, double sigma, double r);
    /*
    PURPOSE: calculate exercise value of European call
    INPUT: two doubles
        s: the spot price
        t: time
    OUTPUT: 0 if not at maturity, otherwise intrinsic value of put
    */
    virtual double getExerciseValue(double s, double t);
    virtual double getBlackScholesValue(double s);
};

class EuropeanPut: public EuropeanOption {
public:
    EuropeanPut(double K, double T, double sigma, double r);
    /*
     PURPOSE: calculate exercise value of European put
     INPUT: two doubles
         s: the spot price
         t: time
     OUTPUT: 0 if not at maturity, otherwise intrinsic value of put
     */
    virtual double getExerciseValue(double s, double t);
    virtual double getBlackScholesValue(double s);
};

class AmericanCall: public AmericanOption {
public:
    AmericanCall(double K, double T, double sigma, double r);
    /*
    PURPOSE: calculate exercise (= intrinsic) value of American call
    INPUT: two doubles
        s: the spot price
        t: time
    OUTPUT: a double, intrinsic value for the call
     */
    virtual double getExerciseValue(double s, double t);
    virtual double getBlackScholesValue(double s);
};

class AmericanPut: public AmericanOption {
public:
    AmericanPut(double K, double T, double sigma, double r);
    /*
    PURPOSE: calculate exercise (= intrinsic) value of American Put
    INPUT: two doubles
        s: the spot price
        t: time
    OUTPUT: a double, intrinsic value for the put
    */
    virtual double getExerciseValue(double s, double t);
    virtual double getBlackScholesValue(double s);
};
class KnockOutCall: public KnockOutOption {
public:
    KnockOutCall(double K, double T, double sigma, double r, double B);
    virtual double getExerciseValue(double s, double t);
    virtual double getBlackScholesValue(double s);
};

class KnockOutPut: public KnockOutOption {
public:
    KnockOutPut(double K, double T, double sigma, double r, double B);
    virtual double getExerciseValue(double s, double t);
    virtual double getBlackScholesValue(double s);
};
#endif /* option_hpp */
