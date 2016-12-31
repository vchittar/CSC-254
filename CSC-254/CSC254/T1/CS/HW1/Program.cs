using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
//Yufei (Edward) Du
//Vishnu Chittari
//
//For algorithm behind code check readme.
namespace HW1
{
    public class Program
    {
		//Main: Input Output things
        public static void Main(string[] args)
        {
            
            List<int[]> result = prime_partitions(Int32.Parse(args[0]), 0);
            foreach (int[] i in result)
            {
                for (int j = 0; j < i.Length; j++)
                {
                    Console.Write(" " + i[j]);
                }
                Console.WriteLine();
            }
        }

		//prime_partitions: calls prime_partitions_rec to find 2 prime number partitions, 
		//uses a loop to call prime_partitions_rec to find 3 prime number partitions,
		//and uses nested loops to expend the partitions to generate more answers.
        public static List<int[]> prime_partitions(int n, int k)
        {
            List<int[]> result = prime_partitions_rec(n, k);
            List<int[]> gen = new List<int[]>();
            int[] primes = genprime(n);
            int i = k;
            while (i < primes.Length)
            {
                List<int> genList = new List<int>();
                gen = prime_partitions_rec(n - primes[i], i+1);
                if (gen != null)
                {
                    foreach (int[] index in gen)
                    {
                        genList.Add(primes[i]);
                        genList.AddRange(index);
                        result.Add(genList.ToArray());
                        genList.Clear();
                    }
                }
                i++;

            }
            for (int m = 0; m < result.Count; m++)
            {
                int[] index = result[m];
                for (int j = 0; j < index.Length; j++)
                {
                    List<int[]> tempList = prime_partitions(index[j], 0);
                    foreach (int[] x in tempList)
                    {
                        List<int> temp = new List<int>();
                        temp.AddRange(x);
                        List<int> indexTemp = new List<int>();
                        indexTemp.AddRange(index);
                        indexTemp.RemoveAt(j);
                        temp.AddRange(indexTemp);
                        int[] sorted = temp.ToArray();
                        Array.Sort(sorted);
                        temp.Clear();
                        temp.AddRange(sorted);
                        if (temp.Count == temp.Distinct().Count())
                        {
                            bool duplicate = false;
                            foreach (int[] current in result)
                            {
                                if (current.SequenceEqual(temp.ToArray()))
                                {
                                    duplicate = true;
                                    break;
                                }
                            }
                            if (!duplicate)
                            {
                                result.Add(temp.ToArray());
                            }
                        }
                    }
                }

            }
            return result;
        }

		//prime_partitions_rec: Returns the list of 2 prime number partitions
        public static List<int[]> prime_partitions_rec(int input, int k)
        {
            int[] primes = genprime(input);
            List<int[]> result = new List<int[]>();
            for (int i = k; i < primes.Length; i++)
            {
                for (int j = primes.Length - 1; j > i; j--)
                {
                    if (primes[i] + primes[j] == input)
                    {
                        result.Add(new int[] { primes[i], primes[j] });
                    }
                }
            }
            return result;
        }
		//genprime Generate an array of prime numbers that are less than max
        //Cite: naive algorithm here: https://en.wikipedia.org/wiki/Primality_test
        public static int[] genprime(int max)
        {
            List<int> primes = new List<int>();
            for (int i = 2; i < max; i++)
            {
                if (checkprime(i))
                {
                    primes.Add(i);
                }
            }
            return primes.ToArray();
        }

        public static bool checkprime(int input)
        {
            if (input <= 1)
            {
                return false;
            }
            else if (input <= 3)
            {
                return true;
            }
            else if (input % 2 == 0 || input % 3 == 0)
            {
                return false;
            }
            for (int i = 5; i * i <= input; i += 6)
            {
                if (input % i == 0 || input % (i + 2) == 0)
                {
                    return false;
                }
            }
            return true;
        }
    }
}
