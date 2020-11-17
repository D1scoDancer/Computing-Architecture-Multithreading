#define HAVE_STRUCT_TIMESPEC

#include <iostream>
#include <deque>
#include <string>
#include <pthread.h>
#include <stdio.h>

using namespace std;

struct Task {
	string cypher;
	int size;
	Task(string a, int b) {
		cypher = a;
		size = b;
	}
};


string generateCypher(int length) {
	string result;

	srand(time(NULL));

	for (int i = 0; i < length; i += 2) {
		int j = char(rand() % 3 + 0x30);
		result += char(j);
		if (j == 50) {
			result += char(rand() % 7 + 0x30);
		}
		else {
			result += char(rand() % 10 + 0x30);
		}
	}
	return result;
}

bool decypher(Task task, string& result) {
	if (task.size % 2 == 1) {
		return false;
	}

	for (int i = 0; i < task.size; i += 2) {
		switch (task.cypher[i])
		{
		case 48:
			switch (task.cypher[i + 1]) {
			case 49:
				result += "a";
				break;
			case 50:
				result += "b";
				break;
			case 51:
				result += "c";
				break;
			case 52:
				result += "d";
				break;
			case 53:
				result += "e";
				break;
			case 54:
				result += "f";
				break;
			case 55:
				result += "g";
				break;
			case 56:
				result += "h";
				break;
			case 57:
				result += "i";
				break;
			}
		case 49:
			switch (task.cypher[i + 1]) {
			case 48:
				result += "j";
				break;
			case 49:
				result += "k";
				break;
			case 50:
				result += "l";
				break;
			case 51:
				result += "m";
				break;
			case 52:
				result += "n";
				break;
			case 53:
				result += "o";
				break;
			case 54:
				result += "p";
				break;
			case 55:
				result += "q";
				break;
			case 56:
				result += "r";
				break;
			case 57:
				result += "s";
				break;
			}
		case 50:
			switch (task.cypher[i + 1]) {
			case 48:
				result += "t";
				break;
			case 49:
				result += "u";
				break;
			case 50:
				result += "v";
				break;
			case 51:
				result += "w";
				break;
			case 52:
				result += "x";
				break;
			case 53:
				result += "y";
				break;
			case 54:
				result += "z";
				break;
			}
		default:
			break;
		}
	}
	return true;
}


int cypher_size;

deque<Task> bag; // портфель
int threadNumber;
pthread_mutex_t mutex;
string result;


Task getTask() {   // допуск к портфелю
	pthread_mutex_lock(&mutex);
	if (!bag.empty()) {
		Task t = bag.front();
		bag.pop_front();
		pthread_mutex_unlock(&mutex);
		return t;
	}

	pthread_mutex_unlock(&mutex);
	return Task("", 0);
}


void* worker(void* param) {  // работа с портфелем
	while (true) {
		Task current = getTask();
		if (current.size == 0) {
			break;
		}

		string line;
		if (!decypher(current, line)) {
			current.cypher += "1";
			current.size += 1;
			bag.push_front(current);
		}
		else {
			result += line;
		}
	}
	return nullptr;
}

int main(int argc, char* argv[]) { //длина зашифрованного сообщения > 0, количество потоков > 0
	cypher_size = stoi(argv[1]);
	threadNumber = stoi(argv[2]);
	string cypher = generateCypher(cypher_size);
	string answer;
	decypher(Task(cypher, cypher_size), answer);

	for (int i = 0; i < cypher_size; i += 2) { // создание портфеля задач
		string s = cypher.substr(i, 2);
		Task t(s, 2);
		bag.push_back(t);
	}

	pthread_mutex_init(&mutex, NULL);
	pthread_t* threads = new pthread_t[threadNumber - 1];
	for (int i = 0; i < threadNumber - 1; i++) {
		pthread_create(&threads[i], NULL, worker, (void*)(i + 1));
	}
	worker((void*)0);
	for (int i = 0; i < threadNumber - 1; i++) {
		pthread_join(threads[i], NULL);
	}

	cout << result << endl;
	return 0;
}