#define HAVE_STRUCT_TIMESPEC

#include <iostream>
#include <queue>
#include <string>
#include <pthread.h>
#include <ctime>


using namespace std;

// Генератор шифра
void RndString(char* A, int size) {
	for (int i = 0; i < size; i += 2) {
		switch (rand() % 3)
		{
		case 0:
			A[i] = 0;
			A[i + 1] = rand() % 10;
			break;
		case 1:
			A[i] = 1;
			A[i + 1] = rand() % 10;
		case 2:
			A[i] = 2;
			A[i + 1] = rand() % 6;
			break;
		}
	}
}

// Вывод
void Out(char* A, int size) {
	for (int i = 0; i < size; i++) {
		cout << A[i] << "   ";
	}
	cout << "\n";
}

struct Task {
	string cypher;
	int size;
	Task(char* a, const int& _size) {
		cypher = a;
		size = _size;
	}
};

bool convert(Task t, string* res) {
	string r;
	for (int i = 0; i < t.size; i+=2) {
		switch (t.cypher[i])
		{
		case 0:
			switch (t.cypher[i+1])
			{
			case 1:
				r += 'a';
				break;
			case 2:
				r += 'b';
				break;
			case 3:
				r += 'c';
				break;
			case 4:
				r += 'd';
				break;
			case 5:
				r += 'e';
				break;
			case 6:
				r += 'f';
				break;
			case 7:
				r += 'g';
				break;
			case 8:
				r += 'h';
				break;
			case 9:
				r += 'i';
				break;
			default:
				return 0;
			}
		case 1:
			switch (t.cypher[i + 1])
			{
			case 0:
				r += 'j';
				break;
			case 1:
				r += 'k';
				break;
			case 2:
				r += 'l';
				break;
			case 3:
				r += 'm';
				break;
			case 4:
				r += 'n';
				break;
			case 5:
				r += 'o';
				break;
			case 6:
				r += 'p';
				break;
			case 7:
				r += 'q';
				break;
			case 8:
				r += 'r';
				break;
			case 9:
				r += 's';
				break;
			default:
				return 0;
			}

		case 2:
			switch (t.cypher[i + 1])
			{
			case 0:
				r += 't';
				break;
			case 1:
				r += 'u';
				break;
			case 2:
				r += 'v';
				break;
			case 3:
				r += 'w';
				break;
			case 4:
				r += 'x';
				break;
			case 5:
				r += 'y';
				break;
			case 6:
				r += 'z';
				break;
			default:
				return 0;
			}
		default:
			break;
		}
	}
	return 1;
}

queue<Task> bag; // портфель
// int size = 0;
int idle = 0; // число простаивающий процессов
string result;

int const arrSize = 100;
pthread_barrier_t barr; //барьер
pthread_t mythread;  //идентификатор для дочернего потока
pthread_mutex_t mutex; // мьютекс для синхронизации вывода;

				
void* worker(void* param) { 
	int p = *((int*)param);
	char Array[arrSize]; //собственный массив потока
	RndString(Array, arrSize);   // инициализация строки шифра
	Task t(Array, arrSize);
	bag.push(t);

	while (true) {
		idle++;
		if (bag.empty() && idle == p) {
			break;
		}
		pthread_mutex_lock(&mutex);
		Task current = bag.front();
		
		// await size > 0
		bag.pop();// удалить задачу из портфеля
		pthread_mutex_unlock(&mutex);// удалить задачу из портфеля
		idle--;

		string line;

		if (!convert(current, &line)) {
			bag.push(current);
		}
		else {
			result += line;
		}
	}

	return nullptr;
}

int main() {
	int zero = 0;
	int max = 4;
	auto seed = clock();
	srand(seed);

	pthread_mutex_init(&mutex, nullptr);

	pthread_barrier_init(&barr, nullptr, 2); //инициализация барьера со значением 2
	pthread_create(&mythread, nullptr, worker, (void*)&zero); //создание дочернего потока
	worker((void*)&max);

	pthread_join(mythread, nullptr);
	cout << result;
	return 0;
}