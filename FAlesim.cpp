using namespace std;
#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

const int LEN = 25; //wymiar
const int AMPLITUDE = 1000;
int room[LEN][LEN]; // pokój
struct fala{
    int id[4];
    int dir[4];
    float amp[4];
    int hp[4];
    int state[4]; //0-brak 1-pasywne 2-aktywne
};
fala danefali[LEN][LEN];

void print(){
    for (int i=1;i<=LEN-2;i++){
        for(int j=1; j<=LEN-2;j++){
            for(int k=0; k<4;k++){
                cout << danefali[i][j].amp[k] << " ";
            }
            cout << "|";
        }
        cout << endl;
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

    for (int i = 0; i <= LEN / 5 - 1; i++) {
        for (int j = 0; j <= LEN / 5 - 1; j++) {
            room[i][j] = 2;
        }
    } //widownia

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
    if(danefali[x][y].state[0]==2&&danefali[x][y-1].state[0]!=2){ //left
        if (danefali[x][y].dir[0]!=3){
            f=true;
        }
        if (danefali[x][y].dir[1]!=4){
            g=true;
        }
        if (danefali[x][y].dir[2]!=1){
            j=true;
        }
    }
    if(danefali[x][y].state[1]==2&&danefali[x-1][y].state[1]!=2){ //up
        if (danefali[x][y].dir[0]!=3){
            f=true;
        }
        if (danefali[x][y].dir[1]!=4){
            g=true;
        }
        if (danefali[x][y].dir[3]!=2){
            h=true;
        }

    }
    if(danefali[x][y].state[2]==2&&danefali[x][y+1].state[2]!=2){//right
        if (danefali[x][y].dir[1]!=4){
            g=true;
        }
        if (danefali[x][y].dir[2]!=1){
            h=true;
        }
        if (danefali[x][y].dir[3]!=2){
            j=true;
        }
    }
    if(danefali[x][y].state[3]==2&&danefali[x+1][y].state[3]!=2){//down
        if (danefali[x][y].dir[2]!=1){
            h=true;
        }
        if (danefali[x][y].dir[3]!=2){
            j=true;
        }
        if (danefali[x][y].dir[0]!=3){
            f=true;
        }
    }
    return kier {f,g,h,j};
}

void obliczanieAmp(int x, int y){
    bool a=false,b=false,c=false,d=false;
    if (danefali[x][y].state[0]==2){a=true;} // sprawdzanie aktywnych
    if (danefali[x][y].state[1]==2){b=true;}
    if (danefali[x][y].state[2]==2){c=true;}
    if (danefali[x][y].state[3]==2){d=true;}
    float TMP1, TMP2,TMP3;
    if(a&&c){TMP1=abs(danefali[x][y].amp[0]-danefali[x][y].amp[2]);}
    else if(a){TMP1=danefali[x][y].amp[0];}
    else {TMP1=danefali[x][y].amp[2];}
    if(b&&d){TMP2=abs(danefali[x][y].amp[1]-danefali[x][y].amp[3]);}
    else if(b){TMP2=danefali[x][y].amp[1];}
    else {TMP2=danefali[x][y].amp[3];}

    if(TMP1!=0&&TMP2!=0){
        TMP3=sqrt((TMP1*TMP1)+(TMP2*TMP2));
        if(a){danefali[x][y].amp[0]=round(TMP3);}
        if(b){danefali[x][y].amp[1]=round(TMP3);}
        if(c){danefali[x][y].amp[2]=round(TMP3);}
        if(d){danefali[x][y].amp[3]=round(TMP3);}
    }
    else if (TMP1!=0){
        if(a){danefali[x][y].amp[0]=round(TMP1);}
        if(c){danefali[x][y].amp[2]=round(TMP1);}
    }
    else if (TMP2!=0){
        if(b){danefali[x][y].amp[1]=round(TMP2);}
        if(d){danefali[x][y].amp[3]=round(TMP2);}
    }
}

void zerowanie(int x, int y){
    for (int i=0;i<4;i++){
        if(danefali[x][y].state[i]==2){
            danefali[x][y].id[i]=0;
            danefali[x][y].dir[i]=0;
            danefali[x][y].amp[i]=0;
            danefali[x][y].hp[i]=0;
            danefali[x][y].state[i]=0;
        }
    }
}

void rozchodzenie(int x, int y){
    obliczanieAmp(x,y);
    auto [a,b,c,d] =kierunki(x,y);
    if(!(a || b || c || d)){return;}
    int direction=5; //where to take data from
    for (int i=0;i<4;i++){
        if(danefali[x][y].dir[i]!=0){
            direction=i;
            break;
        }
    }
    if (direction==5){return;}
    // Function to check if a cell is a wall
    auto isWall = [&](int i, int j) {
        return room[i][j] == 1;
    };

    if(a && danefali[x][y - 1].state[0] != 2){ //left
        if (!isWall(x, y - 1)) {
            danefali[x][y - 1].id[0] = danefali[x][y].id[direction];
            danefali[x][y - 1].dir[0] = 1;
            danefali[x][y - 1].amp[0] = danefali[x][y].amp[direction]*(0.75);
            danefali[x][y - 1].hp[0] = danefali[x][y].hp[direction] - 1;
            danefali[x][y - 1].state[0] = 1;
        } else {
            // Bounce off the wall
            danefali[x][y].dir[2] = 3;  // Change direction to the right
            danefali[x][y].amp[2] =danefali[x][y].amp[direction]/2;  // Reduce amplitude
            danefali[x][y].state[2] = 1;  // Set state to passive to avoid further propagation
        }
    }
    if(b && danefali[x-1][y].state[1] != 2){ //up
        if (!isWall(x-1, y)) {
            danefali[x-1][y].id[1] = danefali[x][y].id[direction];
            danefali[x-1][y].dir[1] = 2;
            danefali[x-1][y].amp[1] = danefali[x][y].amp[direction]*(0.75);
            danefali[x-1][y].hp[1] = danefali[x][y].hp[direction] - 1;
            danefali[x-1][y].state[1] = 1;
        } else {
            // Bounce off the wall
            danefali[x][y].dir[3] = 4;
            danefali[x][y].amp[3] =danefali[x][y].amp[direction]/2;  // Reduce amplitude (you can adjust this as needed)
            danefali[x][y].state[3] = 1;  // Set state to passive to avoid further propagation
        }
    }
    if(c && danefali[x][y + 1].state[2] != 2){ //right
        if (!isWall(x, y + 1)) {
            danefali[x][y + 1].id[2] = danefali[x][y].id[direction];
            danefali[x][y + 1].dir[2] = 3;
            danefali[x][y + 1].amp[2] = danefali[x][y].amp[direction]*(0.75);
            danefali[x][y + 1].hp[2] = danefali[x][y].hp[direction] - 1;
            danefali[x][y + 1].state[2] = 1;
        } else {
            // Bounce off the wall
            danefali[x][y].dir[0] = 1;
            danefali[x][y].amp[0] =danefali[x][y].amp[direction]/2;  // Reduce amplitude (you can adjust this as needed)
            danefali[x][y].state[0] = 1;  // Set state to passive to avoid further propagation
        }
    }
    if(d && danefali[x+1][y].state[3] != 2){ //down
        if (!isWall(x+1, y)) {
            danefali[x+1][y].id[3] = danefali[x][y].id[direction];
            danefali[x+1][y].dir[3] = 4;
            danefali[x+1][y].amp[3] = danefali[x][y].amp[direction]*(0.75);
            danefali[x+1][y].hp[3] = danefali[x][y].hp[direction] - 1;
            danefali[x+1][y].state[3] = 1;
        } else {
            // Bounce off the wall
            danefali[x][y].dir[1] = 2;
            danefali[x][y].amp[1] =danefali[x][y].amp[direction]/2;  // Reduce amplitude (you can adjust this as needed)
            danefali[x][y].state[1] = 1;  // Set state to passive to avoid further propagation
        }
    }
    zerowanie(x,y);
}

void aktywacja(int x, int y){
    int k=1;
    while(room[x-k][y-k]!=1){
        for (int i = -k; i < k + 1; i++) {
            for(int m=0;m<4;m++){
                if(danefali[x-k][y+i].state[m]==1){
                    danefali[x-k][y+i].state[m]=2;
                }
            }
        }
        for (int j = -k + 1; j < k + 1; j++) {
            for(int m=0;m<4;m++){
                if(danefali[x+j][y+k].state[m]==1){
                    danefali[x+j][y+k].state[m]=2;
                }
            }
        }
        for (int i = k - 1; i > -k - 1; i--) {
            for(int m=0;m<4;m++){
                if(danefali[x+k][y+i].state[m]==1){
                    danefali[x+k][y+i].state[m]=2;
                }
            }
        }
        for (int j = k - 1; j > -k - 1; j--) {
            for(int m=0;m<4;m++){
                if(danefali[x+j][y-k].state[m]==1){
                    danefali[x+j][y-k].state[m]=2;
                }
            }
        }
        k++;
    }
}


int przejscie(int x, int y,int licznik){ // x,y=współrzędne źródła
    if(licznik==0){
        danefali[x][y - 1] = {{licznik, 0, 0, 0},
                              {1,       0, 0, 0},
                              {AMPLITUDE,      0, 0, 0},
                              {100,       0, 0, 0},
                              {2,       0, 0, 0}};
        danefali[x - 1][y] = {{0, licznik, 0, 0},
                              {0, 2,       0, 0},
                              {0, AMPLITUDE,0, 0},
                              {0, 100,       0, 0},
                              {0, 2,       0, 0}};
        danefali[x][y + 1] = {{0, 0, licznik, 0},
                              {0, 0, 3,       0},
                              {0, 0, AMPLITUDE,0},
                              {0, 0, 100,       0},
                              {0, 0, 2,       0}};
        danefali[x + 1][y] = {{0, 0, 0, licznik},
                              {0, 0, 0, 4},
                              {0, 0, 0, AMPLITUDE},
                              {0, 0, 0, 100},
                              {0, 0, 0, 2}};
        print();
        cout << "START" << endl;
    }

    int k = 1;
    while (room[x - k][y - k] != 1) {
        for (int i = -k+1; i < k + 1; i++) {
            rozchodzenie(x-k,y+i);
        }
        for (int j = -k + 1; j < k + 1; j++) {
            rozchodzenie(x+j,y+k);
        }
        for (int i = k - 1; i > -k - 1; i--) {
            rozchodzenie(x+k,y+i);
        }
        for (int j = k - 1; j > -k-1; j--) {
            rozchodzenie(x+j,y-k);
        }
        k++;
    }
    aktywacja(x,y);
    return licznik;
}


int main(){

    for (int i=0;i<=LEN-1;i++){
        for(int j=0; j<=LEN-1;j++){
            danefali[i][j]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
        }
    }
    int licznik=0;
    rpodglad();

    int center =(LEN-1)/2;
    cout << center << " " << center << endl;
    room[center][center]=25;//źródło
    for (int i=0;i<25;i++){
        licznik=przejscie(center, center,licznik);
        licznik++;
    }

    cout <<"End" << endl;
    print();

    float wynik[3]={0,0,0.0};
    for (int i = 1; i < (LEN/5 - 1); i++) {
        for (int j = 1; j <= LEN / 5 - 1; j++) {
            for (int k=0; k<4;k++){
                if(wynik[2]<danefali[i][j].amp[k]){
                    wynik[0]=i;
                    wynik[1]=j;
                    wynik[2]=danefali[i][j].amp[k];
                }
            }
        }
    }
    cout << "Highest amplitude was found at " << wynik[0] << "," << wynik[1] << " and reached " << wynik[2];
    return 0;
}