#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* 
 * the below function "read_from_file" gets as argument filename as a string
 * reads the content and returns quadratic equation as a string
 */
char* read_from_file(const char* filename)
{
	static char c[1000];
	FILE *fptr;
	if ((fptr = fopen(filename, "r")) == NULL) {
		printf("Error! File cannot be opened.!!!!!\n");
		exit(1);
	}
	fscanf(fptr, "%[^\n]", c);
	fclose(fptr);
	return &(c[0]);
}

/* 
 * the below function "solve_quad" gets as argument a b c
 * solves the equation and returns roots if any
 */
double* solve_quad(double* arg_ptr)
{
	double *x1_x2 = malloc(2);
	double a = arg_ptr[0], b = arg_ptr[1], c = arg_ptr[2];
	double det = b * b - 4 * a * c;
	if(0 == det)
	{
		x1_x2[0] = -b/(2.0*a);
		x1_x2[1] = x1_x2[0];
	} else if(det > 0)
	{
	   x1_x2[0] = (-b+sqrt(det))/(2*a);
	   x1_x2[1] = (-b-sqrt(det))/(2*a);
	} else {
	    printf("No Solution. \n");
	    return x1_x2;
	}
	return x1_x2;
}

/* 
 * the below function "get_abc" gets as argument quadratic equatation
 * as string parses it and returns a b c as double type array
 */
double* get_abc(const char* arg_expr, unsigned int sz)
{
	static double abc[] = {0, 0, 0};
	char tmp[50] = "";
	int ascii = 0;
	for(int i = 0; i < sz; ++i) {
		if('=' == arg_expr[i]){
			break;
		}
		ascii = arg_expr[i];
		if(' ' == arg_expr[i]) {continue;}
		if((ascii >= 48 && ascii <= 57) || arg_expr[i] == '.') {
			strncat(tmp, &arg_expr[i], 1);
			continue;
		} else if('^' == arg_expr[i]) {
			abc[0] += strtod(tmp, NULL);
			strcpy(tmp, "");
			i += 2;
		} else if(ascii >= 97 && ascii <= 122) {
			if(0 == i || 0 == strcmp(tmp,"-")){
				char num = '1';
				strncat(tmp, &num, 1);
			}
			if('^' != arg_expr[i+1]) {
				if(1 == strlen(tmp)){
					char int_ch = '1';
					strncat(tmp, &int_ch, 1);
				}
				abc[1] += strtod(tmp, NULL);
				strcpy(tmp, "");
			}
		} else if('-' == arg_expr[i] || '+' == arg_expr[i]) {
			if(0 == strlen(tmp)) {
				strncat(tmp, &arg_expr[i], 1);
			} else if(0 < strlen(tmp)){
				abc[2] += strtod(tmp, NULL);
				strcpy(tmp, "");
				strncat(tmp, &arg_expr[i], 1);
			}
			if(arg_expr[i+1] )
			continue;
		}
	}
	if(0 == abc[2]) { abc[2] = strtod(tmp, NULL); }
	return &(abc[0]);
}

/* 
 * the below function "test" gets as argument roots if any
 * compares them with those in golden.txt and prints command line message
 * (the last is optional the purpose  is technical)
 */
void test(double* roots)
{
	FILE *fptr;
	fptr = fopen("golden.txt", "r");
	if(fptr == NULL){
		printf("file can not be opened\n");
		exit(0);
	}
	char x1_str[50], x2_str[50];
	fscanf(fptr, " %50s", x1_str);
	fscanf(fptr, " %50s", x2_str);
	double x1 = strtod(x1_str, NULL);
	double x2 = strtod(x2_str, NULL);
	if((roots[0] == x1 && roots[1] == x2) || (roots[0] == x2 && roots[1] == x1)) {
		printf("Right solution: x1 = %f x2 = %f\n", x1, x2);
	} else {
		printf("The solution is wrong: x1 = %f x2 = %f\n", roots[0], roots[1]);
		printf("Right solution should be: x1 = %f x2 = %f\n", x1, x2);
	}
	fclose(fptr);
	return;
}

/* 
 * the below function "create_output_file" gets as argument roots if any
 * and generates an "output.txt" file containing the results
 */
void create_output_file(double *roots){
	char x0[50];
	char x1[50];
	snprintf(x0, 50, "%f", roots[0]);
	snprintf(x1, 50, "%f", roots[1]);
	char whitespaces[] = "   ";
	strcat(x0, whitespaces);
	strcat(x0, x1);
	FILE * fptr;
	fptr = fopen("output.txt", "w");
	fwrite(x0, sizeof(x0[0]), sizeof(x0), fptr);
	fclose(fptr);
}

int main(int argc, char** argv)
{
	const char* equation = read_from_file(argv[1]);
	double *abc = get_abc(equation, strlen(equation));
	double *roots = solve_quad(abc);
	test(roots);
	create_output_file(roots);
	return 0;
}
