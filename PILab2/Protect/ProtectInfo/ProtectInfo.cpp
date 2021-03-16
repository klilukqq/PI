#include <fstream>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

 int calculateE( int t);
 int calculateD( int e,  int t);
 int greatestCommonDivisor( int e,  int t);


bool IsNatural( int prime)
{
	 int i, j;

	j = ( int)sqrt(( double)prime);

	for (i = 2; i <= j; i++)
	{
		if (prime % i == 0)
		{
			return false;
		}
	}

	return true;
}

 int encrypt( int i,  int e,  int n)
{
	 int current, result;

	current = i - 224;
	result = 1;

	for ( int j = 0; j < e; j++)
	{
		result = result * current;
		
	}
	result = result % n;
	return result;
}
//русский 224 английский 97
 int decrypt( int i,  int d,  int n)
{
	 int current, result;

	current = i;
	result = 1;

	for ( int j = 0; j < d; j++)
	{
		result = result * current;
		result = result % n;
	}

	return result + 224;
}

int InputNum() {
	int num;
	bool flag;
	do {
		cout << "Input natural number" << endl;
		cin >> num;
		flag = IsNatural(num);

		if (flag == false)
		{
			cout << "\nInput natural number\n" << endl;
		}
	} while (flag == false);
	return num;
}

int main() {
	//ввод чисел

	int first_num, second_num;

	 int encryptedText[100];
	memset(encryptedText, 0, sizeof(encryptedText));
	 int decryptedText[100];
	memset(decryptedText, 0, sizeof(decryptedText));

	//ввод начальных чисел
	first_num = InputNum(); 
	second_num = InputNum();

	//вычисления
	int phi, n, e;
	n = first_num * second_num;
	phi = (first_num - 1) * (second_num - 1);
	
	int k = 1;
	int d;

	e = calculateE(phi);

	d = calculateD(e, phi);

	cout << "\nPublic (n = " << n << ", e = " << e << ")" << endl;
	cout << "Private (n = " << n << ", d = " << d << ")" << endl;

	string message;
	cout << "Input message " << endl;

	cin.ignore();
	getline(cin, message);
	cout << "\nThe message is: " << message << endl;
	//кодировка и раскодировка
	for ( int i = 0; i < message.length(); i++)
	{
		encryptedText[i] = encrypt(message[i], e, n);
	}

	cout << "------encrypt message" << endl;
	for ( int i = 0; i < message.length(); i++)
	{
		cout << (char)encryptedText[i];
	}

	//message = RSA(message, d, n);
	cout << "\n------decrypted message" << endl;
	cout << message;

	return 0;
}



 int calculateE( int t)
{
	// Выбирается целое число e ( 1 < e < t ) 

	 int e;

	for (e = 2; e < t; e++)
	{
		if (greatestCommonDivisor(e, t) == 1)
		{
			return e;
		}
	}

	return -1;
}

 int calculateD( int e,  int t)
{
	// Вычисляется число d

	 int d;
	 int k = 1;

	while (1)
	{
		k = k + t;

		if (k % e == 0)
		{
			d = (k / e);
			return d;
		}
	}

}

 int greatestCommonDivisor( int e,  int t)
{
	while (e > 0)
	{
		 int myTemp;

		myTemp = e;
		e = t % e;
		t = myTemp;
	}

	return t;
}
