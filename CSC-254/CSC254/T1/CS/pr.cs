using System;
using System.Collections.Generic;

public class pr
{
    public static void Main(string[] args)
    {
        //System.out.println(Arrays.toString(prime_partitions(29,0)));
        List<int?[]> result = prime_partitions(args[1],0);

        foreach (int?[] i in result)
        {
            Console.WriteLine(i.ToString());
        }
    }

    public static List<int?[]> prime_partitions(int n, int k)
    {
        //ArrayList<Integer[]> result = new ArrayList<Integer[]>();
        List<int?[]> result = prime_partitions_rec(n, k);
        //Integer[] gen = prime_partitions_rec(n, k);
        List<int?[]> gen = new List<int?[]>();
        //if(result.isEmpty()){
            int?[] primes = genprime(n);
            int i = k;
            while (i < primes.Length)
            {
                List<int> genList = new List<int>();
                gen = prime_partitions_rec(n - primes[i],i);
                //System.out.println("Prime is "+primes[i]+", gen is "+Arrays.`(gen)+", List "+genList.toString());
                if (gen != null)
                {
                    //genList.addAll(Arrays.asList(gen));
                    //gen = genList.toArray(new Integer[0]);
                    //result.add(gen);
                    foreach (int?[] index in gen)
                    {
                        genList.AddRange(primes[i]);
                        genList.AddRange(index.asList());
                        result.AddRange(genList.ToArray());
                        genList.Clear();
                    }
                }
                //System.out.println("n is "+n+" i is "+i +" length is "+ primes.length);
                i++;

            }
        //}
        //int iteration = 0;
        for (int m = 0; m < result.Count; m++)
        {
            int?[] index = result[m];
            for (int j = 0; j < index.Length; j++)
            {
                List<int?[]> tempList = prime_partitions(index[j].Value, 0);
                foreach (int?[] x in tempList)
                {
                    //System.out.println("j is "+j + " length is "+index.length + " tempList is "+tempList.size());
                    List<int?> temp = new List<int?>();
                    temp.AddRange(x.asList(x));
                    List<int?> indexTemp = new List<int?>();
                    indexTemp.AddRange(x.ToList(index));
                    indexTemp.RemoveAt(j);
                    temp.Add(indexTemp);
                    int?[] sorted = temp.ToArray();
                    x.Sort(sorted);
                    temp.Clear();
                    temp.AddRange(x.ToList(sorted));
                    //genList.add(primes[i]);
                    //genList.addAll(Arrays.asList(index));
                    //System.out.println(temp.toString());
                    if (!hasInternalDuplicate(temp))
                    {
                        bool duplicate = false;
                        foreach (int?[] current in result)
                        {
                            if (current.Equals(current, temp.ToArray()))
                            {
                                duplicate = true;
                                break;
                            }
                        }
                        if (!duplicate)
                        {
                            result.AddRange(temp.ToArray());
                        }
                    }
                    //genList.clear();
                }
            }

        }
        return result;
    }

    public static bool hasInternalDuplicate(List<int?> input)
    {
        for (int i = 0; i < input.Count; i++)
        {
            for (int j = 1; j < input.Count; j++)
            {
                if (i != j && input[i] == input[j])
                {
                    return true;
                }
            }
        }
        return false;
    }


    public static List<int?[]> prime_partitions_rec(int input, int k)
    {
        int?[] primes = genprime(input);
        List<int?[]> result = new List<int?[]>();
        for (int i = k + 1; i < primes.Length; i++)
        {
            //System.out.println("i is "+ i);
            for (int j = primes.Length - 1; j > i; j--)
            {
                if (primes[i] + primes[j] == input)
                {
                    //return new Integer[]{primes[i], primes[j]};
                    result.AddRange(new int?[]{primes[i], primes[j]});
                }
            }
        }
        return result;
    }

    //Cite: naive algorithm here: https://en.wikipedia.org/wiki/Primality_test
    public static int?[] genprime(int max)
    {
        List<int?> primes = new List<int?>();
        for (int i = 2; i < max; i++)
        {
            if (checkprime(i))
            {
                primes.AddR(i);
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
