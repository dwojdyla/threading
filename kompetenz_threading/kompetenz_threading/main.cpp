#include <thread>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <mutex>
#include <string>
//#include <ctime>

using namespace std;
mutex m;						//Mutex mit dem Namen m 
condition_variable condivar;	//condivar ist meine Bedingungsvariable
vector <int>  myvector;			//vector wird myvector genannt-

/*
*@author Dominik Wojdyla
*@11.06.2016
*
*/


/*
erzeuger1:
es werden 50 verschiedene Zufallszahlen erstellt, anschlieﬂend 
ausgegeben und zuletzt in einem Vector gespeichert.
*/
void erzeuger1() {
	srand(time(NULL));
	for (int i = 0; i < 50; i++) {
	
		unique_lock<mutex> sperre1{ m };	//Die Mutex wird hier gesperrt 
		condivar.wait(sperre1);			//nach dem "wait", also dem Warten wird sie erneut gesperrt 

	int randomZahl = (rand() % 200) + 1;	//Randomzahlen werden erstellt
		cout << randomZahl << endl;				//Randomzahlen werden ausgegeben
		myvector.push_back(randomZahl);			//mit push_back() wird eine Element hinten an den Vector dazugef¸gt 
		condivar.notify_one();					//"deblockiert" den erzeuger1 Thread
	}

}
/*
Bei den beiden Erzeugern wurde bei der Random Erstellung 
einmal ein +1 und einmal ein -1 dazugerechnet. 
Deswegen kommen 2 unterschiedliche random Werte.
Wenn beiden +1 w‰re, dann w¸rden 2 gleiche Zahlen ausgegeben werden. 

*/

/*
erzeuger2:
es werden 50 verschiedene Zufallszahlen erstellt, anschlieﬂend
ausgegeben und zuletzt in einem Vector gespeichert.
*/
void erzeuger2() {
	srand(time(NULL));
	for (int i = 0; i < 50; i++) {
		unique_lock<mutex> sperre2{ m };	//Die Mutex wird hier gesperrt
		condivar.wait(sperre2);				//nach dem "wait", also dem Warten wird sie erneut gesperrt
		int randomZahl = (rand() % 300) - 1;	//Randomzahlen werden erstellt
		cout << randomZahl << endl;				//Randomzahlen werden ausgegeben
		myvector.push_back(randomZahl);			//mit push_back() wird eine Element hinten an den Vector dazugef¸gt 
		condivar.notify_one();					//"deblockiert" den erzeuger2 Thread

	}
}
/*
verbraucher:
der Verbraucher lˆscht ein Element, falls eines im Vector vorhanden ist, 
wartet anschlieﬂend kurz und wiederholt den Vorgang.
*/
void verbraucher() { 
	m.lock();				//mztex locken
	if (!myvector.empty()) {				//wenn myVector nicht leer ist dann:
		myvector.pop_back();				//Hinterstes Element wird gelˆscht
		std::this_thread::sleep_for(10ms);	//geht pennen f¸r 10ms
	}
	m.unlock();
	
}

/*
watcher:
der Watcher schaut einfach nur zu. 
Bei eingabe von "exit", schlieﬂt sich das Programm.
*/
void watcher() {
	m.lock();
	
	string eingabe;							//string eingabe wird erstellt.
	while (eingabe != "exit"){
	cout << "Bitte exit eingeben, wenn Sie das Programm beenden moechten";	//anschlieﬂend kommt diese Meldung
	
	getline(std::cin, eingabe);
	
	if (eingabe == "exit") {			//wenn die eingabe "exit" ist, dann wird das Programm geschlossen. 
		exit(0);
		m.unlock();
	}
	}
}

int main() {
	
		thread t1(erzeuger1);
		thread t2(erzeuger2);
		thread t3(verbraucher);
		thread t4(watcher);
	
	unique_lock<mutex> sperre2(m);
		cout << "\n";
		condivar.notify_one();
		condivar.wait(sperre2);

		

		//t1.detach();		mit detach wird der thread asugef¸hrt ohen ber¸cksichtigung der anderer Threads


		t1.join();
		t2.join();		//mit join() wird gewartet
		t3.join();
		t4.join();
		
		system("PAUSE");
	
	
}