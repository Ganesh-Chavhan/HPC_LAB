#include <iostream>
#include <vector>
#include <omp.h>
#include <cmath>
using namespace std;

double horner(const vector<double>& coeff, double x) {
    double result = coeff[0];
    for (size_t i = 1; i < coeff.size(); i++)
        result = result * x + coeff[i];
    return result;
}

double direct(const vector<double>& coeff, double x) {
    double result = 0.0;
    size_t deg = coeff.size();
    for (size_t i = 0; i < deg; i++)
        result += coeff[i] * pow(x, deg - i - 1);
    return result;
}

int main() {
    vector<double> coeff = {2, -6, 2, -1};
    double x = 3.0;
    double res1 = 0.0, res2 = 0.0;
    double t1 = 0.0, t2 = 0.0;

    double start = omp_get_wtime();
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            double s1 = omp_get_wtime();
            res1 = horner(coeff, x);
            t1 = omp_get_wtime() - s1;
        }

        #pragma omp section
        {
            double s2 = omp_get_wtime();
            res2 = direct(coeff, x);
            t2 = omp_get_wtime() - s2;
        }
    }
    double end = omp_get_wtime();

    cout << "Horner's Result: " << res1 << " Time: " << t1 << " sec" << endl;
    cout << "Direct Result : " << res2 << " Time: " << t2 << " sec" << endl;
    cout << "Final Result (Chosen): " << res1 << endl;
    cout << "Total Parallel Time: " << end - start << " sec" << endl;
    cout << "Wasted Computation Time: " << (t1 > t2 ? t1 - t2 : t2 - t1) << " sec" << endl;
    return 0;
}
