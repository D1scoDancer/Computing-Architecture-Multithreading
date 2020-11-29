#include <iostream>
#include <omp.h>
#include <deque>
#include <iomanip>
#include <string>
#include <time.h>

using namespace std;

#pragma region HelpMethods


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

void decypher(Task task, string& result) {
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
			break;
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
			break;
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
			break;
		default:
			break;
		}
	}
}


#pragma endregion

int cypher_size;
deque<Task> bag; // портфель
string res;

Task getTask() {   // допуск к портфелю
	if (!bag.empty()) {
		Task t = bag.front();
		bag.pop_front();
		return t;
	}
	return Task("", 0);
}


int worker(int numOfThreads) {
#pragma omp parallel num_threads(numOfThreads)
	{
		int i = 0;
		while (1) {
			if (bag.empty()) {
				break;
			}
#pragma omp critical
			{
				Task current = getTask();
				if (current.size != 0) {
					if (current.cypher == "00") {  // в случае ошибки генерации (00) происходит замена символа и создание новой задачи, которая отправляется в начало портфеля
						Task newT("01", 2);
						bag.push_front(newT);
					}
					else {
						string line;
						decypher(current, line);
						res += line;
						cout << "I am Thread(" << omp_get_thread_num() + 1 << " of " << omp_get_num_threads() << ") and I decypher " << current.cypher << " to " << line << endl;
					}
				}

			}
		}
	}
	return 0;
}

int main() {
	cypher_size = 60;
	string cypher = generateCypher(cypher_size);
	cout << "Generated cypher: " << cypher << endl;

	for (int i = 0; i < cypher_size; i += 2) { // создание портфеля задач
		string s = cypher.substr(i, 2);
		Task t(s, 2);
		bag.push_back(t);
	}

	cout << setprecision(10);
	double t = omp_get_wtime();
	worker(3);
	t = omp_get_wtime() - t;
	cout << "Time: " << t * 1000 << "ms" << endl;
	cout << "Result: " << res << endl;
}