#include<stdio.h>

char factorial(char n);

int main(int argv,char **argc) {

	float vec[6];
	float *vp=&(vec[0]);
	vp = vp+3;

	printf("dasdfasdf %d",vp);


	int n,fn;

	for(fn=n=1; fn>0; n++) {
		fn=factorial(n);
		printf("factorial(%d)=%d\n",n,fn);
	}
	return(0);

}

char factorial(char n) {
	int i,fn;
	i=n;
	fn=1;
	for(i; i>0; i--) {
		fn*=i;
	}
	return fn;
}

