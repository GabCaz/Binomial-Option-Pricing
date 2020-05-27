#ifndef option_hpp
#define option_hpp

// LEVEl 1: Specifying valuation rules general to options
class Option {
public:
    // constructor
    Option(double K, double T, double sigma, double r);
    // returns the exercise value of the option for spot price s at time t
    virtual double getExerciseValue(double s, double t) = 0;
    // returns the Black Scholes value of the option
    virtual double getBlackScholesValue(double s) = 0;
    // returns the value of the option using a binomial tree of depth N
    double getBinomialTreeValue(double s, int N, double contDivYield=-1);
    // returns the option value for spot value s
    virtual double getValue(double s) = 0;
    
protected:
    double K; // strike
    double T; // time-to-maturity
    double sigma; // underlying volatility
    double r; // risk-free rate
    // given spot (double), compute the d1 factor in the BS formula
    double getd1(double s);
    double getd2(double s);
    double normalCDF(double x);
};

// LEVEL 2: Specifying when the right can be exercised
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

// LEVEL 3: Specifying whether can buy or sell underlying
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
private:
    friend class CompoundEuropeanCall; // needed when computing terminal tree value of underlying
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

class CompoundEuropeanCall: public EuropeanOption {
public:
    CompoundEuropeanCall(double K, double T, double sigma, double r);
    /*
    PURPOSE: calculate exercise value of Coumpound Call on a European Call
     This option gives the right to purchase a European Call on the underlying,
     at maturity T (T1), for a price k2. The underlying European Call has
     maturity t2.
    INPUT: two doubles
        s: the spot price of the stock
        t: time
        t2: the maturity of the underlying option (fixed at 6 / 12 for our exercise)
        k2: the strike price of the underlying option (fixed at 90)
    OUTPUT: 0 if not at maturity, otherwise the payoff. The payoff is the price
     of the underlying option - k2 if the price of the option is greater than k2,
     and 0 otherwise. The price of the underlying option is estimated with a
     Bimomial tree.
    */
    virtual double getExerciseValue(double s, double t);
    virtual double getBlackScholesValue(double s);
};

class ReloadableCallOption: public AmericanOption {
public:
    ReloadableCallOption(double K, double T, double sigma, double r);
    /*
     Specifies the payoff of the option given time and current asset price.
     The payoff comes in two parts:
     1) 'Call payoff' part: max(price - strike, 0)
     2) new option part: get new options with the same strike and expiration
        date as the old one, where the number equals the number of shares it
        takes to pay the strike when exercising
     */
    virtual double getExerciseValue(double s, double t);
    virtual double getBlackScholesValue(double s);
};

class ExtendibleCall: public AmericanOption {
public:
    ExtendibleCall(double K, double T, double sigma, double r);
    /*
    PURPOSE: calculate exercise (= intrinsic) value of Extendible American call
    INPUT: two doubles
        s: the spot price
        t: time
    OUTPUT: a double, intrinsic value for the call
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
