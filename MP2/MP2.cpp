#define HAVE_STRUCT_TIMESPEC

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <semaphore.h>
#include <pthread.h>

using namespace std;

enum Stuff {
	Tobacco = 1,
	Paper = 2,
	Matches = 3
};

string stuffToStr(Stuff s) {
	if (s == Tobacco) {
		return "Tobacco";
	}
	else if (s == Paper) {
		return "Paper";
	}
	else if (s == Matches) {
		return "Matches";
	}
	else {
		return "?";
	}
}

const int smokers_num = 3;
bool isOver = false;
vector<Stuff> table;
sem_t sem_smoke;
sem_t sem_make;
clock_t start, stop;

void* Barman(void* param) {
	srand(time(NULL) * _getpid());
	int num = *((int*)param);
	while (num > 0) {
		table.clear();
		

		int n1 = rand() % 3 + 1;
		int n2 = (n1 + rand() % 2) % 3 + 1;

		Stuff s1 = static_cast<Stuff>(n1);
		Stuff s2 = static_cast<Stuff>(n2);

		table.push_back(s1);
		table.push_back(s2);

		cout << "[" << (clock() - start) / 1000.0 << "] Barman puts " << stuffToStr(s1) << " and " << stuffToStr(s2) << " on the table" << endl;

		sem_post_multiple(&sem_smoke, 3);
		sem_wait(&sem_make);
		num--;
	}
	isOver = true;
	table.clear();
	sem_post_multiple(&sem_smoke, 3);
	return nullptr;
}

void* Smoker(void* param) {
	int num = *((int*)param);
	while (!isOver) {
		sem_wait(&sem_smoke);
		if (table.size() > 0) {
			if (find(table.begin(), table.end(), num) == table.end()) {
				srand(time(NULL) * _getpid());
				int sleepTime = rand() % 2000 + 1000;
				cout << "[" << (clock() - start) / 1000.0 << "] Smoker " << num << " will be smoking for " << sleepTime/1000.0 << endl;
				this_thread::sleep_for(chrono::milliseconds(sleepTime));
				sem_post(&sem_make);
			}
		}
	}
	return nullptr;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		cout << "No arg for number of cigarettes" << endl;
		return 0;
	}
	int cig_num = atoi(argv[1]);
	if (cig_num <= 0) {
		cout << "cig_num must be >= 0" << endl;
		return 0;
	}

	cout << "Welcome to my bar where 3 smoker will smoke " << cig_num << " cigarretes" << endl;
	start = clock();

	sem_init(&sem_make, 0, 0);
	sem_init(&sem_smoke, 0, 0);
	pthread_t barman;
	pthread_create(&barman, NULL, Barman, (void*)&cig_num);
	pthread_t smokers[smokers_num - 1];
	int nums[smokers_num-1];
	for (int i = 0; i < smokers_num - 1; i++) {
		nums[i] = i + 1;
		pthread_create(&smokers[i], NULL, Smoker, (void*)&nums[i]);
	}
	int mainNum = 3;
	Smoker((void*)&mainNum);

	stop = clock();
	cout << "[" << (stop - start) / 1000.0 << "] It is over" << endl;
	return 0;
}