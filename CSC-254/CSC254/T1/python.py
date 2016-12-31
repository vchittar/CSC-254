import math
#Yufei (Edward) Du
#Vishnu Chittari
#
#For algorithm behind code check readme.


#append is more like adding and extend is more like stretching
n=int(input("Enter the number:: "))

#prime_partitions: calls prime_partitions_rec to find 2 prime number partitions, 
#uses a loop to call prime_partitions_rec to find 3 prime number partitions,
#and uses nested loops to expend the partitions to generate more answers.
def prime_partitions(n, k):
    result = prime_partitions_rec(n,k)
    primes = (genprime(n))
    i = k
    while i < len(primes):

        gen = prime_partitions_rec(n - primes[i], i + 1)
        if gen != False:
            for index in gen:
                genList = []
                genList.append(primes[i]) #adding a list of primes to genList so i would append
                genList.extend(index) #adding the index array to the genlist so i would extend
                result.append(genList)# the result is the extended???
            i = i+1
    m = 0
    while m < len(result):
        index = result[m]
        j = 0
        while j < len(index):
            tempList = prime_partitions(index[j],0)
            for x in tempList:
                temp=x
                indexTemp = list(index)
                indexTemp.pop(j)
                temp.extend(indexTemp)
                if sum(temp) == n:
                    temptemp = sorted(list(set(temp)))
                    if len(temptemp) == len(temp):
                        if temptemp not in result:
                            result.append(temptemp)
            j = j+1
        m = m+1
    return result

#prime_partitions_rec: Returns the list of 2 prime number partitions
def prime_partitions_rec(input, k):
        primes = genprime(input)
        result = []
        i = k
        while i < len(primes):
            j = (len(primes) - 1)
            while j > i:
                if primes[i] + primes[j] == input:
                    result.append(list((primes[i], primes[j])))

                j -= 1
            i += 1
        return result


#prime gen
#Cite: naive algorithm here: https://en.wikipedia.org/wiki/Primality_test
def genprime(max):
    primes = []
    i = 2
    while i < max:
        if (checkprime(i)):
            primes.append(i)
        i += 1
    return primes


# prime checker
def checkprime(input):
    if input <= 1:
        return False
    elif input <= 3:
        return True
    elif input % 2 == 0 or input % 3 == 0:
        return False
    i = 5
    while i * i <= input:
        if input % i == 0 or input % (i + 2) == 0:
            return False
        i += 6
    return True


print(prime_partitions(n,0))
