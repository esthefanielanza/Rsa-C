#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//Descobre se um número é primo verificando se ele é divisível para qualquer i até a sua raiz quadrada
long long verificaPrimo(long long p){

	long long i;
	double j;
	
	//Calcula a raiz quadrada para p
	j = sqrt(p);


	for(i = 2; i <= j; i++){
		//Retorna 0 caso não seja um número primo
		if(p%i == 0)
			return 0;
	}

	//Retorna 1 quando é um número primo
	return 1;
}

//Escolhe o menor primo que divide o coeficiente de euler. Obs: Ele deve ser diferente de p e p2.
long long escolheE(long long phi, long long p, long long p2, long long n){

	long long i, e;
	for(i = 2; i < phi; i++){

		if(phi%i != 0 && verificaPrimo(i) && i != p && i != p2){
			e = i;
			break;
		}
	}

	return e;
}

//Calcula o resto e o quociente de uma divisão
void divisao(long long *resto, long long *quociente, long long a, long long b){

	if(a >= 0){

		*quociente = 0;
		*resto = a;

		while(*resto >= b){

			*resto -= b;
			*quociente = *quociente + 1;
		}
	}

}

//Calcula o mdc estendido e retorna o beta(inverso do e módulo phi) para ser o d
long long mdcEstendido(long long a, long long b){
	
	long long resto, quociente, xB = 1, yB = 0, x = 0, y = 1, alpha, beta, phi;
	phi = a;

	resto = a;
	while(resto != 0){
		divisao(&resto, &quociente, a, b);
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

//Calcula a forma reduzida de a^e módulo n, usando a expansão binária do expoente
long potencia(long long a, long long e, long long n){
	
	long long A = a, P = 1, E = e;
	
	while(1){

		//Chegou ao fim da expansão, retorna o P
		if(E == 0)
			return P;

		//Se o expoente é ímpar
		else if(E%2 != 0){
			//Realizamos a redução módulo n de cada uma das multpilicações 			
			P = (A * P)%n;
			E = (E-1)/2;
		}

		//Se o expoente é par
		else{
			E = E/2;
		}
		//Obtém a sequência de potências
		A = (A*A)%n;
	}
	
}

//Codifica uma string de caracteres usando o resto da divisão de a^e por n para cada caractere, para a é utilizado o código da tabela ASCII
int *codifica(char *mensagem, long long e, long long n){
	
	long long i;
	int *mensagemC;

	mensagemC = malloc(100 * sizeof(long long));
	for(i = 0; i < 100; i++){
		mensagemC[i] = potencia(mensagem[i], e, n);
	}

	//Retorna um vetor de long longeiros
	return mensagemC;
}

//Decodifica um vetor de inteiros em uma string de caracteres usando o resto da divisão de a^d por n para cada inteiro
char *decodifica(int *mensagemC, long long d, long long n){
	
	long long i;
	char *mensagem;

	mensagem = malloc(100 * sizeof(char));
	
	for(i = 0; i < 100; i++){
		mensagem[i] = potencia(mensagemC[i], d, n);
	}

	return mensagem;
}

//Programa principal
int main(void){

	long long i;
	long long p, p2, n, phi, e, d;

	//A mensagem a ser criptografada pode ter no máximo 100 caracteres
	char mensagem[100];
	int *mensagemC;

	//Verifica se é um primo
	long long primoFlag;
	
	//Lê um primo do teclado
	printf("\nDigte o primeiro número primo\n");
	scanf("%llu", &p);

	//Verifica se o mesmo é um primo, caso contrário finalza a execução
	primoFlag = verificaPrimo(p);
	if(!primoFlag){
		printf("Você não digitou um primo\n");
		return 0;
	}

	//Faz a mesma coisa para o segundo primo
	printf("\nDigite outro número primo\n");
	scanf("%llu", &p2);

	primoFlag = verificaPrimo(p2);
	if(!primoFlag || p == p2){
		printf("\nVocê não digitou um primo\n");
		return 0;
	}

	//Lê a mensagem a ser criptografada
	printf("\nDigite uma mensagem\n");
	
	//Limpa o buuffer
	scanf("\n");
	fgets(mensagem, 100, stdin);

	//Calcula o n
	n = p * p2;
	
	//Calcula o quociente de euler
	phi = (p - 1)*(p2 - 1);

	//Escolhe o e para calcular a chave privada
	e = escolheE(phi, p, p2, n);
	
	printf("\nChave privada: (%llu, %llu)\n", e, n);

	//Escolhe o d para calcular a chave pública
	d = mdcEstendido(phi, e);

	printf("\nChave publica: (%llu, %llu)\n", d, n);

	//Codifica a mensagem
	mensagemC = codifica(mensagem, e, n);
	
	//Imprime a mensagem codificada
	printf("\nMensagem encriptada: ");

	for(i = 0; i < strlen(mensagem); i++){
		printf("%c", mensagemC[i]);
	}
	printf("\n");	

	//Decodifica a mensagem
	char *mensagemD;
	mensagemD = decodifica(mensagemC, d, n);

	printf("\nMensagem desencriptada: %s\n\n", mensagemD);

	//Libera memória alocada
	free(mensagemC);
	free(mensagemD);
	return 0;

}