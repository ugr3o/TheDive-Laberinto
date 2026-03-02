#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <chrono>

using namespace std;
using namespace chrono;

struct Punto {
    int fila;
    int col;
};

long long medirTiempo(high_resolution_clock::time_point inicio){
    auto fin = high_resolution_clock::now();
    return duration_cast<microseconds>(fin - inicio).count();
}
// funcion para crear el tablero que se encuentra en memoria
vector<vector<char>> crearTablero(int filas, int cols){
        vector<vector<char>> tablero(filas, vector<char>(cols, '#'));
        return tablero;
    }
//funcion para imprimir el tablero en consola
void imprimirTablero(vector<vector<char>>& tablero,int filas,int cols){

     for (int i =0; i< filas; i++){
        for (int j= 0; j < cols; j++){
            cout << tablero[i][j] << " ";
        }
        cout << endl;
    }
}
//funcion para mostrar ubicacion de entrada y salida dentro del tablero
void entradaSalida(vector<vector<char>>& tablero, int filas, int cols){
    tablero[0][0] = 'E';
    tablero[filas -1][cols -1] = 'S';
}
//generamos el laberinto con DFS, Depth-First-Search
void generarLaberinto(vector<vector<char>>& tablero, int fila, int col, int filas, int cols){
    int orden[] = {0,1,2,3};
    for(int i =3; i > 0; i--){
        int j = rand() % (i + 1);
        swap(orden[i],orden[j]);
    }
    int df[] = {-2,+2,0,0};
    int dc[] = {0,0,-2,+2};
    for(int i =0; i < 4; i++){
        int nuevaFila = fila + df[orden[i]];
        int nuevaCol = col + dc[orden[i]];

        if(nuevaFila >= 0 && nuevaFila < filas && nuevaCol >= 0 && nuevaCol < cols && tablero[nuevaFila][nuevaCol] == '#'){
            tablero[fila + df[orden[i]]/2][col + dc[orden[i]]/2] = ' ';
            tablero[nuevaFila][nuevaCol] = ' ';
            generarLaberinto(tablero, nuevaFila, nuevaCol, filas, cols);
        }
    }
}

void encontrarSalida(vector<vector<char>>& tablero, int fila, int col, int filas, int cols){
   queue<Punto> explorar;
   vector<vector<bool>> visitado(filas,vector<bool>(cols,false));
   vector<vector<Punto>> padre(filas,vector<Punto>(cols,{-1,-1}));
   explorar.push({0,0});
   visitado[0][0]= true;

   while(!explorar.empty()){
        Punto actual = explorar.front(); //utilizamos "Punto" el struct, porque "actual" tiene que guardar las coordendas, fila y col
        explorar.pop();
        if (actual.fila == filas -1 && actual.col == cols -1){
            break;
        }
        int df[] = {-1,+1,0,0};
        int dc[] = {0,0,-1,+1};
        for(int i = 0; i < 4; i++){
            int nuevaFila = actual.fila + df[i];
            int nuevaCol = actual.col + dc[i];
            if(nuevaFila >= 0 && nuevaFila < filas && nuevaCol >= 0 && nuevaCol < cols && tablero[nuevaFila][nuevaCol] != '#' && !visitado[nuevaFila][nuevaCol]){
                visitado[nuevaFila][nuevaCol] = true;
                padre[nuevaFila][nuevaCol] = {actual.fila,actual.col};
                explorar.push({nuevaFila,nuevaCol});
            }
        }
   }
   Punto actual = {filas-1,cols-1};
   while(!(actual.fila == 0 && actual.col ==0)){
    if(tablero[actual.fila][actual.col] != 'S'){
        tablero[actual.fila][actual.col] = '*';
    }
    actual = padre[actual.fila][actual.col];
   }

}

//main donde llamare a todas las funciones
int main(){
    srand(time(0));

    int filas = 0;
    int cols = 0;
    
    cout << "Ingrese el numero de filas: ";
    cin >> filas;
    cout << "Ingrese el numero de columnas: ";
    cin >> cols;

    if(filas % 2 == 0) filas++;
    if(cols % 2 == 0) cols++;

    vector<vector<char>> tablero = crearTablero(filas,cols);
    tablero[1][1] = ' ';

    auto inicio = high_resolution_clock::now();
    generarLaberinto(tablero,1,1,filas,cols);
    cout << "Tiempo generacion: " << medirTiempo(inicio) << " microsegundos" << endl;

    entradaSalida(tablero,filas,cols);
    tablero[0][1] = ' ';
    tablero[filas-2][cols-1] = ' ';
    imprimirTablero(tablero,filas,cols);

    auto inicio2 = high_resolution_clock::now();
    encontrarSalida(tablero,0,0,filas,cols);
    cout << "Tiempo resolucion: " << medirTiempo(inicio2) << " microsegundos" << endl;

    imprimirTablero(tablero,filas,cols);
    return 0;
}