#define HAVE_STRUCT_TIMESPEC

#include <semaphore.h>
#include <ctime>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <thread>
#include <chrono>

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
vector<Stuff> table;
sem_t sem_smoke;
sem_t sem_make;


void* Barman(void* param) {
	int num = *((int*)param);
	while (num > 0) {
		table.clear();

		int n1, n2;
		do {
			n1 = rand() % 3 + 1;
			n2 = rand() % 3 + 1;
		} while (n1 == n2);

		Stuff s1 = static_cast<Stuff>(n1);
		Stuff s2 = static_cast<Stuff>(n2);

		table.push_back(s1);
		table.push_back(s2);

		cout << "Barman puts " << stuffToStr(s1) << " and " << stuffToStr(s2) << " on the table" << endl;

		for (int i = 0; i < smokers_num; i++) {
			sem_post(&sem_smoke);
		}
		sem_wait(&sem_make);

		num--;
	}

	return nullptr;
}

void* Smoker(void* param) {
	int num = *((int*)param);

	while (1) {
		sem_wait(&sem_smoke);


	}
	return nullptr;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		cout << "No arg for number of cigarettes" << endl;
		return 0;
	}
	int cig_num = atoi(argv[1]);

	cout << "Welcome to my smoking bar. Today we will smoke " << cig_num << " cigarettes" << endl;
	sem_init(&sem_smoke, 0, 0);
	sem_init(&sem_make, 0, 0);

	pthread_t barman;
	pthread_create(&barman, NULL, Barman, (void*)&cig_num);

	pthread_t smokers[smokers_num];
	int thread_nums[smokers_num];

	for (int i = 1; i < smokers_num; i++) {
		thread_nums[i] = i + 1;
		pthread_create(&smokers[i], NULL, Smoker, (void*)&thread_nums[i]);
	}
	thread_nums[0] = 1;
	Smoker((void*)&thread_nums[0]);
	this_thread::sleep_for(chrono::milliseconds(2000));
	printf("[%d] The bar is closed. All %d cigarettes were smoked", (int)clock(), cig_num);
	return 0;
}
