import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;

public class RandomPrimeChecker {
	public static void main(String[] args){
		//System.out.println(Arrays.toString(prime_partitions(29,0)));
		//System.out.println(Arrays.toString(genprime(61)));
		int input;
		if(args.length == 0){
			Scanner scan = new Scanner(System.in);
			System.out.println("Input a number");
			input = scan.nextInt();
			scan.close();
		}else{
			input = Integer.parseInt(args[0]);
		}
		ArrayList<Integer[]> result = prime_partitions(input,0);
		for(Integer[] i : result){
			System.out.println(Arrays.toString(i));
		}
	}
	
	public static ArrayList<Integer[]> prime_partitions(int n, int k){
		//ArrayList<Integer[]> result = new ArrayList<Integer[]>();
		ArrayList<Integer[]> result = prime_partitions_rec(n, k);
		//Integer[] gen = prime_partitions_rec(n, k);
		ArrayList<Integer[]> gen = new ArrayList<Integer[]>();
		//if(result.isEmpty()){
			Integer[] primes = genprime(n);
			int i = k;
			while(i < primes.length){
				ArrayList<Integer> genList = new ArrayList<Integer>();
				//genList.add(primes[i]);
				gen = prime_partitions_rec(n-primes[i],i+1);
				//System.out.println("Prime is "+primes[i]+", gen is "+Arrays.toString(gen)+", List "+genList.toString());
				if(gen != null){
					//genList.addAll(Arrays.asList(gen));
					//gen = genList.toArray(new Integer[0]);
					//result.add(gen);
					for(Integer[] index : gen){
						genList.add(primes[i]);
						genList.addAll(Arrays.asList(index));
						result.add(genList.toArray(new Integer[0]));
						genList.clear();
					}
				}
				//System.out.println("n is "+n+" i is "+i +" length is "+ primes.length);
				i++;
				
			}
		//}
		//int iteration = 0;
		for(int m = 0; m < result.size(); m++){
			Integer[] index = result.get(m);
			for(int j = 0; j < index.length; j++){
				ArrayList<Integer[]> tempList = prime_partitions(index[j],0);
				for(Integer[] x : tempList){
					//System.out.println("j is "+j + " length is "+index.length + " tempList is "+tempList.size());
					ArrayList<Integer> temp = new ArrayList<Integer>();
					temp.addAll(Arrays.asList(x));
					ArrayList<Integer> indexTemp = new ArrayList<Integer>();
					indexTemp.addAll(Arrays.asList(index));
					indexTemp.remove(j);
					temp.addAll(indexTemp);
					Integer[] sorted = temp.toArray(new Integer[0]);
					Arrays.sort(sorted);
					temp.clear();
					temp.addAll(Arrays.asList(sorted));
					//genList.add(primes[i]);
					//genList.addAll(Arrays.asList(index));
					//System.out.println(temp.toString());
					if(!hasInternalDuplicate(temp)){
						boolean duplicate = false;
						for(Integer[] current : result){
							if(Arrays.equals(current, temp.toArray(new Integer[0]))){
								duplicate = true;
								break;
							}
						}
						if(!duplicate)
							result.add(temp.toArray(new Integer[0]));
					}
					//genList.clear();
				}
			}
			
		}
		return result;
	}
	
	public static boolean hasInternalDuplicate(ArrayList<Integer> input){
		for(int i = 0; i < input.size(); i++){
			for(int j = 1; j < input.size(); j++){
				if(i != j && input.get(i) == input.get(j))
					return true;
			}
		}
		return false;
	}
	
	
	public static ArrayList<Integer[]> prime_partitions_rec(int input, int k){
		Integer[] primes = genprime(input);
		ArrayList<Integer[]> result = new ArrayList<Integer[]>();
		for(int i = k; i < primes.length; i++){
			//System.out.println("i is "+ i);
			for(int j = primes.length-1; j>i; j--){
				if(primes[i] == 2){
					//System.out.println(primes[i] + "+"+primes[j]);
				}
				if(primes[i]+primes[j]==input){
					//return new Integer[]{primes[i], primes[j]};
					result.add(new Integer[]{primes[i], primes[j]});
				}
			}
		}
		return result;
	}
	
	//Cite: naive algorithm here: https://en.wikipedia.org/wiki/Primality_test
	public static Integer[] genprime(int max){
		ArrayList<Integer> primes = new ArrayList<Integer>();
		for(int i = 2; i < max; i++){
			if(checkprime(i))
				primes.add(i);
		}
		return primes.toArray(new Integer[0]);
	}
	
	public static boolean checkprime(int input){
		if(input <= 1)
			return false;
		else if(input <= 3)
			return true;
		else if(input % 2 == 0 || input % 3 == 0)
			return false;
		for(int i = 5; i * i <= input; i+=6){
			if (input % i == 0 || input % (i+2) == 0)
				return false;
		}
		return true;
	}
}
