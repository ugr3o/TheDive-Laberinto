#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// funcion para crear el tablero
vector<vector<char>> crearTablero(int filas, int cols){
        vector<vector<char>> tablero(filas, vector<char>(cols, '#'));
        return tablero;
    }
//funcion para imprimir el tablero en consola
void ImprimirTablero(vector<vector<char>>& tablero,int filas,int cols){

     for (int i =0; i< filas; i++){
        for (int j= 0; j < cols; j++){
            cout << tablero[i][j] << " ";
        }
        cout << endl;
    }
}
//funcion para mostrar ubicacion de entrada y salida dentro del tablero
void EntradaSalida(vector<vector<char>>& tablero, int filas, int cols){
    tablero[0][0] = 'E';
    tablero[filas -1][cols -1] = 'S';
}
//generamos el laberinto con DFS, Depth-First-Search
void GenerarLaberinto(vector<vector<char>>& tablero, int fila, int col, int filas, int cols){
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
            GenerarLaberinto(tablero, nuevaFila, nuevaCol, filas, cols);
        }
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

    vector<vector<char>> tablero = crearTablero(filas,cols);
    tablero[1][1]= ' ';
    GenerarLaberinto(tablero,1,1,filas,cols);
    EntradaSalida(tablero,filas,cols);
    ImprimirTablero(tablero,filas,cols);
    return 0;
}