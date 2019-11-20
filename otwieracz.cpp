
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
        timesTable = maketab(machines,zadania);
        machinesTable = maketab(machines,zadania);
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
        string trush;
        plik.seekg(0, ios_base::beg);
        getline(plik,trush);
        getline(plik,trush);
        for (int i = 0; i < zadania; ++i) {
            for (int j = 0; j < machines; ++j) {
                plik >> timesTable[i][j];
            }
        }
        getline(plik,trush);
        getline(plik,trush);
        for (int i = 0; i < zadania; ++i) {
            for (int j = 0; j < machines; ++j) {
                plik >> machinesTable[i][j];
            }
        }
        //zmniejszenie maszyn o 1
        for (int k = 0; k < zadania; ++k) {
            for (int i = 0; i < machines; ++i) {
                machinesTable[k][i] -= 1;
            }
        }
        plik.close();
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
            for (int j = 0; j < machines; ++j) {
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
    vector<int*> validProcesses;
    int dimX;
    int dimY;
    int** machinesT;
    int** timesT;

    Solver (int dim_X, int dim_Y, int** times, int** machines){
        dimX = dim_X;
        dimY = dim_Y;
        timesT = times;
        machinesT = machines;
    }
    void suportRmove(std::vector<int*> vec,int ele){
        int ite=0;
        while(ite<vec.size()){
            if(vec[ele]==validProcesses[ite]){
                validProcesses.erase(validProcesses.begin()+ite);
                break;
            }
            ite++;
        }
    }
    int** setProcessor(int** procT, int procAmount) {
        int* arm;
        int x;
        vector<int*> imageV = validProcesses;
        for (int i = 0; i < procAmount; ++i) {
            x=0;
            while (x<imageV.size()) {
                if (procT[i][0]==-1){
                    arm = imageV[x];
                    if(i == machinesT[arm[0]][arm[1]]){
                        procT[i][0] = timesT[arm[0]][arm[1]];
                        procT[i][1] = arm[0];
                        procT[i][2] = arm[1];
                        suportRmove(imageV,x);
                    }
                }
                x++;
            }
        }
        return procT;
    }

    void upadteTimesT(int** newTT){
        timesT = newTT;
    }
//    int* find_min_time(int col){
//        int ans[3];
//        int* answ = ans;
//        int min = 2147483647,x,y;
//        for (int i = 0; i < time_dim ; ++i) {
//            if (timesT[i][col] < min){
//                    min = timesT[i][col];
//                    x = col;
//                    y = i;
//            }
//        }
//        ans[0] = min;
//        ans[1] = y;
//        ans[2] = x;
//        return answ;
//    }
    int setCheck(int* exe) {
        int ite=0;
        while (ite < validProcesses.size()) {
            int* x = validProcesses[ite];
            if ((x[0] == exe[0]) && (x[1] == exe[1])) return 1;
            ite++;
        }
        return 0;
    }
    int processorCheck(int**procT,int procAmount,int* x){
        for (int k = 0; k < procAmount; ++k) {
            if( (procT[k][1]==x[0]) && (procT[k][2]==x[1]) ) return 1;
        }
        return 0;
    }

    void prepareValidProcesses(int** procT, int procAmount){
        for (int i = 0; i < dimY; ++i) {
            for (int j = 0; j < dimX; ++j) {
                if (timesT[i][j] == -1) continue;
                int* x = new int[2];
                x[0] = i;
                x[1] = j;
                if(processorCheck(procT,procAmount,x) || setCheck(x)) break;
                validProcesses.push_back(x);
                break;
            }
        }
//        int j=0;
//        for (int i = 0; i < dimY; ++i) {
//            if (timesT[i][j] == -1) {
//                j++;
//                if(j == dimX-1) continue;
//                i--;
//                continue;
//            }
//            int* x = new int[2];
//            x[0] = i;
//            x[1] = j;
//            if(processorCheck(procT,procAmount,x)){
//                j = 0;
//                continue;
//            }
//            if(setCheck(x)) {
//                j = 0;
//                continue;
//            }
//            //if(timesT[i][j]==-2) continue;
//            validProcesses.push_back(x);
//            j=0;
//        }
    }

};
class Processor{
public:
    int** procek;
    int coreAmount;
    int T;
    int** finishLineup;


    Processor(int core,int dimX,int dimY){
        procek = maketab(3, core);
        for (int i = 0; i < core; ++i) {
            for (int j = 0; j < 3; ++j) {
                procek[i][j] = -1;

            }
        }
        coreAmount = core;
        T = 0;
        finishLineup = maketab(dimX,dimY);
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
void poka(int**T,int dimY,int dimX) {
    for (int i = 0; i < dimY; ++i) {
        for (int j = 0; j < dimX; ++j) {
            cout << T[i][j];
            cout << "  ";
        }
        cout << endl;
    }
}
void showProc(int**proc,int core){
    for (int i = 0; i < core; ++i) {
        cout << proc[i][0];
        cout << "  ";
        cout << proc[i][1];
        cout << "  ";
        cout << proc[i][2];
        cout << endl;
    }
}
int checkend(int** matrix,int dimX, int dimY){
    for (int i = 0; i < dimY; ++i) {
        if(matrix[i][dimX-1] != -1) return 1;
    }
    return 0;
}
int main(int argc, char* argv[]){
//    if (argc > 3){
//        cout << "too much arguments was given" << endl;
//        exit(1);
//    } else if (argc < 3){
//        cout << "too few arguments was given" << endl;
//        exit(1);
//    }
    string path = "C:\\Users\\1\\Desktop\\tai01.txt";
    Otwieracz bulgaria(path, "t");
    cout << bulgaria.zadania << endl;
    cout << bulgaria.machines << endl;
    poka(bulgaria.timesTable,bulgaria.zadania,bulgaria.machines);
    cout << "czasyXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"<< endl;
    poka(bulgaria.machinesTable,bulgaria.zadania,bulgaria.machines);
    cout << "maszynyXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"<< endl;


    Solver case1(bulgaria.machines, bulgaria.zadania, bulgaria.timesTable, bulgaria.machinesTable );

    Processor intel(bulgaria.machines, case1.dimX, case1.dimY);


    //#######pętla###############
    int obs=1;
    while(obs) {
        case1.prepareValidProcesses(intel.procek, intel.coreAmount);

        intel.procekUpdate(case1.setProcessor(intel.procek, intel.coreAmount));

        intel.makeProcessorStep();

        case1.upadteTimesT(intel.freeProcessor(case1.timesT));

        obs = checkend(case1.timesT, case1.dimX, case1.dimY);
        cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
        poka(intel.finishLineup, case1.dimY, case1.dimX);
        cout << "koncowe ustawienie###################################" << endl;
        poka(case1.timesT, case1.dimY, case1.dimX);
        cout << "czasy wykonywania zadan###################################" << endl;
        cout << intel.T << endl;
        cout << "aktualny czas###################################" << endl;
        showProc(intel.procek, intel.coreAmount);
        cout << "stan procesora###################################" << endl;
    }
    //rezultatem ma być intel.finishlineup
    //poka(intel.finishLineup,intel.coreAmount);
    return 0;
}



