using namespace std;
#include <iostream>
#include <cmath>
#include <fstream>


const int LEN = 25; //wymiar
const float STRATA=0.93; //strata w przejściu
const float S_INTER=0.78; // strata w interferencji
const float S_INTER2=0.83; // strata w interferencji
const float S_SCIANA=0; // strata w ścianę
const float AMP=1000; // amplituda początkowa
int room[LEN][LEN]; // pokój
struct fala{
	int id[4];
	float amp[4][2];
	int state[4][2]; //0-brak 1-pasywne 2-aktywne
	};
fala danefali[LEN][LEN];

void printToFile(std::ofstream& outFile) {
    //outFile << "np.array([" << std::endl;
    for (int i = 1; i <= LEN - 2; i++) {
        for (int j = 1; j <= LEN - 2; j++) {
            float b = 0;
            for (int k = 0; k < 4; k++) {
                if (b < danefali[i][j].amp[k][1]) {
                    b = danefali[i][j].amp[k][1];
                }
            }
            outFile  << b << " ";
        }
        outFile << std::endl;
    }
}

void rpodglad()
{
    for (int i = 0; i <= LEN - 1; i++) {
        for (int j = 0; j <= LEN - 1; j++) {
            room[i][j] = 0;
        }
    }
    // wypełnianie room

    for (int i = 0; i <= LEN - 1; i++) {
        room[i][0] = 1;
        room[i][LEN - 1] = 1;
        room[0][i] = 1;
        room[LEN - 1][i] = 1;
    } // tworzenie ścian
}

auto kierunki(int x,int y){ //1,a-lewo 2,b-góra 3,c-prawo 4,d-dół
	struct kier {bool a;bool b;bool c;bool d;};
	bool f=false,g=false,h=false,j=false;
	if(danefali[x][y].state[0][1]!=0){ //left
			f=true;
            g=true;
            j=true;
	}
	if(danefali[x][y].state[1][1]!=0){ //up
            f=true;
            g=true;
            h=true;
	}
	if(danefali[x][y].state[2][1]!=0){//right
            g=true;
            h=true;
            j=true;
	}
	if(danefali[x][y].state[3][1]!=0){//down
            h=true;
            j=true;
            f=true;
	}
	return kier {f,g,h,j};
}

void obliczanieAmp(int x, int y){
        bool a=false,b=false,c=false,d=false;
        float f =danefali[x][y].amp[0][1];
        float g =danefali[x][y].amp[1][1];
        float h =danefali[x][y].amp[2][1];
        float j =danefali[x][y].amp[3][1];
        if (f!=0){a=true;} // sprawdzanie aktywnych
        if (g!=0){b=true;}
        if (h!=0){c=true;}
        if (j!=0){d=true;}
        float TMP1, TMP2,TMP3;
        TMP1=abs(f-h);
        TMP2=abs(g-j);
        if(TMP1==0){
            danefali[x][y].state[0][1]=0;
            danefali[x][y].state[2][1]=0;
            danefali[x][y].amp[0][1]=0;
            danefali[x][y].amp[2][1]=0;
            a=false; c=false;
        }

        if(TMP2==0){
            danefali[x][y].state[1][1]=0;
            danefali[x][y].state[3][1]=0;
            danefali[x][y].amp[1][1]=0;
            danefali[x][y].amp[3][1]=0;
            b=false; d=false;
        }
        if(TMP1==0||TMP2==0){TMP3=sqrt((TMP1*TMP1)+(TMP2*TMP2))*S_INTER;}
        else{TMP3=sqrt((TMP1*TMP1)+(TMP2*TMP2))*S_INTER2;}
        if(TMP3<1){TMP3=0;}

        if(a){danefali[x][y].amp[0][1]=round(TMP3);}
        if(b){danefali[x][y].amp[1][1]=round(TMP3);}
        if(c){danefali[x][y].amp[2][1]=round(TMP3);}
        if(d){danefali[x][y].amp[3][1]=round(TMP3);}
}

void zerowanie(int x, int y){
    for(int i=0;i<4;i++){
        if(danefali[x][y].state[i][1]!=0){
            danefali[x][y].amp[i][1]=0;
            danefali[x][y].state[i][1]=0;
            if(danefali[x][y].state[i][0]==0){
                danefali[x][y].state[i][0]=0;
            }
        }
    }
}

void move(int x,int y){
    int direction=5; //where to take data from
    for (int i=0;i<4;i++){
        if(danefali[x][y].amp[i][1]!=0){
            direction=i;
            break;
        }
    }
    if (direction==5){return;}
    auto [a,b,c,d] =kierunki(x,y);
    // Function to check if a cell is a wall
    auto isWall = [&](int i, int j) {
        return room[i][j] == 1;
    };
    if(a){ //left
        if (!isWall(x, y - 1)) {
            danefali[x][y - 1].id[0] = danefali[x][y].id[direction];
            danefali[x][y - 1].amp[0][0] = danefali[x][y].amp[direction][1]*STRATA;
            danefali[x][y].state[0][0]=1;
        } else {
            // Bounce off the wall
            danefali[x][y].amp[2][0] =danefali[x][y].amp[direction][1]*S_SCIANA;  // Reduce amplitude
            danefali[x][y].state[2][0] = 1;  // Set state to passive to avoid further propagation
        }
    }
    if(b){ //up
        if (!isWall(x-1, y)) {
            danefali[x-1][y].id[1] = danefali[x][y].id[direction];
            danefali[x-1][y].amp[1][0] = danefali[x][y].amp[direction][1]*STRATA;
            danefali[x][y].state[1][0]=1;
        } else {
            // Bounce off the wall
            danefali[x][y].amp[3][0] =danefali[x][y].amp[direction][1]*S_SCIANA;  // Reduce amplitude (you can adjust this as needed)
            danefali[x][y].state[3][0] = 1;  // Set state to passive to avoid further propagation
        }
    }
    if(c){ //right
        if (!isWall(x, y + 1)) {
            danefali[x][y + 1].id[2] = danefali[x][y].id[direction];
            danefali[x][y + 1].amp[2][0] = danefali[x][y].amp[direction][1]*STRATA;
            danefali[x][y].state[2][0]=1;
        } else {
            // Bounce off the wall
            danefali[x][y].amp[0][0] =danefali[x][y].amp[direction][1]*S_SCIANA;  // Reduce amplitude (you can adjust this as needed)
            danefali[x][y].state[0][0] = 1;  // Set state to passive to avoid further propagation
        }
    }
    if(d){ //down
        if (!isWall(x+1, y)) {
            danefali[x+1][y].id[3] = danefali[x][y].id[direction];
            danefali[x+1][y].amp[3][0] = danefali[x][y].amp[direction][1]*STRATA;
            danefali[x][y].state[3][0]=1;
        } else {
            // Bounce off the wall
            danefali[x][y].amp[1][0] =danefali[x][y].amp[direction][1]*S_SCIANA;  // Reduce amplitude (you can adjust this as needed)
            danefali[x][y].state[1][0] = 1;  // Set state to passive to avoid further propagation
        }
    }
}
void rozchodzenie(int x, int y){
    int direction=5; //where to take data from
    for (int i=0;i<4;i++){
        if(danefali[x][y].amp[i][1]!=0){
            direction=i;
            break;
        }
    }
    if (direction==5){return;}
    obliczanieAmp(x,y);
    move(x,y);
    zerowanie(x,y);
}

void aktywacja(){

for(int i=0;i<LEN;i++) {
    for (int j = 1; j < LEN - 1; j++) {
        for (int m=0; m<4 ;m++) {
            if (danefali[i][j].amp[m][0]!=0) {
                danefali[i][j].amp[m][1]=danefali[i][j].amp[m][0];
                danefali[i][j].amp[m][0]=0;
                danefali[i][j].state[m][1]=1;
                danefali[i][j].state[m][0]=0;
                }
            }
        }
    }
}

void pulse(int x, int y,int licznik){
    danefali[x][y]={{licznik+1,licznik+1,licznik+1,licznik+1},{{0,AMP},{0,AMP},{0,AMP},{0,AMP}},{{0,1},{0,1},{0,1},{0,1}}};
    move(x,y);
}

int przejscie(int x, int y,int licznik){ // x,y=współrzędne źródła
    if(licznik%50==0){
        //danefali[x][y]={{licznik+1,licznik+1,licznik+1,licznik+1},{{0,AMPLITUDE},{0,AMPLITUDE},{0,AMPLITUDE},{0,AMPLITUDE}},{{0,1},{0,1},{0,1},{0,1}}}
        //;
        /*danefali[x][y-1].id[0]=licznik+1;
        danefali[x][y-1].amp[0][1]=AMP*STRATA;
        danefali[x][y-1].state[0][1]=1;
        danefali[x-1][y].id[1]=licznik+1;
        danefali[x-1][y].amp[1][1]=AMP*STRATA;
        danefali[x-1][y].state[1][1]=1;
        danefali[x][y+1].id[2]=licznik+1;
        danefali[x][y+1].amp[2][1]=AMP*STRATA;
        danefali[x][y+1].state[2][1]=1;
        danefali[x+1][y].id[3]=licznik+1;
        danefali[x+1][y].amp[3][1]=AMP*STRATA;
        danefali[x+1][y].state[3][1]=1;
        */
        pulse(x,y,licznik);
    }
    for (int i=1;i<LEN-1;i++){
        for (int j=1;j<LEN-1;j++){
            //if(licznik==3&&i>4&&i<10&&j>4&&j<10){
            //    print();
            //}
            rozchodzenie(i,j);
        }
    }
aktywacja();
return licznik;
}


int main(){


for (int i=0;i<=LEN-1;i++) {
     for (int j = 0; j <= LEN - 1; j++) {
         danefali[i][j] = {{0,      0,      0,      0},
                           {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
                           {{0,0},      {0,0},      {0,0},      {0,0}}};
     }
}
int licznik=0;
rpodglad();

    std::ofstream outFile("Frames.txt"); // Open file for writing

if (!outFile.is_open()) {
    std::cerr << "Error: Unable to open the file for writing!" << std::endl;
    return 1; // Return an error code // Check if the file is successfully opened
}
    outFile << LEN-2 << endl;

int center =(LEN-1)/2;
room[center][center]=25;//źródło
for (int i=0;i<LEN*2;i++) {
    licznik = przejscie(center, center, licznik);
    licznik++;
    printToFile(outFile); // Write data to the file
    }

outFile.close(); // Close the file

return 0;
}
