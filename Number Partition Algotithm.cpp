#include<iostream> 

using namespace std;

int isPrime(int n) {
    if (n == 2) {
        return 1;
    }
    if (n % 2 == 0) {
        return 0;
    }
    else {
        for (int i = 3;i < n;i+=2) {
            if (n % i == 0) {
                return 0;
            }
        }
        return 1;
    }
}

void printCorrectArray(int p[], int n)
{   
    int hasOnes = 0;
    int onlyPrimes = 1;
    for (int i = 0; i < n; i++) {
        if (p[i] == 1) {
            hasOnes=1;
            break;
        }
    }
    if (!hasOnes) {
        for (int i = 0;i < n;i++) {
            if (!isPrime(p[i])) {
                onlyPrimes = 0;
            }
        }
    }
    if (!hasOnes && onlyPrimes) {
        for (int i = 0; i < n; i++)
            cout << p[i] << " ";
        cout << endl;
    }
}

void printParts(int n)
{
    int partitions[128];
    int k = 0;
    partitions[k] = n; 
    while (true)
    {
        printCorrectArray(partitions, k + 1);
        int removed_value = 0;
        while (k >= 0 && partitions[k] == 1)
        {
            removed_value += partitions[k];
            k -= 1;
        }

        if (k < 0) {
            return;
        }
        
        partitions[k] -= 1;
        removed_value += 1;

        while (removed_value > partitions[k])
        {
            partitions[k + 1] = partitions[k];
            removed_value = removed_value - partitions[k];
            k += 1;
        }

        partitions[k + 1] = removed_value;
        k += 1;
    }
}

int main()
{    
    int number;
    cin >> number;
    printParts(number);
    return 0;
}