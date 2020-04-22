/*!
  \file
  \brief Calculating median of values read from STDIN

  \mainpage Median calculator

  This small program reads in input data from STDIN, calculates the median 
  value of this input and outputs it to STDOUT.
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double *read_data(FILE *fp, int *nr_values);
double median(const double *values, const int nr_values);

int main(int argc, char *argv[]) {
    int nr_values = 0;
    double *values = read_data(stdin, &nr_values);
    printf("%lf\n", median(values, nr_values));
    free(values);
    return 0;
}

/*!
  \brief Read in a given number of values from a given file stream.

  Allocates memory and stores a given number of input values in it. Outputs
  error in case memory cannot be allocated. Function returns if no more values
  can be read, in case nr_values is higher than actual number of input values.
  \param fp        Pointer to a file stream where to read input values from.
  \param nr_values The number of values to read from file stream. Value might
                   change if less values can be read than anticipated. 
  \return Returns a pointer to the memory block where input values are stored.
*/
double *read_data(FILE *fp, int *nr_values) {
    const int increment = 1000;
    int current_size = increment;
    double value;
    double *values = (double *) malloc(current_size*sizeof(double));
    if (values == NULL)
        errx(EXIT_FAILURE, "### error: can't allocate %d byts\n", current_size);
    *nr_values = 0;
    while (fscanf(fp, "%lf", &value) == 1) {
        if (*nr_values == current_size) {
            current_size += increment;
            values = (double *) realloc(values, sizeof(double)*current_size);
            if (values == NULL)
                errx(EXIT_FAILURE, "### error: can't allocate %ld byts\n",
                     sizeof(double)*current_size);
        }
        values[(*nr_values)++] = value;
    }
    return values;
}

/*!
  \brief Prints a number of elements of an array.

  The first n elements of an array of doubles is printed.
  \param values Pointer to value array that shall be printed.
  \param n      Number of values to print from array.
*/
void print_data(const double *values, const int n) {
    for (int i = 0; i < n; i++)
        printf("%10.2lf", values[i]);
    printf("\n");
}

/*!
  \brief Compares to values interpreted as doubles

  Input values are casted to doubles and compared. Returns either -1, 0, or 1.
  \param v1 First value
  \param v2 Second value
  \return Returns -1 if v1 is smaller than v2, 1 if v1 is bigger than v2, and
                  0 otherwise. 
*/
int value_cmp(const void *v1, const void *v2) {
    double *d1 = (double *) v1;
    double *d2 = (double *) v2;
    if (*d1 < *d2)
        return -1;
    else if (*d1 > *d2)
        return 1;
    else
        return 0;
}

/*!
  \brief Calculates median of a given number of elements of an array of values

  Calculats the median value of an array of values considering the first
  nr_values elements in the array. Uses quicksort on a temporary created array
  to calculate median.
  \param values    Array of values to calculate median for.
  \param nr_values Number of values of the array to consider for calculating
                   the median.
  \return Median value
*/
double median(const double *values, const int nr_values) {
    double *tmp = (double *) malloc(nr_values*sizeof(double));
    if (tmp == NULL)
        errx(EXIT_FAILURE, "### error: can't allocate %ld byts\n",
             sizeof(double)*nr_values);
    memcpy(tmp, values, nr_values*sizeof(double));
    qsort(tmp, nr_values, sizeof(double), value_cmp);
    double median;
    if (nr_values % 2 == 0)
        median = (tmp[nr_values/2 - 1] + tmp[nr_values/2])/2.0;
    else
        median = tmp[nr_values/2];
    free(tmp);
    return median;    
}
