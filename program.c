#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>

long get_usecs (void)
{
    struct timeval t;
    gettimeofday(&t,NULL);
    return t.tv_sec*1000000+t.tv_usec;
}

void usage(const char* app_name) {
    printf("Argument error! Usage: %s <input_file> \n", app_name);
    exit(0);
}

void clear(int* a, int len) {
    for (int index=0; index<len; index++) {
        *(a+index) = 0;
    }
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        usage(argv[0]);
    }

    // Open files
    FILE* input_file = fopen(argv[1], "r");
    if(input_file == NULL) {
        usage(argv[0]);
    }

    // Read the matrix
    unsigned long dim = 0;
    fscanf(input_file, "%lu\n", &dim);
    int **mat = malloc(dim*sizeof(int*));
    for(int i = 0; i < dim; i++) mat[i] = malloc(dim*sizeof(int));

    int element = 0;
    for(int i=0; i<dim; i++) {
        for(int j=0; j<dim; j++) {
            if (j != (dim-1)) 
                fscanf(input_file, "%d\t", &element);
            else 
                fscanf(input_file, "%d\n",&element);
            mat[i][j] = element;
        }
    }

#ifdef _PRINT_INFO
    // Print the matrix
    printf("Input matrix [%d]\n", dim);
    for(int i=0; i<dim; i++) {
        for(int j=0; j<dim; j++) {
            printf("%d\t", mat[i][j]);
        }
        printf("\n");
    }
#endif

    long alg_start = get_usecs();

    int ps[dim][dim];
    for (int j=0; j<dim; j++) {
        ps[0][j] = mat[0][j];
        for (int i=1; i<dim; i++) {
            ps[i][j] = ps[i-1][j] + mat[i][j];
        }
    }

    int max_sum = mat[0][0];
    int top = 0, left = 0, bottom = 0, right = 0; 

    int sum[dim];
    int pos[dim];
    int local_max;

    for (int i=0; i<dim; i++) {
        for (int k=i; k<dim; k++) {
            clear(sum, dim);
            clear(pos, dim);
            local_max = 0;

            sum[0] = ps[k][0] - (i==0 ? 0 : ps[i-1][0]);
            for (int j=1; j<dim; j++) {
                if (sum[j-1] > 0){
                    sum[j] = sum[j-1] + ps[k][j] - (i==0 ? 0 : ps[i-1][j]);
                    pos[j] = pos[j-1];
                } 
                else {
                    sum[j] = ps[k][j] - (i==0 ? 0 : ps[i-1][j]);
                    pos[j] = j;
                }
                if (sum[j] > sum[local_max]){
                    local_max = j;
                }
            } //Kandane ends here

            if (sum[local_max] > max_sum) {
                // sum[local_max] is the new max value
                // the corresponding submatrix goes from rows i..k.
                // and from columns pos[local_max]..local_max

                max_sum = sum[local_max];
                top = i;
                left = pos[local_max];
                bottom = k;
                right = local_max;
            }
        }
    }

    // Compose the output matrix
    int outmat_row_dim = bottom - top + 1;
    int outmat_col_dim = right - left + 1;
    int outmat[outmat_row_dim][outmat_col_dim];
    for(int i=top, k=0; i<=bottom; i++, k++) {
        for(int j=left, l=0; j<=right ; j++, l++) {
            outmat[k][l] = mat[i][j];
        }
    }
    long alg_end = get_usecs();

    // Print output matrix
    printf("Sub-matrix [%dX%d] with max sum = %d, left = %d, top = %d, right = %d, bottom = %d\n", outmat_row_dim, outmat_col_dim, max_sum, left, top, right, bottom);

    // Release resources
    fclose(input_file);

    // Print stats
    printf("%s,arg(%s),%s,%f sec\n", argv[0], argv[1], "CHECK_NOT_PERFORMED", ((double)(alg_end-alg_start))/1000000);
    return 0;
}
