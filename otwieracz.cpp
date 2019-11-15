
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
class Otwieracz {
public:
    ifstream plik;
    int machines;
    int zadania;
    int** timesTable;
    int** machinesTable;
    int*** mergedMT;

    Otwieracz(string path, string t){
        plik.open(path, ios::in);
        getZadania();
        getMachines();
        timesTable = maketab(zadania,zadania);
        machinesTable = maketab(zadania,zadania);
        if(t == "b"){
            beasley();
        }
        else if (t == "t"){
            tailard();
        }
        else {
            cout << " mode not established ";
            exit(1);
        }
        merge();
    }
protected:
    void getZadania() {
        plik.seekg(0, ios_base::beg);
        plik >> zadania;
    }
    void getMachines(){
        plik.seekg(0, ios_base::beg);
        int x;
        plik >> x;
        plik >> machines;

    }

    int** maketab(int a, int b) {
        int **x = new int *[b];
        for (int i = 0; i < b; ++i) {
            x[i] = new int[a];
        }
        return x;
    }
    void tailard() {
        if (!plik.is_open()) {
            cout << "Unable to open file";
            exit(1);
        }
        int x;
        plik.seekg(0, ios_base::beg);
        for(int i=0; i<6;i++){
            plik >> x;
            }
        plik.seekg(7 ,ios_base::cur);
        for (int i = 0; i < zadania; ++i) {
            for (int j = 0; j < zadania; ++j) {
                plik >> timesTable[i][j];
            }
        }  
        plik.seekg(10 ,ios_base::cur);
        for (int i = 0; i < zadania; ++i) {
            for (int j = 0; j < zadania; ++j) {
                plik >> machinesTable[i][j];
            }
        }
    }
    void beasley() {
        int x, y;
        //plik.open("C:\\Users\\1\\Desktop\\struk.txt");
        //plik.open("source.txt");
        if (!plik.is_open()) {
            cout << "Unable to open file";
            exit(1); // terminate with error
        }
        plik.seekg(0, ios_base::beg);
        plik >> y;
        plik >> x;
        //time = maketab(y,y);
        //machine = maketab(y,y);
        for (int i = 0; i < zadania; ++i) {
            for (int j = 0; j < zadania; ++j) {
                plik >> machinesTable[i][j];
                plik >> timesTable[i][j];
            }
        }
        plik.close();
    }

    void merge(){
        mergedMT = new int**[zadania];
        for (int i = 0; i < zadania; ++i) {
            mergedMT[i] = maketab(zadania,2);
        }
    }
};
class Solver{
public:
    vector<int > done_exercises;
    vector<int*> validProcesses;
    int time_dim;
    int machine_dim;
    int** machinesT;
    int** timesT;

    Solver (int t_dim, int m_dim, int** times, int** machines){
        time_dim = t_dim;
        machine_dim = m_dim;
        timesT = times;
        machinesT = machines;
    }


    int* find_min_time(int col){
        int ans[3];
        int* answ = ans;
        int min = 2147483647,x,y;
        for (int i = 0; i < time_dim ; ++i) {
            if (timesT[i][col] < min){
                    min = timesT[i][col];
                    x = col;
                    y = i;
            }
        }
        ans[0] = min;
        ans[1] = y;
        ans[2] = x;
        return answ;
    }
    void prepareValidProcesses(){
        int obs1=0,obs2=0;
        int* x;
        for (int i = 0; i < time_dim; ++i) {
            for (int j = 0; j < time_dim; ++j) {
                if (timesT[i][j] == -1){
                    obs1 = j;
                    obs2 = i;
                    break;
                }
            }
            x = new int[2];
            x[0] = timesT[obs2][obs1];
            x[1] = machinesT[obs2][obs1];

            validProcesses.push_back(x);
        }

    }

};
class Processor{
public:
    int** procek;
    int coreAmount;
    Processor(int core){
        procek = new int*[core];
        for (int i = 0; i < coreAmount; ++i) {
            procek[i] = nullptr;
        }
        coreAmount = core;
    }
    void freeProcessor(){
        for (int i = 0; i < coreAmount; ++i) {
            if(procek[i][1] == 0) procek[i] = nullptr;
        }
    }
};
int main(){
    string path = "/home/ciszek/OK/tai01.txt";
    Otwieracz bulgaria(path, "t");
    cout << bulgaria.zadania << endl;
    cout << bulgaria.machines << endl;
    cout << bulgaria.timesTable[3][6] << endl;
    cout << bulgaria.machinesTable[3][6] << endl;
    Solver case1(bulgaria.zadania, bulgaria.machines, bulgaria.timesTable, bulgaria.machinesTable );

    return 0;
}
