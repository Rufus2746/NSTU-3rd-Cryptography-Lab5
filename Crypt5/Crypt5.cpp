#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

const int N = 15;   //Мощность алфавита

int input(string path){   //Ввод данных из файлов
   int data = 0;
   ifstream file(path);
   if(file.is_open()){
      file >> data;
      file.close();
   } else{
      cerr << "Error: can't open the input file." << endl;
   }
   return data;
}

int generate(int seed, int a, int c){   //Линейный конгруэнтный генератор
   seed = (a * seed + c) % N;
   return seed;
}

double sequenceTest(list<int> sequence, int period){
   auto seqFront = sequence.begin();
   double kDouble = 5*log2(N);
   int k = round(kDouble);   //Округление для нахождения elem
   double p = 1/kDouble;
   int elems = period/k;   //Кол-во элементов в каждом интервале

   double sum = 0;
   for(int i = 0; i<k-1; i++){
      list<int> unique;
      for(int j = i*elems; j<elems+i*elems; j++){
         advance(seqFront, j);
         unique.push_back(*seqFront);
      }
      int nu = sequence.size();   //Кол-во уникальных номеров
      sum += pow(nu/N-p, 2)/p;
   }
   double S = N*sum;
   return S;
}

void output(list<int> sequence, int period, double pearson, string path){   //Вывод последовательности, периода и оценки критерия Пирсона в файл
   ofstream file(path);
   if(file.is_open()){
      file << "Period: " << period << endl;
      file << "Pearson: " << pearson << endl;
      file << "Sequence: [";
      for(auto const &i: sequence){
         file << i << " ,";
      }
      file.seekp(-2, ios_base::cur);
      file << "]";
      file.close();
   } else{
      cerr << "Error: can't open file " << path << endl;
   }
}

void mRand(){
   int seed = input("seed.txt");
   int a = input("a.txt");
   int c = input("c.txt");
   int firstSeed = seed, period = 1;
   list<int> sequence = {seed};

   do{   //Генерация всей последовательности
      seed = generate(seed, a, c);
      sequence.push_back(seed);
      period++;
   } while(seed != firstSeed);

   double pearson = sequenceTest(sequence, period);
   output(sequence, period, pearson, "output.txt");
}

void main(){
   mRand();
}