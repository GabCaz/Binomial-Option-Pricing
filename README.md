# Options pricing with Binomial tree
Price various options using Binomial trees, the Black-Scholes formula, or a combination thereof. For example, European and American calls and puts, reloadable options, and extendible options can be priced.

The parent class “Options” creates the binomial tree, and it then suffices to specify the payoff rules in derived classes (corresponding to a specific type of option) to price it using the Binomial tree. To illustrate, I created a couple such derived classes.

The tree is implemented using a single vector, which is efficient in terms of memory. However, it does not allow to easily price options which have very strong path-dependency, such as knockout options.
