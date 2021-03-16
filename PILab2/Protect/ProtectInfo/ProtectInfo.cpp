#include <fstream>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

long int calculateE(long int t);
long int calculateD(long int e, long int t);
long int greatestCommonDivisor(long int e, long int t);


bool IsNatural(long int prime)
{
	long int i, j;

	j = (long int)sqrt((long double)prime);

	for (i = 2; i <= j; i++)
	{
		if (prime % i == 0)
		{
			return false;
		}
	}

	return true;
}

long int encrypt(long int i, long int e, long int n)
{
	long int current, result;

	current = i - 97;
	result = 1;

	for (long int j = 0; j < e; j++)
	{
		result = result * current;
		
	}
	result = result % n;
	return result;
}

long int decrypt(long int i, long int d, long int n)
{
	long int current, result;

	current = i;
	result = 1;

	for (long int j = 0; j < d; j++)
	{
		result = result * current;
		result = result % n;
	}

	return result + 97;
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

	long int encryptedText[100];
	memset(encryptedText, 0, sizeof(encryptedText));
	long int decryptedText[100];
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
	for (long int i = 0; i < message.length(); i++)
	{
		encryptedText[i] = encrypt(message[i], e, n);
	}

	cout << "------encrypt message" << endl;
	for (long int i = 0; i < message.length(); i++)
	{
		cout << (char)encryptedText[i];
	}

	//message = RSA(message, d, n);
	cout << "\n------decrypted message" << endl;
	cout << message;

	return 0;
}



long int calculateE(long int t)
{
	// Выбирается целое число e ( 1 < e < t ) 

	long int e;

	for (e = 2; e < t; e++)
	{
		if (greatestCommonDivisor(e, t) == 1)
		{
			return e;
		}
	}

	return -1;
}

long int calculateD(long int e, long int t)
{
	// Вычисляется число d

	long int d;
	long int k = 1;

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

long int greatestCommonDivisor(long int e, long int t)
{
	while (e > 0)
	{
		long int myTemp;

		myTemp = e;
		e = t % e;
		t = myTemp;
	}

	return t;
}
