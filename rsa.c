#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// find out if number is prime by checking if is divisible for any i value until its square root
// return 0 if not prime; 1 if prime
long long checkIfPrime(long long p){

	long long i;
	double j;
	
	j = sqrt(p);

	for(i = 2; i <= j; i++){
		if(p%i == 0)
			return 0;
	}

	return 1;
}

// chose smaller prime taht can divide euler value
long long choseE(long long phi, long long p, long long p2, long long n){

	long long i, e;
	for(i = 2; i < phi; i++){

		if(phi%i != 0 && checkIfPrime(i) && i != p && i != p2){
			e = i;
			break;
		}
	}

	return e;
}

void division(long long *resto, long long *quociente, long long a, long long b){

	if(a >= 0){

		*quociente = 0;
		*resto = a;

		while(*resto >= b){

			*resto -= b;
			*quociente = *quociente + 1;
		}
	}

}

// Greatest Common Divisor
long long extendedGcd(long long a, long long b){
	
	long long resto, quociente, xB = 1, yB = 0, x = 0, y = 1, alpha, beta, phi;
	phi = a;

	resto = a;
	while(resto != 0){
		division(&resto, &quociente, a, b);
		a = b;
		b = resto;
		if(resto > 0){
			alpha = xB - quociente *x;
			beta = yB - quociente * y;

			xB = x;
			yB = y;
			x = alpha;
			y = beta;
		}
	}

	if(beta < 0)
		beta = phi + beta;

	return beta;

}

long power(long long a, long long e, long long n){
	
	long long A = a, P = 1, E = e;
	
	while(1){

		if(E == 0)
			return P;

		else if(E%2 != 0){
			P = (A * P)%n;
			E = (E-1)/2;
		}

		else{
			E = E/2;
		}
		A = (A*A)%n;
	}
	
}

int *codifica(char *mensagem, long long e, long long n){
	
	long long i;
	int *mensagemC;

	mensagemC = malloc(100 * sizeof(long long));
	for(i = 0; i < 100; i++){
		mensagemC[i] = power(mensagem[i], e, n);
	}

	return mensagemC;
}

char *decodifica(int *mensagemC, long long d, long long n){
	
	long long i;
	char *mensagem;

	mensagem = malloc(100 * sizeof(char));
	
	for(i = 0; i < 100; i++){
		mensagem[i] = power(mensagemC[i], d, n);
	}

	return mensagem;
}

int main(void){

	long long i;
	long long p, p2, n, phi, e, d;

	char mensagem[100];
	int *mensagemC;

	long long primeFlag;
	
	printf("\nEnter first prime value\n");
	scanf("%llu", &p);

	primeFlag = checkIfPrime(p);
	if(!primeFlag){
		printf("That's not a prime value\n");
		return 0;
	}

	printf("\nEnter another prime value\n");
	scanf("%llu", &p2);

	primeFlag = checkIfPrime(p2);
	if(!primeFlag || p == p2){
		printf("That's not a prime value\n");
		return 0;
	}

	printf("\nEnter message\n");
	
	scanf("\n");
	fgets(mensagem, 100, stdin);

	n = p * p2;
	
	phi = (p - 1)*(p2 - 1);

	e = choseE(phi, p, p2, n);
	
	printf("\nPrivate key: (%llu, %llu)\n", e, n);

	d = extendedGcd(phi, e);

	printf("\nPublic key: (%llu, %llu)\n", d, n);

	mensagemC = codifica(mensagem, e, n);
	
	printf("\nEncrypted message: ");

	for(i = 0; i < strlen(mensagem); i++){
		printf("%c", mensagemC[i]);
	}
	printf("\n");	

	char *mensagemD;
	mensagemD = decodifica(mensagemC, d, n);

	printf("\nDecrypted message: %s\n\n", mensagemD);

	free(mensagemC);
	free(mensagemD);
	return 0;

}