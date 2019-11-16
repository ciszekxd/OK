
#include <iostream>
#include <fstream>
#include <set>

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
    set<int*> validProcesses;
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
    int** setProcessor(int** procT, int procAmount) {

        int* arm;
        for (int i = 0; i < procAmount; ++i) {
            set<int*>::iterator iterator = validProcesses.begin();
            while (iterator != validProcesses.end()) {
                if (procT[i][0]==-1){
                    arm = *iterator;
                    if(i == machinesT[arm[0]][arm[1]]){
                        procT[i][0] = timesT[arm[0]][arm[1]];
                        procT[i][1] = arm[0];
                        procT[i][2] = arm[1];
                        validProcesses.erase(iterator);
                    }
                }
            iterator++;
            }
        }
        return procT;
    }

    void upadteTimesT(int** newTT){
        timesT = newTT;
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
    int hardcheck(int* exe){
        set<int*>::iterator iterator = validProcesses.begin();
        int* x = *iterator;
        while(iterator != validProcesses.end()){

            if((x[0]==exe[0])&&(x[1]==exe[1])) return 0;
            iterator++;
        }
        return 1;

    }
    void prepareValidProcesses(){

        int j=0;

        for (int i = 0; i < time_dim; ++i) {
            if (timesT[i][j] == -1) {
                j++;
                i--;
                continue;
            }
            int* x = new int[2];;
            x[0] = i;
            x[1] = j;
            if (!hardcheck(x)) continue;
            validProcesses.insert(x);
        }
    }

};
class Processor{
public:
    int** procek;
    int coreAmount;
    int T;
    int** finishLineup;


    Processor(int core,int execs){
        procek = maketab(3, core);
        for (int i = 0; i < core; ++i) {
            procek[i][0] = -1;
        }
        coreAmount = core;
        T = 0;
        finishLineup = maketab(execs,execs);
    }
    void procekUpdate(int** uproc){
        procek = uproc;
    }
    void makeProcessorStep(){
        int min = 2147483647;
        for (int i = 0; i < coreAmount; ++i) {
            if ((procek[i][0] < min) && (procek[i][0] != -1)){
                min = procek[i][0];
            }
        }
        for (int j = 0; j < coreAmount; ++j) {
            if (procek[j][0]==-1) continue;
            procek[j][0] -= min;
        }
        T += min;
    }
    int** freeProcessor(int** timesT){
        for (int i = 0; i < coreAmount; ++i) {
            if (procek[i][0]==0) {
                procek[i][0] = -1;
                timesT[procek[i][1]][procek[i][2]] = -1;
                finishLineup[procek[i][1]][procek[i][2]] = T;
            }
        }
        return timesT;
    }
    int** maketab(int a, int b) {
        int **x = new int *[b];
        for (int i = 0; i < b; ++i) {
            x[i] = new int[a];
        }
        return x;
    }
};
void poka(int**T,int dim) {
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            cout << T[i][j];
            cout << "  ";
        }
        cout << endl;
    }
}
int checkend(int** matrix,int exe){
    for (int i = 0; i < exe; ++i) {
        if(matrix[i][exe-1] != -1) return 1;
    }
    return 0;
}
int main(){
    string path = "/home/ciszek/OK/testcase.txt";
    Otwieracz bulgaria(path, "b");

    Solver case1(bulgaria.zadania, bulgaria.machines, bulgaria.timesTable, bulgaria.machinesTable );

    Processor intel(bulgaria.machines, bulgaria.zadania);

    
    //#######pętla###############
    int obs=1;
    while(obs) {
        case1.prepareValidProcesses();
        //problem
        intel.procekUpdate(case1.setProcessor(intel.procek, intel.coreAmount));
        // idea części dalszej
        intel.makeProcessorStep();
        case1.upadteTimesT(intel.freeProcessor(case1.timesT));
        obs = checkend(case1.timesT,bulgaria.zadania);
        //###########################
    }
    //rezultatem ma być intel.finishlineup
    poka(intel.finishLineup,intel.coreAmount);
    return 0;
}
