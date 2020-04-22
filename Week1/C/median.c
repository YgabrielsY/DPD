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

void print_data(const double *values, const int n) {
    for (int i = 0; i < n; i++)
        printf("%10.2lf", values[i]);
    printf("\n");
}

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
