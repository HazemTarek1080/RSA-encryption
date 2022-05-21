// RSA.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<cmath>
#include<math.h>
#include<iostream>
#include<time.h>
#include <thread>
#include<fstream>
using namespace std;

unsigned int gcd(unsigned long long a, unsigned long long b) {
	unsigned long long x = a;
	unsigned long long y = b;
	while (y != 0) {
		unsigned int r = fmod(x, y);
		x = y;
		y = r;
	}
	return x;
}//uses the euclidean algorithm to find the greatest common divisor
unsigned int generateexponent(unsigned int p, unsigned int q) {
	srand(time(0));
	unsigned int z = (p - 1)*(q - 1);
	unsigned int e = rand()%50;
	while (gcd(e, z) != 1) {
		e = rand()%50;
	}
	return e;
}//generates a random number in the interval [0,50] and tests whether it is coprime to (p-1)(q-1) or not 
//if not it generates another random number till it reaches a number that is coprime to (p-1)(q-1)

unsigned long long generateinverse(unsigned int e, unsigned int p, int q) {
	unsigned long long n = (p-1)*(q-1);
	int k = 0;
	do {
		k++;
	} while (fmod((n*k + 1), e) != 0);
	return (n*k+1)/e;
}//generates the modular multiplicative inverse using the ab mod m = 1 which gives us (mk+1/b = a) where a and k are integers
//it keeps iterating till it finds an integer k such that a is an integer

unsigned int generateprimehelper(unsigned int &x) {
	for (int i = 2; i <= (int)sqrt(x); i++) {
		if (fmod(x, i) == 0) {
			x++;
			return generateprimehelper(x);
		}
	}
	return x;
}
unsigned int generateprime() {
	unsigned int randomnumber = (rand()+1000)%9999;
	for (unsigned int i = 2; i <= (unsigned int)sqrt(randomnumber); i++) {
		if (fmod(randomnumber, i) == 0) {
			randomnumber++;
			return generateprimehelper(randomnumber);
		}
	}
	return randomnumber;
}//generates a random number in the range [1000,9999] and checks its primality, if it is not prime it increments the number and checks its
//primality till it finds a prime number

unsigned long long ModularExp(unsigned long long base, unsigned long long exponent, unsigned long long Modulus)
{
	int x = 1;
	unsigned long long power= fmod(base,Modulus); 

	while (exponent > 0)
	{
		if (exponent & 1)
			x =fmod ((power*x),Modulus);
		exponent = exponent >> 1; 
		power = fmod((power*power),Modulus);
	}
	return x;
}//implements modular exponentiation using the pseudocode presented in the textbook

unsigned long long cipher(unsigned int e, unsigned int p, unsigned int q, unsigned int M) {
	return ModularExp(M, e, p*q);//encrypts the text
}
unsigned long long decipher(unsigned int d, unsigned int p, unsigned int q, unsigned long long c) {
	return ModularExp(c,d, p*q);//decrypts the text
}


int main()
{
	srand(time(0));
	unsigned int p = generateprime();//generates the first prime number
	std::this_thread::sleep_for(1s);
	unsigned int q = generateprime();//generates the second prime nu,ber
	unsigned int e = generateexponent(p, q);//generates the exponent
	unsigned long long d = generateinverse(e, p, q);//generates the inverse of the exponent
	ifstream inputfile;
	inputfile.open("C:\\Users\\ahmed\\source\\repos\\RSA\\Input.txt");//please change the file path before running
	//opens the plain text file
	if (!inputfile) {
		cerr << "Unable to open file Input.txt";
		exit(1);   // call system to stop
	}
	char c;
	int k = 0;
	char* input= new char[30000];
	while (inputfile.get(c)) {
		input[k] = (int)c;
		input[k];
		k++;
	}//reads the file character by character and stores the information in an array of characters
	inputfile.close();
	ofstream encrypted;
	encrypted.open("C:\\Users\\ahmed\\source\\repos\\RSA\\encrypted.txt");//please change the file path before running
	//opens the file where we will store the encrypted information
	if (!encrypted) {
		cerr << "Unable to open file encrypted.txt";
		exit(1);   // call system to stop
	}
	
	for (int i = 0; i < k; i++) {
		encrypted << cipher(e, p, q, (unsigned long long)input[i]) << endl;
	}//encrypts the information
	encrypted.close();
	ifstream fordecryption;
	fordecryption.open("C:\\Users\\ahmed\\source\\repos\\RSA\\encrypted.txt");//please change the file path before running
	//reopens the encrypted file to extract the data
	if (!fordecryption) {
		cerr << "Unable to open file encrypted.txt";
		exit(1);   // call system to stop
	}
	unsigned long long* inputcoded = new unsigned long long[k];
	int temp = 0;
	int i = 0;
	while (fordecryption>>temp) {
		inputcoded[i] = temp;
		i++;
	}//stores the encrypted data in an array
	fordecryption.close();
	ofstream decrypted;
	decrypted.open("C:\\Users\\ahmed\\source\\repos\\RSA\\decrypted.txt");//please change the file path before running
    //opens the file where we will store the new decrypted information
	if (!decrypted) {
		cerr << "Unable to open file decrypted.txt";
		exit(1);   // call system to stop
	}
	for (int i = 0; i < k; i++) {
		decrypted << (char)decipher(d, p, q, inputcoded[i]);
	}//decrypts the data and stores it in the new file
	decrypted.close();
}