#include<stdio.h>

short int factorial(short int n);

int main(int argv,char **argc) {

	int n,fn;

	for(fn=n=1; fn>0; n++) {
		fn=factorial(n);
		printf("factorial(%d)=%d\n",n,fn);
	}
	return(0);

}

short int factorial(short int n) {
	int i,fn;
	i=n;
	fn=1;
	for(i; i>0; i--) {
		fn*=i;
	}
	return fn;
}
